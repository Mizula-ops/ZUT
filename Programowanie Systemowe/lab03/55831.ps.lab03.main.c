//PS IS1 322 LAB03
//Artur Mizuła
//ma55831@zut.edu.pl
#include "lib.h"
#include <dlfcn.h>
#include <stdio.h>       // printf, fprintf
#include <string.h>      // strerror
#include <stdint.h>      // uint32_t
#include <systemd/sd-bus.h>
#include <bits/getopt_core.h>
#include <stdlib.h>
#include <grp.h>
int printLogUsers(int gflag,int uflag,void *handle,gid_t *(*Return)(const char *,int *)){

    sd_bus *bus = NULL;
	sd_bus_message *msg = NULL;
	int r;
    r = sd_bus_open_system(&bus);
	if (r < 0) {
		fprintf(stderr, "Błąd połączenia z system bus: %s\n", strerror(-r));
		return 1;
	}
    r = sd_bus_call_method(bus,
	"org.freedesktop.login1",
	"/org/freedesktop/login1",
	"org.freedesktop.login1.Manager",
	"ListSessions",
	NULL,
	&msg,
	"");
	if (r < 0) {
		fprintf(stderr, "Błąd wywołania ListSessions: %s\n", strerror(-r));
		sd_bus_unref(bus);
		return 1;
	}
    r = sd_bus_message_enter_container(msg, SD_BUS_TYPE_ARRAY, "(susso)");
	if (r < 0) {
		fprintf(stderr, "Błąd parsowania odpowiedzi: %s\n", strerror(-r));
		sd_bus_message_unref(msg);
		sd_bus_unref(bus);
		return 1;
	}
    while ((r = sd_bus_message_enter_container(msg, SD_BUS_TYPE_STRUCT, "susso")) > 0) {
	const char *session_id, *user_name;
    uint32_t uid;
    sd_bus_message_read_basic(msg, SD_BUS_TYPE_STRING,  &session_id);
    sd_bus_message_read_basic(msg, SD_BUS_TYPE_UINT32,  &uid);
    sd_bus_message_read_basic(msg, SD_BUS_TYPE_STRING,  &user_name);	
    sd_bus_message_skip(msg, "so");
    
    printf("%s ",user_name);
    
	if(uflag==1)
        printf("(%u) ",uid);
    if(gflag==1){
        int ngroups = 0;
        gid_t *groups = Return(user_name, &ngroups);
        printf("[");
        for(int i=0; i<ngroups; i++){
            struct group *gr = getgrgid(groups[i]);
            if(gr != NULL){
                printf("%s", gr->gr_name);
                if(i < ngroups-1)
                    printf(",");
                
            }
            printf("]");
            free(groups);
        }

    }
    printf("\n");
    sd_bus_message_exit_container(msg);
	}
    sd_bus_message_exit_container(msg);
    sd_bus_message_unref(msg);
	sd_bus_unref(bus);
	return 0;

}

int main(int argc, char *argv[]){
    int ret,gflag,uflag=0;
    while((ret =getopt(argc,argv,"gi"))!=-1){
        switch (ret){
            case 'g': gflag =1;break;
            case 'i': uflag=1;break;
            case '?':
                fprintf(stderr,"Unknown option '-%c'.\n",optopt);return 1;
            default: abort();
        }
    }
    if(gflag==1){
        void *handle = dlopen("./libgroup.so",RTLD_LAZY);
        if(!handle){
            printf("Brak biblioteki - zostana pominiete grupy\n");
            gflag=0;
            printLogUsers(gflag,uflag,NULL,NULL);
        }
        else{
            gid_t *(*Return)(const char *,int *);
            Return=dlsym(handle,"returnGroups");
            if(Return==NULL){
                printf("Brak biblioteki - zostana pominiete grupy\n");
                gflag=0;
            }
            printLogUsers(gflag,uflag,handle,Return);
            dlclose(handle);
        }
        
    }
    else{
        printLogUsers(gflag,uflag,NULL,NULL);
    }
 
    return 0;
}