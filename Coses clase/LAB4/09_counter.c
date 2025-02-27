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
Thread 0 decreases by 1 
Thread 1 decreases by 2 
Thread 2 decreases by 3 
Beware that the variable is never negative
*/
int counter = 50;
pthread_mutex_t lock;

void * f(void * arg){
    int decrement = (int) arg;
    while(counter - decrement >= 0) {
        pthread_mutex_lock(&lock);
        printf("Trying to decrease, counter = %d, decrement = %d\n", counter, decrement);
        if (counter - decrement >= 0) {
            counter -= decrement;
        }
        pthread_mutex_unlock(&lock);
    }

}

int main(int argc, char *argv[]) {
    //a = 10;
    pthread_t tid[3];
    pthread_mutex_init(&lock, NULL);
    for (int i =0 ; i < 3; ++i)
        pthread_create(&tid[i], NULL, f, (void *) i +1);

    for (int i =0; i< 3; ++i)
        pthread_join(tid[i], NULL);
    _exit(0);
}