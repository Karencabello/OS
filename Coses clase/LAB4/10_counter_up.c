#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
/*
pthread_t t;
pthread_create(&t, NULL, thread_function, (void *) pData);
pthread_join(t, NULL | &p); // = wait
pthread_mutex_t lock;
pthread_mutex_init(&lock, NULL);
pthread_mutex_lock(&lock);
pthread_mutex_unlock(&lock);

Write a program that creates 3 threads, each decreases continuously a global variable counter.
They never stop
Thread 0 decreases by 1 
Thread 1 decreases by 2 
Thread 2 decreases by 3 
Thread 1 increases by 1. 
Beware that the variable is never negative
*/
int counter = 50;
pthread_mutex_t lock;

void * f(void * arg){
    int decrement = (int) arg;
    while(1) {
        pthread_mutex_lock(&lock);
        printf("Trying to decrease, counter = %d, decrement = %d\n", counter, decrement);
        if (counter - decrement >= 0) {
            counter -= decrement;
        }
        pthread_mutex_unlock(&lock);
    }
}

// Variation of th
void * increment(void * arg){
    while(1)
    {
        printf("Increment, counter = %d\n", counter);
        pthread_mutex_lock(&lock);
        counter += 1;
        pthread_mutex_unlock(&lock);
    }
}
int main(int argc, char *argv[]) {
    //a = 10;
    pthread_t tid[4];
    pthread_mutex_init(&lock, NULL);
    for (int i =0 ; i < 3; ++i)
        pthread_create(&tid[i], NULL, f, (void *) i +1);
    pthread_create(&tid[3], NULL, increment, NULL);

    for (int i =0; i< 4; ++i)
        pthread_join(tid[i], NULL);
    _exit(0);
}