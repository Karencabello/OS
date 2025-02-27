#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

// We want to do 
// \sum_0^N i^2 

#define N 100

int res[N]; // We can use global variables to communicate with the son
int randomVector[N];


void* thread_function(void* arg) {
    int i = (int) arg;
    int j = randomVector[i];
    res[i] = j*j;
    return (void *) (i*i);
}

int main(int argc, char* argv[]) {
    // Define a random vector
    for (int i = 0; i < N; ++i)
        randomVector[i] = rand()%10;
        
    pthread_t tids[N];
    for (int i = 0; i < N; ++i)
        pthread_create(&tids[i], NULL, thread_function, (void *) i);
    
    int total = 0;
    for (int i = 0; i < N; ++i) {
        /* Option 1: Clean return
        void ** res;
        pthread_join(tids[i], (void **) &res);
        total += (int) res;
        */

       // Option 2: global variables
        pthread_join(tids[i], NULL); // we wait for the thread to finish, but we don't look at its return
        total += res[i];

    }
    printf("The sum is %d\n", total);

}