// IMPORTANT: in class it was commmented that the user introduced one number each time, and that each thread processed one number at a time.
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <signal.h>

#define SIZE 3
#define NUM_TH_PROD 5
#define NUM_TH_PC 2
#define NUM_TH_CONS 2


int prodBuffer[SIZE];
int fibBuffer[SIZE];

pthread_t thprod[NUM_TH_PROD], thcons_prod[NUM_TH_PC], thcons[NUM_TH_CONS];

int countBuff1 = 0;
int countBuff2 = 0;

pthread_mutex_t lockBuff1;
pthread_mutex_t lockBuff2;
sem_t buff1Free;
sem_t buff1Filled;
sem_t buff2Free;
sem_t buff2Filled;

void handle_exit_signal(int sig) {
    
    if (sig == SIGTERM) {
        printf("Thread terminating.\n");
    }

    pthread_exit(NULL);
}

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
    signal(SIGTERM, handle_exit_signal); // Handle termination in main thread
    int th = *((int*)arg);
    free(arg);
    printf("producer (%d) created\n", th);
    int x;
    char n[16];
    while(1) {

        sem_wait(&buff1Free); // Espera espacio en buffer_1
        scanf("%s", n); // Lee entrada dek usuario
        if(strcmp(n, "EXIT") == 0) { // Usuario escribe EXIT, terminar
            printf("Thread terminating.\n");
            for (int i = 0; i < NUM_TH_PROD; i++) {
                if(i != th) {
                    pthread_kill(thprod[i], SIGTERM);
                }
            }
            for (int i = 0; i < NUM_TH_PC; i++) {
                pthread_kill(thcons_prod[i], SIGTERM);
            }
            for (int i = 0; i < NUM_TH_CONS; i++) {
                pthread_kill(thcons[i], SIGTERM);
            }
            pthread_exit(NULL);
        }
        x = atoi(n); // convierte entrada en int
        pthread_mutex_lock(&lockBuff1); // Lock buffer_1
        prodBuffer[countBuff1++] = x; // Guardamos en el buffer
        printf("PRODUCER (%d)--> Add original: %d; Position: %d\n", th, x, countBuff1 - 1);
        pthread_mutex_unlock(&lockBuff1); // Unlock buffer
        sem_post(&buff1Filled); // Buffer_1 is not empty 
    }
}

void* cons_prod(void* arg) {
    int th = *((int*)arg);
    free(arg);
    printf("consumer_producer (%d) created\n", th);
    while(1) {
        
        // consume one value from prodBuffer
        sem_wait(&buff1Filled); // Espera a que haya un valor en buffer_1
        pthread_mutex_lock(&lockBuff1); // Bloquea buffer_1
        int value = prodBuffer[--countBuff1]; // Obtiene valor en posición del buffer_1
        //printf("CONS_PROD (%d) --> Transform fib: %d; Position: %d\n", th,fib_num, countBuff1-1);
        pthread_mutex_unlock(&lockBuff1); // Desbloquea buffer_1
        sem_post(&buff1Free); // Buffer_1 empty

        // add one value to fibBuffer
        int fib_num = fib(value); // calcula fibonacci 
        sem_wait(&buff2Free); // Espera espacio en el buffer 2
        pthread_mutex_lock(&lockBuff2); // Bloquea buffer_2
        fibBuffer[countBuff2++] = fib_num; // Almacena fibonacci en buffer_2
        printf("CONS_PROD (%d) --> Add fib: %d; Position; %d\n",th ,fib_num, countBuff2 - 1);
        pthread_mutex_unlock(&lockBuff2); // Desbloquea buffer_2
        sem_post(&buff2Filled); // Buffer_2 not empty
    }
}

void* consumer(void* arg) {
    // This function prints the fibonacci transformed array once it is full. 
    // We free the lock such that the next time it is filled, the other thread prints the array.
    int th = *((int*)arg);
    free(arg);
    printf("consumer (%d) created\n", th);
    while(1) {
        // consume one value from fibBuffer
        sem_wait(&buff2Filled); // decrements
        pthread_mutex_lock(&lockBuff2); //Lock buffer_2
        int value = fibBuffer[--countBuff2]; // Obtiene valor en posición del buffer
        printf("CONSUMER (%d)--> Consume fib: %d; Position: %d\n", th, value, countBuff2);
        printf("FIBONACCI: %d\n", value);
        pthread_mutex_unlock(&lockBuff2);
        sem_post(&buff2Free); // increments
    }
}

int main(int argc, char* argv[]) {
    printf("When you are done adding production inputs enter: EXIT \n"); //Cambiar done por exit
    srand(time(NULL));
    //pthread_t thprod[NUM_TH_PROD], thcons_prod[NUM_TH_PC], thcons[NUM_TH_CONS];

    // Inicializamos el lock y semaforos
    pthread_mutex_init(&lockBuff1, NULL);
    pthread_mutex_init(&lockBuff2, NULL);
    sem_init(&buff1Filled, 0, 0);
    sem_init(&buff1Free, 0, SIZE);
    sem_init(&buff2Filled, 0, 0);
    sem_init(&buff2Free, 0, SIZE);

    // INITIALIZE agents
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

    // Clean up
    pthread_mutex_destroy(&lockBuff1);
    pthread_mutex_destroy(&lockBuff2);
    sem_destroy(&buff1Filled);
    sem_destroy(&buff1Free);
    sem_destroy(&buff2Filled);
    sem_destroy(&buff2Free);

    return 0;
}
