//PS IS1 322 LAB04
//Artur Mizuła
//ma55831@zut.edu.pl
#include <dlfcn.h>
#include <stdio.h>       // printf, fprintf
#include <string.h>      // strerror
#include <stdint.h>      // uint32_t
#include <systemd/sd-bus.h>
#include <bits/getopt_core.h>
#include <stdlib.h>
#include <grp.h>
#include <ctype.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
void get_time(char **cmd,int tflag,int vflag){
    double sum_real=0, sum_system=0, sum_user=0;
    struct timespec start, stop;
    clock_gettime(CLOCK_REALTIME, &start);
    double accum;
    for(int i=0;i<tflag;i++){
        pid_t pid=fork();
        if(pid ==0){
            if(!vflag){
                close(1);
                close(2);
                int h=open("/dev/null",O_WRONLY);
                dup2(h,1);
                dup2(h,2);
            }
            execvp(cmd[0],cmd);
    
        }
        else{
            int status;
            struct rusage usage;
            struct timespec start1, stop1;
            clock_gettime(CLOCK_REALTIME, &start1);
            wait4(pid,&status,0,&usage);
            printf("Petla numer: %d\n",i+1);
            printf("user:   %.6f s\n", usage.ru_utime.tv_sec+ usage.ru_utime.tv_usec/1000000.0);
            printf("system: %.6f s\n", usage.ru_stime.tv_sec+usage.ru_stime.tv_usec/1000000.0);
            sum_user+=usage.ru_utime.tv_sec+ usage.ru_utime.tv_usec/1000000.0;
            sum_system+=usage.ru_stime.tv_sec+usage.ru_stime.tv_usec/1000000.0;
            clock_gettime(CLOCK_REALTIME, &stop1);
            printf("real: %.6f s\n", ( stop1.tv_sec - start1.tv_sec )+ ( stop1.tv_nsec - start1.tv_nsec )/ 1000000000.0);
        }
    }
    clock_gettime(CLOCK_REALTIME, &stop);

    sum_user=sum_user/tflag;
    sum_system =sum_system/tflag;
    accum= ( stop.tv_sec - start.tv_sec )+ ( stop.tv_nsec - start.tv_nsec )/ 1000000000.0;
    sum_real=accum/tflag;
    printf("Sredni czas \n");
    printf("real_mean:   %.6f s\n", sum_real);
    printf("user_mean:   %.6f s\n", sum_user);
    printf("system_mean: %.6f s\n", sum_system);
}
int main(int argc, char *argv[]){
    int ret,vflag=0,tflag=1;
    
    while((ret =getopt(argc,argv,"+vt:"))!=-1){
        switch (ret){
            case 'v': vflag =1;break;
            case 't': 
                if(optarg==NULL){
                    fprintf(stderr,"Brak wartosci dla -t\n");
                    return 1;
                }
                for(int i=0;optarg[i]!= '\0';i++){
                    if (!isdigit(optarg[i])) {
                    fprintf(stderr, "Brak poprawnej wartości dla -t\n");
                    return 1;
                    }
                }
                
         
                tflag=atoi(optarg);
                break;
            case '?':
                fprintf(stderr,"Unknown option '-%c'.\n",optopt);return 1;
            default: abort();
        }
    }
    if (optind == argc){
        fprintf(stderr,"Brak programu do uruchomienia\n");
        return 1;
    }
    
    char **cmd = &argv[optind];
    get_time(cmd,tflag,vflag);

            
    
}
    
