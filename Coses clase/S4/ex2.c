/* Set a global array A[100] to the square of its index, meaning A[i] = i*i, using a 
thread per cell. 
a.  To do so, pass the address of the index to the thread function. What’s the result? 
What has happened? 
b.  Now, pass a pointer to a copy of the index value. To do so, you can allocate the 
required space using malloc, and then copy the value of the index into the 
pointer. Remember to free the pointer. What’s the result? What has happened? */



#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 100

int A[NUM_THREADS]; //Global array 

void* thread(void* arg){
    int i = *(int*)arg; //Dereference the pointer to get index
    free(arg);
    A[i] = i*i; //Assign square of index to A[i]
    return NULL;
}
int main(int argc, char *argv[]){
    pthread_t threads[NUM_THREADS];
    int j;

    //Create threads
    for(j = 0; j <= NUM_THREADS; j++){
        int* index = malloc(sizeof(int)); //Allocate memory for index copy 
        *index = j; //copy value in allocated space
        pthread_create( &threads[j], NULL, thread, index);
    }
    
    //wait threads to finish
    for(j = 0; j <= NUM_THREADS; j++){
        pthread_join(threads[j], NULL);
    }

    //Print results
    for(j = 0; j <= NUM_THREADS; j++){
        printf("A[%d] = %d\n", j, A[j]);
    }

    printf("All threads have finished\n");
    return 0;
}