//PS IS1 322 LAB07
//Artur Mizuła
//ma55831@zut.edu.pl

#include <unistd.h>

#define _GNU_SOURCE       
#include <crypt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[]){
    if(argc !=3){
        fprintf(stderr,"Bład za mala liczba argumentow");
        return 1;
    }
   char *full_salt = malloc(strlen(argv[2]) + 5);

    sprintf(full_salt, "$6$%s$", argv[2]);
    struct crypt_data data;
    data.initialized = 0;

    char *hash = crypt_r(argv[1], full_salt, &data);

    printf("%s\n", hash);
    free(full_salt);
    return 0;
}

