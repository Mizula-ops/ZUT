
//PS IS1 322 LAB07
//Artur Mizuła
//ma55831@zut.edu.pl
#define _GNU_SOURCE  
#include <unistd.h>     
#include <crypt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/mman.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>
#include <signal.h>
typedef struct{
    long mtype;
    size_t start;
    size_t end;

    key_t shm_key;
    key_t sem_key;

    char filename[256];
    char salt[512];
    char hash[512];
}Cracker;
typedef struct{
    int finished_tasks;
    int found;
    int active_task;
    char password[256];
}SharedData;

typedef struct {
    int number;
    size_t size;
    char *buff;
    Cracker *data;
    int msgid;
    key_t shm_key;
    key_t sem_key;
    char *filename;
    char *hash;
    char *salt;
    SharedData *shared;
} QueueArgs;
int msgid = -1;
int shmid = -1;
int semid = -1;
SharedData *shared =NULL;
void cleanup(){
    if (msgid != -1) msgctl(msgid, IPC_RMID, NULL);
    if (shmid != -1) shmctl(shmid, IPC_RMID, NULL);
    if (semid != -1) semctl(semid, 0, IPC_RMID);
}
void handler(int sig) {
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;
    sb.sem_flg = SEM_UNDO;
    semop(semid, &sb, 1);
    shared->found = 1;
    sb.sem_op = 1;
    semop(semid, &sb, 1);


    cleanup();
    exit(1);

}
int tasks=0;
void *queque(void *arg){
    QueueArgs *a = (QueueArgs *)arg;
     for(int i=0;i<a->number;i++){
            if(a->shared->found){
                break;
            }
            size_t start = i*a->size/a->number;
            size_t end = (i+1) *a->size/a->number;

            if(i!=0){
            while (start < a->size && a->buff[start - 1] != '\n') 
                start++;
            }
            if(i!=a->number-1){
            while (end < a->size && a->buff[end] != '\n') 
                end++;
            }
            a->data[i].start = start;
            a->data[i].end = end;
            a->data[i].mtype=1;
            a->data[i].shm_key = a->shm_key;
            a->data[i].sem_key = a->sem_key;
            strncpy(a->data[i].filename, a->filename, sizeof(a->data[i].filename) - 1);
            a->data[i].filename[sizeof(a->data[i].filename) - 1] = '\0';
            strncpy(a->data[i].salt, a->salt, sizeof(a->data[i].salt) - 1);

            a->data[i].salt[sizeof(a->data[i].salt) - 1] = '\0';
           strncpy(a->data[i].hash, a->hash, sizeof(a->data[i].hash) - 1);
            a->data[i].hash[sizeof(a->data[i].hash) - 1] = '\0';

            while (msgsnd(a->msgid, &a->data[i], sizeof(Cracker) - sizeof(long), IPC_NOWAIT) == -1) {
                if (a->shared->found) {
                    return NULL;
                }
                if (errno == EAGAIN) {
                    sleep(10);
                } else {
                    perror("msgsnd");
                    return NULL;
                }
            }
            printf("Wygenerowano zadanie %d start: %zu koniec: %zu \n", i, a->data[i].start, a->data[i].end);
            tasks=i+1;
        }
        return NULL;

}
int main(int argc, char *argv[]){   
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    if(argc != 4){
        fprintf(stderr,"Bład za mala liczba argumentow");
        return 1;
    }

    char *hash=argv[1];
    char *filename=argv[2];
    int number=atoi(argv[3]);
    int p1=open(argv[2],O_RDONLY);

    if(p1 < 0){
        fprintf(stderr,"Nie mozna otworzyc pliku");
        return 1;
    }
    struct stat st;
    fstat(p1, &st);
    size_t size = st.st_size;
    char *buff = mmap(NULL, size, PROT_READ, MAP_PRIVATE, p1, 0);
    size_t total_words = 0;
    for (size_t i=0; i<size; i++){
        if (buff[i] == '\n') {
                total_words++;
            }
    }
   
    char salt[512];
    int dollars = 0;
    int k = 0;
    while (hash[k] != '\0') {
        if (hash[k] == '$') {
            dollars++;
            if (dollars == 3) {
                k++;
                break;
            }
        }
        k++;
    }
    memcpy(salt, hash, k);
    salt[k] = '\0';

    key_t shm_key = ftok(".", 's');
    shmid = shmget(shm_key, sizeof(SharedData), IPC_CREAT | 0600);
    if(shmid == -1) {
        perror("shmget");
        return 1;
    }

    shared = shmat(shmid, NULL, 0);
    if(shared == (void*)-1) {
        perror("shmat");
        return 1;
    }
    shared->finished_tasks=0;
    shared->found =0;
    shared->active_task=0;
    shared->password[0]='\0';

    key_t sem_key = ftok(".", 'x');
    semid = semget(sem_key, 1, IPC_CREAT | 0600);
    if (semid == -1) {
        perror("semget");
        return 1;
    }
    semctl(semid, 0, SETVAL, 1);

    Cracker *data = malloc(number * sizeof(Cracker));
        if (data == NULL) {
        perror("malloc");
        return 1;
    }
    key_t key = ftok(".",'z');
    msgid= msgget(key,IPC_CREAT | 0600);
        if(msgid == -1) {
        perror("msgget");
        return 1;

        }
    printf("msg_key: %d\n", key);
    printf("msg_id: %d\n", msgid);
    QueueArgs args;
    args.number=number;
    args.size=size;
    args.buff=buff;
    args.data=data;
    args.msgid=msgid;
    args.shm_key=shm_key;
    args.sem_key=sem_key;
    args.filename=filename;
    args.hash=hash;
    args.salt=salt;
    args.shared=shared;
    pthread_t tid;
    pthread_create(&tid,NULL,queque,&args);
    
  
    printf("Ilosc Wszystkich taskow %d",number);
    while(shared->finished_tasks < number && !shared->found) {
    printf("\rWszystkie: %d | Wygenerowane: %d | Skonczone: %d | Aktywne: %d | Do pobrania: %d",
    number,
    tasks,
    shared->finished_tasks,
    shared->active_task,
    tasks - shared->finished_tasks - shared->active_task);
    fflush(stdout);
    sleep(5);
     printf("\n");
    }
    pthread_join(tid, NULL);
    if (shared->found) {
    printf("Znalezione haslo: %s\n", shared->password);
    } else {
    printf("Nie znaleziono hasla\n");
    }

    cleanup();
    free(data);
    return 0;
}