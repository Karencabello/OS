#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <semaphore.h>

#define N 100 // size of the vector

double *u, *u_norm; // input and output of the vector
double u_norm_2 = 0; // Shared value for squared nornm
pthread_mutex_t lock; // Mutex to safe update u_norm_2
sem_t barrier; // Semaphore to wait for the end of the computation

// Thread function

void* normaliseVectorThread(void* arg){
    int i = *(int*)arg;
    free(arg);

    // Step 1: Squared norm
    pthread_mutex_lock(&lock);
    u_norm_2 += u[i] * u[i];
    pthread_mutex_unlock(&lock);

    pthread_barrier_wait(&barrier);

    // Step 2: Normalise vector
    u_norm[i] = u[i] / sqrt(u_norm_2);
    return NULL;
}

int main(){
    pthread_t threads[N];

    // Allocate memory for vectors
    u = malloc(N * sizeof(double));
    u_norm = malloc(N * sizeof(double));

    // Initialize vector 
    for(int i = 0; i < N; i++){
        u[i] = (double) (rand() % 10 + 1);
    }

    // Initialize mutex and barrier
    pthread_mutex_init(&lock, NULL);
    pthread_barrier_init(&barrier, NULL, N);

    // Create threads
    for(int i = 0; i < N; i++){
        int* index = malloc(sizeof(int)); // Allocate memory to store thread index
        *index = i; // Store thread index
        pthread_create(&threads[i], NULL, normaliseVectorThread, index);
    }

    // Wait for threads to finish 
    for(int i = 0; i < N; i++){
        pthread_join(threads[i], NULL);
    }

    // Clean
    pthread_mutex_destroy(&lock);
    pthread_barrier_destroy(&barrier);
    free(u);
    free(u_norm);
    return 0;
}
