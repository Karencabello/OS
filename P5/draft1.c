#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#define SIZE 3
#define NUM_TH_PROD 5
#define NUM_TH_PC 1
#define NUM_TH_CONS 1


int prodBuffer[SIZE];
int fibBuffer[SIZE];

int countBuff1 = 0;
int countBuff2 = 0;

pthread_mutex_t lockBuff1;
pthread_mutex_t lockBuff2;
sem_t buff1Free;
sem_t buff1Filled;
sem_t buff2Free;
sem_t buff2Filled;

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
    for (int i = 2; i <= n; ++i) {
        fib_i = fib_i_1 + fib_i_2;
        fib_i_2 = fib_i_1;
        fib_i_1 = fib_i;
    }
    return fib_i;
}

void* producer(void* arg) {
    int th = *((int*)arg);
    free(arg);
    int x;
    char n[11];
    while(1) {
        sem_wait(&buff1Free); 
        scanf("%d", &x);
        /*
        scanf("%d", n);
        if(strcmp(n, "EXIT") == 0) {
            // hacer que termine el thread y todos los threads vivos
        }
        // si no, el código de antes
        else {
            x = atoi(n);
            pthread_mutex_lock(&lockBuff1);
            prodBuffer[countBuff1] = x;
            printf("PRODUCER (%d)--> Add original: %d; Position: %d\n", th, x, countBuff1);
            countBuff1++;
            pthread_mutex_unlock(&lockBuff1);
            sem_post(&buff1Filled);
        
        }
        */
        pthread_mutex_lock(&lockBuff1);
        prodBuffer[countBuff1] = x;
        printf("PRODUCER (%d)--> Add original: %d; Position: %d\n", th, x, countBuff1);
        countBuff1++;
        pthread_mutex_unlock(&lockBuff1);
        sem_post(&buff1Filled);
    }
}

void* cons_prod(void* arg) {
    int th = *((int*)arg);
    free(arg);
    while(1) {
        // consume one value from prodBuffer
        sem_wait(&buff1Filled); // decrements
        pthread_mutex_lock(&lockBuff1);
        int fib_num = fib(prodBuffer[countBuff1-1]);
        printf("CONS_PROD (%d) --> Transform fib: %d; Position: %d\n", th,fib_num, countBuff1-1);
        countBuff1--;
        pthread_mutex_unlock(&lockBuff1);
        sem_post(&buff1Free); // increments

        // add one value to fibBuffer
        sem_wait(&buff2Free); 
        pthread_mutex_lock(&lockBuff2);
        fibBuffer[countBuff2] = fib_num;
        printf("CONS_PROD (%d) --> Add fib: %d; Position; %d\n", *((int*)arg),fib_num, countBuff2);
        countBuff2++;
        pthread_mutex_unlock(&lockBuff2);
        sem_post(&buff2Filled);
    }
}

void* consumer(void* arg) {
    // This function prints the fibonacci transformed array once it is full. 
    // We free the lock such that the next time it is filled, the other thread prints the array.
    int th = *((int*)arg);
    free(arg);
    while(1) {
        // consume one value from fibBuffer
        sem_wait(&buff2Filled); // decrements
        pthread_mutex_lock(&lockBuff2);
        printf("CONSUMER (%d)--> Consume fib: %d; Position: %d\n", th, fibBuffer[countBuff2-1], countBuff2-1);
        countBuff2--;
        pthread_mutex_unlock(&lockBuff2);
        sem_post(&buff2Free); // increments
    }
}

int main(int argc, char* argv[]) {
    printf("When you are done adding production inputs enter: DONE \n");
    srand(time(NULL));
    pthread_t thprod[NUM_TH_PROD], thcons_prod[NUM_TH_PC], thcons[NUM_TH_CONS];

    pthread_mutex_init(&lockBuff1, NULL);
    pthread_mutex_init(&lockBuff2, NULL);
    sem_init(&buff1Filled, 0, 0);
    sem_init(&buff1Free, 0, SIZE);
    sem_init(&buff2Filled, 0, 0);
    sem_init(&buff2Free, 0, SIZE);

    // INITIALIZE 2 producers and 3 consumers
    for(int i=0; i<NUM_TH_PROD; i++){
        int* a = malloc(sizeof(int));
        *a = i;
        if(pthread_create(&thprod[i], NULL, &producer, a) != 0){
            perror("Failed at creating thread\n");
        }
    }
    for(int i=0; i<NUM_TH_PC; i++){
        int* b = malloc(sizeof(int));
        *b = i;
        if(pthread_create(&thcons_prod[i], NULL, &cons_prod, b) != 0){
            perror("Failed at creating thread\n");
        }
    }

    for(int i=0; i<NUM_TH_CONS; i++){
        int* c = malloc(sizeof(int));
        *c = i;

        if(pthread_create(&thcons[i], NULL, &consumer, c) != 0){
            perror("Failed at creating thread\n");
        }
    }
    for(int i=0; i<NUM_TH_PROD; i++){
        if(pthread_join(thprod[i], NULL) != 0){
            perror("Failed at creating thread\n");
        }
    }

   for(int i=0; i<NUM_TH_PC; i++){
        int* arg;
        if(pthread_join(thcons_prod[i], NULL) != 0){
            perror("Failed at creating thread\n");
        }
    }

    for(int i=0; i<NUM_TH_CONS; i++){
        if(pthread_join(thcons[i], NULL) != 0){
            perror("Failed at creating thread\n");
        }
    }

    pthread_mutex_destroy(&lockBuff1);
    pthread_mutex_destroy(&lockBuff2);
    sem_destroy(&buff1Filled);
    sem_destroy(&buff1Free);
    sem_destroy(&buff2Filled);
    sem_destroy(&buff2Free);

    return 0;
}
