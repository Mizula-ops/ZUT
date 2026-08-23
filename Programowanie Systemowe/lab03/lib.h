
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>

gid_t *returnGroups(const char *username, int *ngroups);
