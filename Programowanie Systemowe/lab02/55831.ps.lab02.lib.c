//PS IS1 322 LAB02
//Artur Mizuła
//ma55831@zut.edu.pl
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <stdlib.h>
#include "lib.h"
gid_t *returnGroups(char *username, int *ngroups){
    struct passwd *pw = getpwnam(username);
    *ngroups = 0;
    getgrouplist(username, pw->pw_gid, NULL, ngroups);
    gid_t *groups = malloc((*ngroups) * sizeof(gid_t));
    getgrouplist(username, pw->pw_gid, groups, ngroups);
    return groups;
}