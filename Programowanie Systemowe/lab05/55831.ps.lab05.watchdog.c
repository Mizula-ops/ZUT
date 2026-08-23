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
typedef struct{
    pid_t pid;
    int last_seq;
    time_t last_heartbeat;
    int active;
}WorkerInfo;

WorkerInfo *workers =NULL;
int worker_count =0;
int stop=0;
void heartbeat_handler(int sig, siginfo_t *info, void *ucontext){
    pid_t sender_pid = info->si_pid;
    int value = info->si_value.sival_int;
    int seq=value/10;
    int type=value%10;
    for (int i=0;i<worker_count;i++){
        if(workers[i].active && workers[i].pid==sender_pid ){
            if(type==0)
                printf("[RECV] PID=%d seq=%d type=HEARTBEAT\n",sender_pid,seq);
            else if(type==1)
                printf("[RECV] PID=%d seq=%d type=FAILURE_SOFT\n",sender_pid,seq);
            if (workers[i].last_seq != 0 && seq >workers[i].last_seq +1){
                for(int m=workers[i].last_seq+1; m<seq; m++){
                    printf("[WARNING] PID=%d missing seq: %d\n", sender_pid, m);
                }
            }
            workers[i].last_seq =seq;
            workers[i].last_heartbeat = time(NULL);
            break;
        }
    }
}
void sigint_handler(int sig){
    stop=1;
}
pid_t spawnWorker(char *H){
    pid_t pid=fork();
    if(pid==0){
        char watchdog_pid[32];

        snprintf(watchdog_pid, sizeof(watchdog_pid), "%d", getppid());
        char *args[]={"./worker",watchdog_pid,H,NULL};
        execvp(args[0],args);

    
    }
    return pid;
}

int main(int argc, char *argv[]){
    if(argc !=4){
        fprintf(stderr,"Bład za mala liczba argumentow");
        return 1;
    }
   for (int i = 1; i < 4; i++) {
        for (int j = 0; argv[i][j] != '\0'; j++) {
            if (!isdigit(argv[i][j])) {
                fprintf(stderr, "Argumenty muszą być liczbami\n");
                return 1;
            }
        }
    }
   
    int N = atoi(argv[1]);
    int H = atoi(argv[2]);
    int T = atoi(argv[3]);
    struct sigaction act;
    act.sa_sigaction = heartbeat_handler;
    act.sa_flags = SA_SIGINFO;
    sigemptyset(&act.sa_mask);
    sigaction(SIGRTMIN, &act, NULL);
    
    struct sigaction act_int;
    act_int.sa_handler=sigint_handler;
    act_int.sa_flags=0;
    sigemptyset(&act_int.sa_mask);
    sigaction(SIGINT, &act_int, NULL);
    
    workers=malloc(N*sizeof(WorkerInfo));
     worker_count=N;
     for(int i=0;i<N;i++){
        workers[i].pid=spawnWorker(argv[2]);
        workers[i].last_heartbeat=time(NULL);
        workers[i].last_seq=0;
        workers[i].active=1;
     }
    

     while(!stop){
        sleep(1);
        time_t now=time(NULL);
        for(int i=0;i<worker_count; i++){
            if(workers[i].active && now - workers[i].last_heartbeat >T){
                printf("[TIMEOUT] PID=%d inactive\n", workers[i].pid);
                kill(workers[i].pid, SIGKILL);
                waitpid(workers[i].pid,NULL,0);

                workers[i].pid=spawnWorker(argv[2]);
                workers[i].last_heartbeat=time(NULL);
                workers[i].last_seq=0;
                workers[i].active=1;
            }
        }
     }

     for (int i = 0; i < worker_count; i++) {
        if (workers[i].active) {
            kill(workers[i].pid, SIGTERM);
            waitpid(workers[i].pid, NULL, 0);
        }
    }
        free(workers);
     
}
    
