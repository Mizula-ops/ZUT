
//PS IS1 322 LAB05
//Artur Mizuła
//ma55831@zut.edu.pl

#include <stdlib.h>
#include <ctype.h>
#include <sys/wait.h>
#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>


int H=0;
int stop=1;
int seq=0;
int do_skip=0;
int do_soft=0;
void skip_handler(int sig){
    do_skip=1;
}
void soft_handler(int sig){
    do_soft=1;
}
int main(int argc, char *argv[]){
    if(argc !=3){
        fprintf(stderr,"Bład za mala liczba argumentow");
        return 1;
    }
    int pidWatchdog = atoi(argv[1]);
    int H = atoi(argv[2]);
    
    struct sigaction act1;
    act1.sa_handler = skip_handler;
    act1.sa_flags = 0;
    sigemptyset(&act1.sa_mask);
    sigaction(SIGUSR1, &act1, NULL);
    
    struct sigaction act2;
    act2.sa_handler = soft_handler;
    act2.sa_flags = 0;
    sigemptyset(&act2.sa_mask);
    sigaction(SIGUSR2, &act2, NULL);
    
   struct timespec timeh, timerest;
    timeh.tv_sec=H;
    timeh.tv_nsec=0;
    
    while(1){
        seq++;
        if (do_skip){
            printf("[LOCAL] PID=%d seq=%d type=SKIPPED\n",getpid(),seq);
            do_skip=0;
        }
        else if(do_soft){
            printf("[LOCAL] PID=%d seq=%d type=FAILURE_SOFT\n",getpid(),seq);
            union sigval sv;
            sv.sival_int=seq*10+1;
            sigqueue(pidWatchdog, SIGRTMIN, sv);
            do_soft=0;
        }
        else{
            printf("[LOCAL] PID=%d seq=%d type=HEARTBEAT\n",getpid(),seq);
            union sigval sv;
            sv.sival_int =seq*10+0;
            sigqueue(pidWatchdog,SIGRTMIN,sv);
        } 
        sleep(H);
        while(nanosleep(&timeh,&timerest)==-1){
        timeh=timerest;
    }
    }
     
}