//PS IS1 322 LAB07
//Artur Mizuła
//ma55831@zut.edu.pl
#define _GNU_SOURCE  
#include <unistd.h>     
#include <crypt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/mman.h>
#include <time.h>
typedef struct {
    size_t start;
    size_t end;
}Cracker;

char *buff;
char *hash; 
char *salt;
int found =0;
pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER;
int percent=0;
int check_words=0;
int total_words=0;
pthread_mutex_t mutex_words= PTHREAD_MUTEX_INITIALIZER;
void *worker(void *arg) {

    Cracker *d = (Cracker *)arg;
    
    struct crypt_data cdata;
    cdata.initialized = 0;

    size_t i = d->start;
    
    while (i < d->end) {
        pthread_mutex_lock(&mutex);
            if(found){
                pthread_mutex_unlock(&mutex);
                return NULL;
            }
        pthread_mutex_unlock(&mutex);

        char *start = &buff[i];
        size_t len=0;
        while (i < d->end && buff[i] != '\n') {
            i++;
            len++;
        }
        i++;
        
        if (len == 0) {
            continue;
        }
        char *word=malloc(len+1);
        memcpy(word,start,len);
        word[len] = '\0';

        pthread_mutex_lock(&mutex_words);
            check_words++;
            if (check_words%1000==0 ){
                double new_percent = ((double)check_words * 100.0) / total_words;
                printf("Postep: %.4f\n", new_percent);
            }
        pthread_mutex_unlock(&mutex_words);

        char *res = crypt_r(word, salt, &cdata);
        if (res != NULL && strcmp(res, hash) == 0) {
            pthread_mutex_lock(&mutex);
            found=1;
            printf("Znalezione haslo: %s\n", word);
            pthread_mutex_unlock(&mutex);
            free(word);
            return NULL;
        }
        free(word);
    }
    return NULL;
}
void *benchmark(void *arg) {

    Cracker *d = (Cracker *)arg;
    
    struct crypt_data cdata;
    cdata.initialized = 0;

    size_t i = d->start;
    
    while (i < d->end) {
        char *start = &buff[i];
        size_t len=0;
        while (i < d->end && buff[i] != '\n') {
            i++;
            len++;
        }
        i++;
        
        if (len == 0) {
            continue;
        }
        char *word=malloc(len+1);
        memcpy(word,start,len);
        word[len] = '\0';

        char *res = crypt_r(word, salt, &cdata);
        free(word);
    }
    return NULL;
}
int main(int argc, char *argv[]){
    if(argc != 3 && argc !=4){
        fprintf(stderr,"Bład za mala liczba argumentow");
        return 1;
    }
    
    hash=argv[1];
    int p1=open(argv[2],O_RDONLY);
    if(p1 < 0){
        fprintf(stderr,"Nie mozna otworzyc pliku");
        return 1;
    }
    struct stat st;
    fstat(p1, &st);
    size_t size = st.st_size;
    buff = mmap(NULL, size, PROT_READ, MAP_PRIVATE, p1, 0);
    total_words = 0;
    if (argc==4){
        int threads = atoi(argv[3]);
        

        for (size_t i = 0; i < size; i++) {
            if (buff[i] == '\n') {
                total_words++;
            }
        }

        int dollars = 0;
        int k=0;
        while(hash[k] != '\0'){
            if(hash[k]=='$'){
                dollars++;
                if(dollars ==3){
                    k++;
                    break;
                }
            }
            k++;
        }
        salt=malloc(k+1);
        for(int i=0;i<k;i++){
            salt[i]=hash[i];
        }
        salt[k]='\0';

        long cpu = sysconf(_SC_NPROCESSORS_ONLN);
        if (threads > cpu) {
        threads = cpu;
        }
        pthread_t thread[threads];
        Cracker data[threads];
        for (int i=0;i<threads;i++){
            size_t start = i*size/threads;
            size_t end = (i+1) *size/threads;

            if(i!=0){
            while (start < size && buff[start - 1] != '\n') 
                start++;
            }
            if(i!=threads-1){
            while (end < size && buff[end] != '\n') 
                end++;
            }
            data[i].start=start;
            data[i].end=end;
            pthread_create(&thread[i], NULL, worker, &data[i]);
        }

        for(int i=0;i<threads;i++){
            pthread_join(thread[i],NULL);
        }
        if(!found){
            printf("Nie znaleziono hasla\n");
        }
        
    }
    else{
        int part_buff=0;
     
        while(part_buff < size && total_words!=1000) {
            if (buff[part_buff++] == '\n') {
                total_words++;
            }
        }
    
    
        int dollars = 0;
        int k=0;
        while(hash[k] != '\0'){
            if(hash[k]=='$'){
                dollars++;
                if(dollars ==3){
                    k++;
                    break;
                }
            }
            k++;
        }
        salt=malloc(k+1);
        for(int i=0;i<k;i++){
            salt[i]=hash[i];
        }
        salt[k]='\0';

        long cpu = sysconf(_SC_NPROCESSORS_ONLN);
        for(int threads=1;threads<=cpu;threads++){
            pthread_t thread[threads];
            Cracker data[threads];
            struct timespec start_time, end_time;

            clock_gettime(CLOCK_MONOTONIC, &start_time);
            for (int i=0;i<threads;i++){
               
                size_t start = i*part_buff/threads;
                size_t end = (i+1) *part_buff/threads;

                if(i!=0){
                while (start < size && buff[start - 1] != '\n') 
                    start++;
                }
                if(i!=threads-1){
                while (end < part_buff && buff[end] != '\n') 
                    end++;
                }
                data[i].start=start;
                data[i].end=end;
                pthread_create(&thread[i], NULL, benchmark, &data[i]);
            }
            for(int i=0;i<threads;i++){
            pthread_join(thread[i],NULL);
            }
            clock_gettime(CLOCK_MONOTONIC, &end_time);
            long diff_ms = (end_time.tv_sec - start_time.tv_sec) * 1000L
             + (end_time.tv_nsec - start_time.tv_nsec) / 1000000L;
            printf("Dla %d watkow czas to: %ld ms\n", threads, diff_ms);
            }
    }
    munmap(buff,size);
    close(p1);
    free(salt);
    return 0;

}