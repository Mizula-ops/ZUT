//PS IS1 322 LAB02
//Artur Mizuła
//ma55831@zut.edu.pl
#include <sys/types.h>
#include <utmpx.h>
#include <grp.h>
#include <string.h>
#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void printLogUsers(int group){
    struct utmpx *entry = getutxent();
    while(entry != NULL){
        if(entry->ut_type == USER_PROCESS && group == 0){
            printf("%s\n", entry->ut_user);
        }
        else if(entry->ut_type == USER_PROCESS && group == 1){
            int ngroups = 0;
            gid_t *groups = returnGroups(entry->ut_user, &ngroups);
            printf("%s[", entry->ut_user);
            for(int i=0; i<ngroups; i++){
                struct group *gr = getgrgid(groups[i]);
                if(gr != NULL){
                    printf("%s", gr->gr_name);
                    if(i < ngroups-1)
                        printf(",");
                }
            }

            printf("]\n");
            free(groups);
        }
        entry = getutxent();
    }
}

int main(int argc, char *argv[]){
    int ret,gflag=0;
    while((ret =getopt(argc,argv,"g"))!=-1){
        switch (ret){
            case 'g': gflag =1;break;
            case '?':
                fprintf(stderr,"Unknown option '-%c'.\n",optopt);return 1;
            default: abort();
        }
    }
    if(gflag==1){
        printLogUsers(1);
    }
    else{
        printLogUsers(0);
    }
    

    return 0;
}