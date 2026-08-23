//SO IS1 211B LAB11
//Artur Mizuła
//ma55831@zut.edu.pl
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#define MAX_LINES 370200
#define ALPHABET_SIZE 26

char** words;
int words_cnt;
int histogram[ALPHABET_SIZE];
HANDLE mutex;

typedef struct {
    int start;
    int end;
} ThreadArgs;


void* count_histogram(LPVOID arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    int start = args->start;
    int end = args->end;

    int local_histogram[ALPHABET_SIZE] = {0};

    for (int i = start; i <= end; i++) {
        char* word = words[i];
        for (int j = 0; word[j] != '\0'; j++) {
            if (word[j] >= 'a' && word[j] <= 'z') {
                local_histogram[word[j] - 'a']++;
            }
        }
    }

    WaitForSingleObject(mutex, INFINITE); 
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        histogram[i] += local_histogram[i];
    }
    ReleaseMutex(mutex); 

    printf("Thread #%lu stopped\n", GetCurrentThreadId());
    free(arg); 
    return 0;
}


void count() {
    int local_histogram[ALPHABET_SIZE] = {0};

    for (int i = 0; i < words_cnt; i++) {
        char* word = words[i];
        for (int j = 0; word[j] != '\0'; j++) {
            if (word[j] >= 'a' && word[j] <= 'z') {
                local_histogram[word[j] - 'a']++;
            }
        }
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        histogram[i] += local_histogram[i];
    }
}

int main(int argc, char* argv[]) {
    words = malloc(sizeof(char*) * MAX_LINES);
    FILE* f = fopen("words_alpha.txt", "r");
    assert(f != NULL);

    char line[255];
    for (words_cnt = 0; words_cnt < MAX_LINES; words_cnt++) {
        if (fgets(line, sizeof(line), f)) {
            words[words_cnt] = strdup(line);
        } else {
            break;
        }
    }
    fclose(f);

    printf("Lines in file: %d\n", words_cnt);

    memset(histogram, 0, sizeof(histogram));
    mutex = CreateMutex(NULL, FALSE, NULL);

    int num_threads = 0;
    if (argc > 1) {
        num_threads = atoi(argv[1]);
        if (num_threads < 0 || num_threads > 16) {
            printf("Invalid number of threads. Using 0 threads.\n");
            num_threads = 0;
        }
    }

    clock_t start_time = clock();

    if (num_threads > 0) {
        HANDLE threads[num_threads];
        int words_per_thread = words_cnt / num_threads;
        int extra_words = words_cnt % num_threads;

        for (int i = 0; i < num_threads; i++) {
            int start = i * words_per_thread;
            int end = start + words_per_thread - 1;
            if (i == num_threads - 1) {
                end += extra_words;
            }

            ThreadArgs* args = malloc(sizeof(ThreadArgs));
            args->start = start;
            args->end = end;

            printf("Thread #%d started start=%d, end=%d\n", i + 1, start, end);
            threads[i] = CreateThread(NULL, 0, count_histogram, args, 0, NULL);
        }

        for (int i = 0; i < num_threads; i++) {
            WaitForSingleObject(threads[i], INFINITE);
            CloseHandle(threads[i]); 
        }
    } else {
        count();  
    }

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %f s\n", elapsed_time);

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        printf("%c : %d\t", 'a' + i, histogram[i]);
        if ((i + 1) % 5 == 0) {
            printf("\n");
        }
    }

    for (int i = 0; i < words_cnt; i++) {
        free(words[i]);
    }
    free(words);
    CloseHandle(mutex); 

    return 0;
}
