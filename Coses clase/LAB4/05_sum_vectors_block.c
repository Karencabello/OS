#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

// Now, we We want to do 
// v[i] = a[i] + b[i]
// Each thread will do 10 operations. 
// N is the number of elements of the array

#define N 100

int v[N]; // We can use global variables to communicate with the son
int a[N];
int b[N];

typedef struct {
    int low_index;
    int high_index;
} FunctionArguments;

void* thread_function(void* arg) {
    FunctionArguments* n = (FunctionArguments* ) arg;
    for (int i = n->low_index; i < n->high_index; ++i) {
        v[i] = a[i] + b[i];
    }
    free(n); // Now we know it will not be used again
}

int main(int argc, char* argv[]) {
    // Define a random vector
    for (int i = 0; i < N; ++i) {
        a[i] = rand()%10;
        b[i] = rand()%10;
    }
    int nThreads = N/10;
        
    pthread_t tids[N];
    for (int i = 0; i < nThreads; ++i) {
        // If we use a local variable, f might stop existing before the thread reads it!
        FunctionArguments *f = malloc (sizeof(FunctionArguments)); 
        f->low_index = 10 *i;
        f->high_index = 10 * (i + 1);
        pthread_create(&tids[i], NULL, thread_function, (void *) f);
    }
    for (int i = 0; i < nThreads; ++i) {
        pthread_join(tids[i], NULL); // we wait for the thread to finish, but we don't look at its return
    }
    // Print the array
    for (int i =0; i < N; ++i) {
        printf("%d, should be %d \n", v[i], a[i] + b[i]);
    }
}