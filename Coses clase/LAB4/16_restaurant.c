#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Make a
#define N 10

pthread_mutex_t lock;
pthread_cond_t isLast;
int counter = N;

void * restaurant(void * ){
    // We want that all threads eat the starter, before
    // any dish eats the main dish
    printf("Eating the starter\n");

    pthread_mutex_lock(&lock);
    counter--;
    while (counter != 0) {
        pthread_cond_wait(&isLast,&lock);
    }
    pthread_cond_broadcast(&isLast);
    //pthread_mutex_unlock(&lock);

    printf("Eating the main dish\n");

}

int main(){
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&isLast, NULL);

    pthread_t tid[N];
    for(int i =0; i < N; ++i)
        pthread_create(&tid[i], NULL, restaurant, NULL);

    for(int i =0; i < N; ++i)
        pthread_join(tid[i], NULL);
}