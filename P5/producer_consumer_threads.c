/* You have to implement a producer-consumer model with two buffers, using three different 
types of threads. The program will continuously read input from the keyboard and process it 
through multiple threads before outputting the results to the console. */

/* A producer thread will 
    read integers from the keyboard, 
    convert each read value to an integer, 
    and store it in buffer_1. 
A consumer-producer thread will 
    read from buffer_1, 
    compute its Fibonacci number (the function is given below), 
    and store the result in  buffer_2.  
Finally,  a  consumer  thread  will  
    read  from  buffer_2  
    and  print the final computed values to the standard output. */


/* The  program  must  use  two  finite-size  buffers:  buffer_1,  which  stores  the  binary 
representations of integers, and buffer_2, which holds the computed Fibonacci numbers. 
Access to these buffers must be properly synchronized using mutexes and semaphores to 
prevent race conditions and avoid active waiting. Producers must wait when their buffer is 
full, and consumers must wait when their buffer is empty.  */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define SIZE 10
#define NUM_THREADS 2

int buffer_1[SIZE]; // binary integers
int buffer_2[SIZE]; // Fibbonaci Numbers

int in1 = 0; // Where we intert integer in buffer_1
int in2 = 0; // Where we intert integer in buffer_2
int out1 = 0; // Where we remove integer from buffer_1
int out2 = 0; // Where we remove integer from buffer_2
int count1 = 0; // Current elements in buffer_1
int count2 = 0; // Current elements in buffer_2

pthread_mutex_t lockProd; // Protects buffer_1
pthread_mutex_t lockConsProd; // Protects buffer_2
pthread_mutex_t lockCons; // buffer_2 accessed safely
sem_t full1; // Controls buffer_1
sem_t empty1; // Controls buffer_1
sem_t full2; // Controls buffer_2
sem_t empty2; // Controls buffer_2


void *thread_producer(void *arg){
    while(1){
        // Read integer from the keyboard
        int num = rand() % 10;
        //printf("Enter a number:\n");
        //scanf("%d", &num);

        // Convert value in integer
        int value = atoi(num);

        sem_wait(&empty1); // Wait if buffer 1 is full
        pthread_mutex_lock(&lockProd); // Lock buffer_1


        // Store in buffer 1
        buffer_1[in1] = value; 
        in1 = (in1 + 1) % SIZE; // Update

        count1++;
        
        pthread_mutex_unlock(&lockProd); // Unlock buffer_1
        sem_post(&full1); // Signal that buffer 1 is not empty
    }
    

}

void *thread_consumer_producer(void *arg){

    while(1){

        sem_wait(&full1); // Wait if buffer 1 is empty
        pthread_mutex_lock(&lockProd); // Lock buffer_1

        // Read from buffer_1
        int value = buffer_1[out1];
        out1 = (out1 + 1) % SIZE; // Update
        count1--;

        pthread_mutex_unlock(&lockProd); // Unlock buffer_1
        sem_post(&empty1); // Signal that buffer 1 has space

        // Fibonacci number
        int fibo = fib(value);

        sem_wait(&empty2); // Wait if buffer_2 is full
        pthread_mutex_lock(&lockConsProd); // Lock buffer_2

        // Store the result in  buffer_2
        buffer_2[in2] = fibo;
        in2 = (in2 + 1) % SIZE; // Update
        count2++;

        pthread_mutex_unlock(&lockConsProd); // Unlock buffer_2
        sem_post(&full2); // Signal that buffer_2 is not empty
    }
}

void *thread_consumer(void *arg){
    while(1){
        sem_wait(&full2); // Wait if buffer_2 is empty
        pthread_mutex_lock(&lockCons); // Lock buffer_2

        // Read  from  buffer_2
        int value = buffer_2[out2];
        out2 = (out2 + 1) % SIZE; // Update
        count2--;

        pthread_mutex_unlock(&lockCons); // Unlock buffer_2
        sem_post(&empty2); // Signal that buffer_2 has space

        // Print the final computed values to the standard output
        printf("Fibonacci: %d\n", value);
    }
    

}

int fib(int n){ 
    if (n == 0) 
        return 0; 
    else if (n == 1) 
        return 1; 
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

int main(int argc, char* argv[]){

    // Inicializamos el lock y semaforos
    pthread_mutex_init(&lockProd, NULL);
    pthread_mutex_init(&lockConsProd, NULL);
    pthread_mutex_init(&lockCons, NULL);
    sem_init(&full1, 0, 0);
    sem_init(&empty1, 0, SIZE);
    sem_init(&full2, 0, 0);
    sem_init(&empty2, 0, SIZE);
    
    pthread_t thread_prod[NUM_THREADS], thread_cons_prod[NUM_THREADS], thread_cons[NUM_THREADS];
    
    // thread_producer creation
    for (int i = 0; i < NUM_THREADS; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i + 1;
        pthread_create(&thread_prod[i], NULL, thread_producer, arg);
    }
    
    // thread_consumer_producer creation 
    for (int i = 0; i < NUM_THREADS; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i + 1;
        pthread_create(&thread_cons_prod[i], NULL, thread_consumer_producer, arg);
    }

    // thread_consumer creation
    for (int i = 0; i < NUM_THREADS; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i + 1;
        pthread_create(&thread_cons[i], NULL, thread_consumer, arg);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread_prod[i], NULL);
        pthread_join(thread_cons_prod[i], NULL);
        pthread_join(thread_cons[i], NULL);
    }
    

    // Clean up
    pthread_mutex_destroy(&lockProd);
    pthread_mutex_destroy(&lockConsProd);
    pthread_mutex_destroy(&lockCons);
    sem_destroy(&full1);
    sem_destroy(&empty1);
    sem_destroy(&full2);
    sem_destroy(&empty2);

    return 0;
}

/* FALTA: 
    - SINCRONIZACIÓN 
    - Semaforos
    - Buffer lleno --> producer no puede escribir y no haga active waiting
    - buffer 2 --> consumer no saque cosas si está vacio 
    */