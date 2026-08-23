#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(){
    for (int i = 0; i < 500000; i++) {
        write(1, "x\n", 2);
    }
    return 0;
}