/* You have to implement a producer-consumer model with two buffers, using three different 
types of threads. The program will continuously read input from the keyboard and process it 
through multiple threads before outputting the results to the console. */

/* A producer thread will 
    read space-separated integers from the keyboard, 
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
int num = 0;

pthread_mutex_t lock;
//sem_t


void *thread_producer(void *arg){

    // Read integer from the keyboard
    int num = rand() % 10;


    // Convert value in integer
    int value = atoi(num);


    // Store in buffer 1
    buffer_1[num] = value;
    

num++;

}

void *thread_consumer_producer(void *arg){

    // Read from buffer_1

    // Fibonacci number

    // Store the result in  buffer_2

}

void *thread_consumer(void *arg){

    // Read  from  buffer_2 

    // Print the final computed values to the standard output

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

    // Inicializamos el lock y las dos condiciones
    pthread_mutex_init(&lock, NULL);
    //pthread_cond_init(&condA, NULL);
    //pthread_cond_init(&condB, NULL);
    
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
    pthread_mutex_destroy(&lock);
    //pthread_cond_destroy(&condA);
    //pthread_cond_destroy(&condB);
    
    return 0;

}

/* FALTA: 
    - SINCRONIZACIÓN 
    - Semaforos
    - Buffer lleno --> producer no puede escribir y no haga active waiting
    - buffer 2 --> consumer no saque cosas si está vacio 
    */