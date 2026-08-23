//PS IS1 322 LAB06
//Artur Mizuła
//ma55831@zut.edu.pl

#include <pthread.h>
#include <time.h>
#include <stdlib.h>
static pthread_key_t timeKey;
static pthread_once_t timeOnce = PTHREAD_ONCE_INIT;
static void freeMemory(void *ptr){
    free(ptr);
}
static void createKey(){
    pthread_key_create(&timeKey,freeMemory);
}

void start(){
    struct timespec *t;
    pthread_once(&timeOnce, createKey);
    t=pthread_getspecific(timeKey);
    if(t == NULL){
        t=malloc(sizeof(struct timespec));
        pthread_setspecific(timeKey,t);
    }
    clock_gettime(CLOCK_REALTIME,t)
;
}
long stop(){
    struct timespec *t_start;
    struct timespec t_end;
    pthread_once(&timeOnce, createKey);
    t_start=pthread_getspecific(timeKey);
    if (t_start == NULL) {
        return -1;
    }
    clock_gettime(CLOCK_REALTIME, &t_end);
    long sec = (t_end.tv_sec - t_start->tv_sec) * 1000L;
    long ms = (t_end.tv_nsec - t_start->tv_nsec) / 1000000L;
    return sec+ms;
}