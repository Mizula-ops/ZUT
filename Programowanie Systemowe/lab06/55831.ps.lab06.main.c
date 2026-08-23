//PS IS1 322 LAB06
//Artur Mizuła
//ma55831@zut.edu.pl

#include <stdlib.h>
#include <ctype.h>
#include <sys/wait.h>
#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include "timer.h"
typedef struct{
    pthread_t tid;
    long time_live;
    time_t should_stop;


}WorkerInfo;

WorkerInfo *workers=NULL;
int worker_count=0;


void signal_handler(int sig) {
    pthread_t self = pthread_self();

    for (int i = 0; i < worker_count; i++) {
        if (workers[i].tid == self) {
            workers[i].should_stop = 1;
            break;
        }
    }
}

static void *threadFunc(void *arg){
    WorkerInfo *worker = (WorkerInfo *)arg;
    start();
    long long n = 1;
    while (!worker->should_stop) {
        for (int i = 1; i < 10000; i++) {
            n = n * i;
            if (n > 1000000) {
                n = 1;
            }
        }
    }
    long time_ms=stop();
    printf("Wątek %lu zakonczyl prace, czas= %ld ms\n",pthread_self(),time_ms);
    return NULL;
}

int main(int argc, char *argv[]){
    if(argc !=3){
        fprintf(stderr,"Bład za mala liczba argumentow");
        return 1;
    }
   for (int i = 1; i < 3; i++) {
        for (int j = 0; argv[i][j] != '\0'; j++) {
            if (!isdigit(argv[i][j])) {
                fprintf(stderr, "Argumenty muszą być liczbami\n");
                return 1;
            }
        }
    }
    
    int T = atoi(argv[1]);
    int N = atoi(argv[2]);
    srand(time(NULL));
    workers=malloc(N*sizeof(WorkerInfo));
    worker_count=N;

    struct sigaction act;
    act.sa_handler = signal_handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGUSR1, &act, NULL);

   
    struct timespec global_start;
    clock_gettime(CLOCK_REALTIME, &global_start);
    for(int i=0;i<N;i++){
            workers[i].time_live=1+rand()%(T*1000);
            workers[i].should_stop =0;
            pthread_create(&workers[i].tid,NULL,threadFunc,&workers[i]);
            printf("Utworzono watek %lu, czas zycia: %ld ms\n",workers[i].tid, workers[i].time_live);
    }
    int finish=0;
    while (finish < N) {
        usleep(1000);

        struct timespec now;
        clock_gettime(CLOCK_REALTIME, &now);

        long diff_ms = (now.tv_sec - global_start.tv_sec) * 1000L
                 + (now.tv_nsec - global_start.tv_nsec) / 1000000L;

        for (int i = 0; i < N; i++) {
            if (!workers[i].should_stop && diff_ms >= workers[i].time_live) {
                pthread_kill(workers[i].tid, SIGUSR1);
                finish++;
            }
        }
    }
    for(int i=0;i<N;i++){
            pthread_join(workers[i].tid,NULL);
        
    }
    free(workers);

}

