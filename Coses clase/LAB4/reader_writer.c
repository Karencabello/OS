#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

/*
Reader / writer

*/

#define N_WRITERS 3
#define N_READERS  10
#define N_DATA_BLOCKS 100
int data[N_DATA_BLOCKS];
pthread_mutex_t lock;
pthread_cond_t cond;

int write_in_progress = 0; // Is 1 if there is a write operation in process
int read_in_progress = 0; 

void * reader(void * n) {
    while(1) {
        int block = rand()%N_DATA_BLOCKS;
        // Check if the data is being modified
        pthread_mutex_lock(&lock);
        if (write_in_progress == 0) { // Need to protect
            read_in_progress++;       // Need to protect global variable
            pthread_mutex_unlock(&lock);
            int value = data[block];     // NO need to prorext
            printf("Reader reads %d\n", value);
            usleep(10000);   
            pthread_mutex_lock(&lock);
            read_in_progress--; // Need to protect, global variable
        }
        pthread_mutex_unlock(&lock);

    }
}
void * writer(void * n) {
    int i = (int) n;
    while(1) {
        int block = rand()%N_DATA_BLOCKS;
        pthread_mutex_lock(&lock);
        if (read_in_progress == 0 && write_in_progress == 0) {
            write_in_progress = 1;
            data[block]++;
            write_in_progress = 0;
        }
        pthread_mutex_unlock(&lock);

    }
}

int main(int argc, char * argv[]) {
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_t tid_writer[N_WRITERS];
    for (int i = 0; i < N_WRITERS; i++) {
        pthread_create(&tid_writer[i], NULL, writer, (void *) i);
    }
    pthread_t tid_reader[N_READERS];
    for (int i = 0; i < N_READERS; i++) {
        pthread_create(&tid_reader[i], NULL, reader, (void *) i);
    }

    for (int i = 0; i < N_WRITERS; i++)
        pthread_join(tid_writer[i], NULL);
    for (int i = 0; i < N_READERS; i++)
        pthread_join(tid_reader[i], NULL);
}