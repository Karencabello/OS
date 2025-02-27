#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

// We want to do 
// v[i] = a[i] + b[i]
// Each thread will do a single operation

#define N 100

int v[N]; // We can use global variables to communicate with the son
int a[N];
int b[N];

void* thread_function(void* arg) {
    int i = (int) arg;
    v[i] = a[i] + b[i];
}

int main(int argc, char* argv[]) {
    // Define a random vector
    for (int i = 0; i < N; ++i) {
        a[i] = rand()%10;
        b[i] = rand()%10;
    }
        
    pthread_t tids[N];
    for (int i = 0; i < N; ++i)
        pthread_create(&tids[i], NULL, thread_function, (void *) i);
    
    for (int i = 0; i < N; ++i) {
        pthread_join(tids[i], NULL); // we wait for the thread to finish, but we don't look at its return
    }
    // Print the array
    for (int i =0; i < N; ++i) {
        printf("%d \n", v[i]);
    }
}