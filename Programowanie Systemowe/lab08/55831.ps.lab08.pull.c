
#define _GNU_SOURCE  
#include <unistd.h>     
#include <crypt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <errno.h>
typedef struct{
    long mtype;
    size_t start;
    size_t end;

    key_t shm_key;
    key_t sem_key;

    char filename[256];
    char salt[512];
    char hash[512];
} Cracker;
typedef struct{
    int finished_tasks;
    int found;
    int active_task;
    char password[256];
}SharedData;
int has_task=0;
int msgid;
int semid;
SharedData *shared = NULL;
Cracker current_task;
int ile=0;
char *buff = NULL;
int p1 = -1;
size_t size = 0;
void lock_sem() {
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;
    sb.sem_flg = SEM_UNDO;
    semop(semid, &sb, 1);
}

void unlock_sem() {
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = 1;
    sb.sem_flg = SEM_UNDO;
    semop(semid, &sb, 1);
}

void handler(int sig) {
    if (has_task) {
        has_task = 0;
        while (msgsnd(msgid, &current_task,sizeof(Cracker)-sizeof(long),IPC_NOWAIT) == -1) {
            if (shared->found) {
                return;
            }
            if (errno == EAGAIN) {
                sleep(1);
            } else {
                perror("msgsnd return_task");
                break;
            }
        }
        printf("Zwrocono task start=%zu end=%zu do kolejki\n",current_task.start,current_task.end);
        lock_sem();
        if (shared != NULL && shared->active_task > 0)
            shared->active_task--;
        unlock_sem();
    }
    exit(1);

}
int main(int argc, char *argv[]){  

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    if(argc != 3){
        fprintf(stderr,"Bład za mala liczba argumentow");
        return 1;
    }
   
    key_t key=atoi(argv[1]);
    int numbers=atoi(argv[2]);
    msgid =msgget(key,0);
    int shmid;
    printf("Worker PID: %d\n",getpid());
    for(int i=0; i<numbers;i++){
        if (shared != NULL && shared->found) {
            return 0;
        }
        Cracker task;
        int res = -1;
        while (1) {
            res = msgrcv(msgid, &task, sizeof(Cracker) - sizeof(long), 1, IPC_NOWAIT);
            if (res != -1) {
                break; 
            }
            if (errno == ENOMSG) {
            if (shared != NULL && shared->found) {
                return 0;
            }
                sleep(1);
                continue;
            }
            perror("msgrcv");
            return 1;
        }

        current_task = task;
        has_task = 1;


        
        if (shared == NULL) {
            shmid = shmget(task.shm_key, sizeof(SharedData), 0600);
            if (shmid == -1) {
                perror("shmget");
                return 1;
            }

            shared = shmat(shmid, NULL, 0);
            if (shared == (void*)-1) {
                perror("shmat");
                return 1;
            }

            semid = semget(task.sem_key, 1, 0600);
            if (semid == -1) {
                perror("semget");
                return 1;
            }
        }
        
        
        lock_sem();
        shared->active_task++;
        unlock_sem();

        if (buff == NULL) {
            p1 = open(task.filename, O_RDONLY);
            struct stat st;
            fstat(p1, &st);
            size = st.st_size;
            buff = mmap(NULL, size, PROT_READ, MAP_PRIVATE, p1, 0);
        }


       
        printf("msgrcv res = %d\n", res);
        printf("Pobrano task: start=%zu end=%zu\n",task.start, task.end);
        struct crypt_data cdata;
        cdata.initialized = 0;
        size_t i = task.start;
    
        while (i < task.end) {
        
        if (ile % 1000 == 0) {
            int already_found = shared->found;

            if (already_found) {
                lock_sem();
                if (shared->active_task > 0)
                    shared->active_task--;
                unlock_sem();
                has_task = 0;
                break;
            }
        }
            char *start = &buff[i];
            size_t len=0;
            while (i < task.end && buff[i] != '\n') {
                i++;
                len++;
            }
            i++;    
            if (len == 0) {
            continue;
            }
            char *word=malloc(len+1);
            memcpy(word,start,len);
            word[len] = '\0';    

            char *res = crypt_r(word, task.salt, &cdata);
            ile++;
            if(ile%1000==0){
                printf("\r Haslo numer: %d",ile);
                fflush(stdout);
            }
                
           
            if (res != NULL && strcmp(res, task.hash) == 0) {
                lock_sem();
                if (shared->active_task > 0)
                    shared->active_task--;
                shared->finished_tasks++;
                strncpy(shared->password, word, sizeof(shared->password) - 1);
                shared->password[sizeof(shared->password) - 1] = '\0';
                shared->found=1;
                has_task=0;
                unlock_sem();
                printf("Znalezione haslo: %s\n", word);
                free(word);
                return 0;
            }
            free(word);
        }
        has_task = 0;   
        lock_sem(); 
        if (shared->active_task > 0)
            shared->active_task--;
        shared->finished_tasks++;
        unlock_sem();
        
    }
    return 0;
    

    
}