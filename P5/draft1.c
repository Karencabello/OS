#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "semaphore.h"


#define SIZE 10
#define NUM_THREADS 6
#define TH_GROUP 2

int prodBuffer[SIZE];
int fibBuffer[SIZE];

int countBuff1 = 0;
int countBuff2 = 0;

pthread_mutex_t lockProd;
pthread_mutex_t lockCons1;
pthread_mutex_t lockCons2;
sem_t buff1Free;
sem_t buff1Filled;
sem_t buff2Free;
sem_t buff2Filled;
pthread_cond_t readingPaused;

int fib(int n) {
    if (n == 0){
        return 0;
    }
    else if (n == 1){
        return 1;
    }
    int fib_i_1 = 1;
    int fib_i_2 = 0;
    int fib_i = 1;
    for (int i = 1; i < n; ++i){
        int aux = fib_i_1 + fib_i_2;
        fib_i_2 = fib_i_1;
        fib_i_1 = fib_i;
        fib_i = aux;
    }
    return fib_i;
}

void* producer(void* arg) {
    while(1) {
        // add one value to prodBuffer
        int x = rand() % 10; 
        sem_wait(&buff1Free); 
        pthread_mutex_lock(&lockProd);
        prodBuffer[countBuff1] = x;
        countBuff1++;
        pthread_mutex_unlock(&lockProd);
        sem_post(&buff1Filled);
    }

}

void* consumer1(void* arg) {
    while(1) {
        // consume one value from prodBuffer
        sem_wait(&buff1Filled); // decrements
        pthread_mutex_lock(&lockCons1);
        int fib_num = fib(prodBuffer[countBuff1-1]);
        countBuff1--;
        sem_post(&buff1Free); // increments

        // add one value to fibBuffer
        sem_wait(&buff2Free); 
        fibBuffer[countBuff2] = fib_num;
        countBuff2++;
        pthread_mutex_unlock(&lockCons1);
        sem_post(&buff2Filled);
    }   
}

void* consumer2(void* arg) {
    // This function prints the fibonacci transformed array once it is full. 
    // We free the lock such that the next time it is filled, the other thread prints the array.
    while(1) {
        pthread_mutex_lock(&lockCons2);
        if(countBuff2 < SIZE) {
            pthread_cond_wait(&readingPaused, &lockCons2);
        }
        else{
            printf("Prod Buffer \n");
            for(int i=0; i<SIZE; i++){
                printf("%d ", prodBuffer[i]);
            }

            printf("\n");

            printf("Fib Buffer \n");
            for(int i=0; i<SIZE; i++){
                printf("%d ", fibBuffer[i]);
    
            }
            printf("\n");
            pthread_mutex_unlock(&lockCons2);
        }
    }
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    pthread_t thprod[NUM_THREADS], thcons1[NUM_THREADS], thcons2[NUM_THREADS];

    pthread_mutex_init(&lockProd, NULL);
    pthread_mutex_init(&lockCons1, NULL);
    pthread_mutex_init(&lockCons2, NULL);
    sem_init(&buff1Filled, 0, 0);
    sem_init(&buff1Free, 0, SIZE);
    sem_init(&buff2Filled, 0, 0);
    sem_init(&buff2Free, 0, SIZE);
    pthread_cond_init(&readingPaused, NULL);
    

    for(int i=0; i<TH_GROUP; i++){
        if(pthread_create(&thprod[i], NULL, &producer, NULL) != 0){
            perror("Failed at creating thread");
        }
        if(pthread_create(&thcons1[i], NULL, &consumer1, NULL) != 0){
            perror("Failed at creating thread");
        }
        if(pthread_create(&thcons2[i], NULL, &consumer2, NULL) != 0){
            perror("Failed at creating thread");
        }
    }

    for(int i=0; i<TH_GROUP; i++){
        if(pthread_join(thprod[i], NULL) != 0){
            perror("Failed at joining thread");
        }
        if(pthread_join(thcons1[i], NULL) != 0){
            perror("Failed at joining thread");
        }
        if(pthread_join(thcons2[i], NULL) != 0){
            perror("Failed at joining thread");
        }
    }
    pthread_mutex_destroy(&lockProd);
    pthread_mutex_destroy(&lockCons1);
    pthread_mutex_destroy(&lockCons2);
    sem_destroy(&buff1Filled);
    sem_destroy(&buff1Free);
    sem_destroy(&buff2Filled);
    sem_destroy(&buff2Free);
    pthread_cond_destroy(&readingPaused);

    return 0;
}
