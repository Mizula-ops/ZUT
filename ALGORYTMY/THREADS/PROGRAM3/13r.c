// SO IS1 211B LAB13
// Artur MIzula
// ma55831@zut.edu.pl 
#include <windows.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>
 int main(int argc, char *argv[]) {
    if (argc != 3) 
    {
        fprintf(stderr, "Error za mala ilosc argumentow\n");
        return 0;
    }
 
    const char *file_n = argv[1];
    const char *name = argv[2];
 
    HANDLE File = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, name);
    if (File==NULL) 
    {
        fprintf(stderr, "Error nie mozna odtwozy");
    }
 
    char *sharedMemory = (char *)MapViewOfFile(File, FILE_MAP_ALL_ACCESS, 0, 0, 101);
    if (!sharedMemory) 
    {
        CloseHandle(File);
        fprintf(stderr, "Error");
    }
 
    int file = open(file_n, O_CREAT | O_BINARY|O_WRONLY| S_IWRITE | S_IREAD);
    if (file == -1) 
    {
       fprintf(stderr, "Error nie mozna odtworzyc pliku\n");
        UnmapViewOfFile(sharedMemory);
        CloseHandle(File);
        return 0;
    }
 
    printf("Shared memory attached, ready to receive\n");
    sharedMemory[0] = 0;
    while (1) 
    {
       
        if(sharedMemory[0] == 1) 
        {  
            write(file, sharedMemory + 1,100); 
            printf("%d byte(s) received...\n", 100);
            sharedMemory[0] = 0; 
            Sleep(1);
        }
        else if(sharedMemory[0] !=0 && sharedMemory[0] !=1){
            write(file, sharedMemory + 1,sharedMemory[0] ); 
            printf("%d byte(s) received...\n",sharedMemory[0] );
            break;
        }
    }

    printf("Copying finished\n");
 
    
    UnmapViewOfFile(sharedMemory);
    CloseHandle(File);
     close(file);
    return 0;
} 