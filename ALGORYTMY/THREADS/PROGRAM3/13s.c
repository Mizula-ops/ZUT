// SO IS1 211B LAB13
// Artur MIzula
// ma55831@zut.edu.pl 
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <io.h>
#include <sys/types.h>

 
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Error less arguments\n");
        return 0;
    }
    const char *file_n = argv[1];
    const char *name = argv[2];
    HANDLE File = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 101,name);
    if (File == NULL) 
    {
        fprintf(stderr, "Error nie mozna odtwozy");
    }
 
    char *shm_ptr = (char *)MapViewOfFile(File, FILE_MAP_ALL_ACCESS, 0, 0, 101);
    if (!shm_ptr) 
    {
        CloseHandle(File);
        fprintf(stderr, "Error");
    }
 
    int file = open(file_n, O_RDONLY | O_BINARY);
    if (file == -1) 
    {
        fprintf(stderr, "Error nie mozna odtworzyc pliku\n");
        UnmapViewOfFile(shm_ptr);
        CloseHandle(File);
        return 0;
    }
    memset(shm_ptr,0,100);
    shm_ptr[0] = 1;
    printf("Shared memory attached\n");
    printf("Waiting for receiver...\n");
    
    Sleep(1);
    while(1){
        while (shm_ptr[0] == 1){ 
        Sleep(1);  
    }
    int bRead = read(file, shm_ptr + 1, 100);
    if(bRead != 100){
        shm_ptr[0] = bRead;
         printf("%d byte(s) sent...\n", bRead);
         break;
    }
          printf("%d byte(s) sent...\n", bRead); 
          Sleep(1); 
        shm_ptr[0] = 1;
    }
    
    
    
    printf("Copying finished, receiver finished\n");
    UnmapViewOfFile(shm_ptr);
    CloseHandle(File);
    close(file);
    return 0;
}