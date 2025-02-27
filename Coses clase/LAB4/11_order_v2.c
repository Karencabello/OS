#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Make 2 threads, assure that threadA executes before threadB
// We want the entree before the main dish 
pthread_mutex_t lock;
int entree = 0;

void * thread_A(void *A) {
    printf("Hi, I am the entree\n");
    entree = 1;
}

void * thread_B(void *A) {
    while (entree == 0);
    printf("Hi, I am the main dish\n");
}
int main(int argc, char * argv[]) {
   
    pthread_t t1, t2;

    pthread_create(&t1, NULL, thread_B, NULL);
    sleep(1); // Just to wait
    pthread_create(&t2, NULL, thread_A, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}