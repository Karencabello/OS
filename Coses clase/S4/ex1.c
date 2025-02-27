/* Create 100 threads such that all wait 1 second (using the function sleep provided in 
unistd.h) and the main thread waits for them to finish at the end. Remember that you can 
use pthread_join to wait for them. */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 100

void* thread(void* arg){
    sleep(1); 
    printf("Thread %ld finished\n", (long)arg );
    return NULL;
}
int main(int argc, char *argv[]){
    pthread_t threads[NUM_THREADS];
    int i;

    //Create threads
    for(i = 0; i <= NUM_THREADS; i++){
        pthread_create(&threads[i], NULL, thread, (void*)(long)i);
    }
    //Wait for threads to finish
    for(i = 0; i <= NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }
    printf("All threads have finished\n");
    return 0;

}