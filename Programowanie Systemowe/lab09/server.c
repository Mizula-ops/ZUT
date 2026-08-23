#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>

volatile sig_atomic_t running = 1;
FILE *log_file = NULL;
char client_ips[1024][INET_ADDRSTRLEN];
void handle_sigint(int sig) {
    running = 0;
}

void close_log() {
    if (log_file != NULL) {
        fclose(log_file);
        log_file = NULL;
    }
}

void send_error(int client_fd, int code, const char *status) {
    char body[128];
    char header[256];

    int body_len = snprintf(body, sizeof(body), "%d %s", code, status);

    int header_len = snprintf(header, sizeof(header),
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n"
        "\r\n",
        code, status, body_len);

    write(client_fd, header, header_len);
    write(client_fd, body, body_len);
}

const char *get_mime_type(const char *path) {
    char *ext = strrchr(path, '.');

    if (ext == NULL) return "application/octet-stream";

    if (strcmp(ext, ".html") == 0) return "text/html";
    if (strcmp(ext, ".txt") == 0) return "text/plain";
    if (strcmp(ext, ".css") == 0) return "text/css";
    if (strcmp(ext, ".js") == 0) return "application/javascript";
    if (strcmp(ext, ".jpg") == 0) return "image/jpeg";
    if (strcmp(ext, ".jpeg") == 0) return "image/jpeg";
    if (strcmp(ext, ".png") == 0) return "image/png";
    if (strcmp(ext, ".gif") == 0) return "image/gif";

    return "application/octet-stream";
}

int send_file(int client_fd, const char *filepath) {
    struct stat st;

    if (stat(filepath, &st) == -1) {
        send_error(client_fd, 404, "Not Found");
        return 404;
    }

    if (!S_ISREG(st.st_mode)) {
        send_error(client_fd, 403, "Forbidden");
        return 403;
    }

    if (access(filepath, R_OK) == -1) {
        send_error(client_fd, 403, "Forbidden");
        return 403;
    }

    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        send_error(client_fd, 403, "Forbidden");
        return 403;
    }

    const char *mime = get_mime_type(filepath);

    char header[512];
    int header_len = snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %ld\r\n"
        "Connection: close\r\n"
        "\r\n",
        mime,
        st.st_size);

    write(client_fd, header, header_len);

    char file_buffer[4096];
    ssize_t n;

    while ((n = read(fd, file_buffer, sizeof(file_buffer))) > 0) {
        write(client_fd, file_buffer, n);
    }

    close(fd);
    return 200;
}
void write_log(const char *ip, const char *method, const char *path, int code) {
    if (log_file == NULL)
        return;

    flockfile(log_file);

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    char date[64];
    strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", tm_info);

    fprintf(log_file, "%s %s %s %s %d\n", date, ip, method, path, code);
    fflush(log_file);

    funlockfile(log_file);
}


int main(int argc, char *argv[]) {
    signal(SIGINT, handle_sigint);

    int opt;
    int port = -1;
    char *dir = NULL;

    while ((opt = getopt(argc, argv, "p:d:")) != -1) {
        switch (opt) {
            case 'p':
                port = atoi(optarg);
                break;
            case 'd':
                dir = optarg;
                break;
            default:
                fprintf(stderr, "Uzycie: %s -p port -d dir\n", argv[0]);
                return 1;
        }
    }

    if (port < 1 || port > 65535) {
        fprintf(stderr, "Blad: niepoprawny numer portu\n");
        return 1;
    }

    if (dir == NULL) {
        fprintf(stderr, "Blad: brak katalogu document root\n");
        return 1;
    }

    struct stat st;

    if (stat(dir, &st) == -1) {
        perror("Blad: katalog nie istnieje");
        return 1;
    }

    if (!S_ISDIR(st.st_mode)) {
        fprintf(stderr, "Blad: document root nie jest katalogiem\n");
        return 1;
    }

    if (access(dir, R_OK | X_OK) == -1) {
        perror("Blad: brak praw dostepu do katalogu");
        return 1;
    }

    char log_path[1024];
    snprintf(log_path, sizeof(log_path), "%s/server.log", dir);
    log_file = fopen(log_path, "a");
    if (log_file == NULL) {
        perror("fopen");
        return 1;
    }

    int lsfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lsfd == -1) {
        perror("socket");
        close_log();
        return 1;
    }

    int optval = 1;
    setsockopt(lsfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(lsfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(lsfd);
        close_log();
        return 1;
    }

    if (listen(lsfd, SOMAXCONN) == -1) {
        perror("listen");
        close(lsfd);
        close_log();
        return 1;
    }

    int epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1");
        close(lsfd);
        close_log();
        return 1;
    }

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = lsfd;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, lsfd, &ev) == -1) {
        perror("epoll_ctl");
        close(epfd);
        close(lsfd);
        close_log();
        return 1;
    }

    struct epoll_event events[64];

    while (running) {
        int n = epoll_wait(epfd, events, 64, -1);

        if (n == -1) {
            if (errno == EINTR)
                continue;

            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < n; i++) {
            if (events[i].data.fd == lsfd) {
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int client_fd = accept(lsfd,(struct sockaddr *)&client_addr,&client_len);
                if (client_fd == -1) {
                    perror("accept");
                    continue;
                }
                inet_ntop(AF_INET,&client_addr.sin_addr,client_ips[client_fd],INET_ADDRSTRLEN);
                struct epoll_event client_ev;
                client_ev.events = EPOLLIN;
                client_ev.data.fd = client_fd;

                if (epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &client_ev) == -1) {
                    perror("epoll_ctl client");
                    close(client_fd);
                }
            } else {
                int client_fd = events[i].data.fd;
                const char *ip = client_ips[client_fd];
                char buffer[4096];
                int bytes = read(client_fd, buffer, sizeof(buffer) - 1);

                if (bytes <= 0) {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, client_fd, NULL);
                    close(client_fd);
                    continue;
                }

                buffer[bytes] = '\0';

                char method[8];
                char path[512];
                char version[16];

                int parsed = sscanf(buffer, "%7s %511s %15s",method, path, version);

                if (parsed != 3) {
                    send_error(client_fd, 400, "Bad Request");
                    write_log(ip,"BAD", "-", 400);

                    epoll_ctl(epfd, EPOLL_CTL_DEL, client_fd, NULL);
                    close(client_fd);
                    continue;
                }

                if (strcmp(method, "GET") != 0) {
                    send_error(client_fd, 501, "Not Implemented");
                    write_log(ip,method, path, 501);

                    epoll_ctl(epfd, EPOLL_CTL_DEL, client_fd, NULL);
                    close(client_fd);
                    continue;
                }

                if (strstr(path, "..") != NULL || path[0] != '/') {
                    send_error(client_fd, 403, "Forbidden");
                    write_log(ip,method, path, 403);

                    epoll_ctl(epfd, EPOLL_CTL_DEL, client_fd, NULL);
                    close(client_fd);
                    continue;
                }

                char filepath[1024];

                if (strcmp(path, "/") == 0) {
                    snprintf(filepath, sizeof(filepath), "%s/index.html", dir);
                } else {
                    snprintf(filepath, sizeof(filepath), "%s%s", dir, path);
                }

                int code = send_file(client_fd, filepath);
                write_log(ip,method, path, code);

                epoll_ctl(epfd, EPOLL_CTL_DEL, client_fd, NULL);
                close(client_fd);
            }
        }
    }

    close(epfd);
    close(lsfd);
    close_log();

    return 0;
}