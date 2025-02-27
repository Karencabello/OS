#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define NUM_THREADS 3
//VICTORIA!!!!
int counter;
int turn = 1; // 1 --> A ; 0 --> B
int fd;
pthread_mutex_t lock;
pthread_cond_t condA;
pthread_cond_t condB;

//Funcion del jugador A, le enviamos los 3 threads y el que llegue primero, pasará y los otros se cerraran.
void * thread_player_A(void * arg) {
    // Sleep para que acaben de hacerse
    usleep(10000);
    // Miramos qué hay que restar
    int* decrementp = (int*)arg;
    int decrement = (*decrementp);
    
    while(counter > 0) {
        //cabiar condicion
        pthread_mutex_lock(&lock);
        
        while(turn == 0){ // si no es el turno de A, que sus threads descansen
            pthread_cond_wait(&condA, &lock);
        }

        if (turn == 1 && counter >= decrement) { // si es el turno de A, que reste lo que le toca
            printf("Player A decreasing %d \n", decrement);
            counter -= decrement;
            lseek(fd, 0, SEEK_SET);
            write(fd, &counter, sizeof(int)); //Update game.dat
            printf("Counter = %d\n", counter);
            turn = 0; // Cambiamos turno --> Important to make it BEFORE the signal 
        }
        pthread_mutex_unlock(&lock); // liberamos el lock
        //pthread_cond_broadcast(&condB); // despertamos los otros threads
        pthread_cond_signal(&condB); 
    }
    free(arg);
    //REVISAR CONDITION VARIABLES
        //condicion para cada jugador 
        //broadcast en condicion del contrario --> hacer signal mejor pq solo despierta a uno 
    
}

//Funcion del jugador B, le enviamos los 3 threads y el que llegue primero, pasará y los otros se cerraran.
// Can also be done by sending which player is on the struct, which is its cond, and what is the other cond.
void * thread_player_B(void * arg) {// Write here the code to obtain the specific decremement of each thread
    // Sleep para que acaben de hacerse
    usleep(10000);
    // Miramos qué hay que restar
    int* decrementp = (int*)arg;
    int decrement = (*decrementp);
    
    while(counter > 0) {

        pthread_mutex_lock(&lock);
        
        while(turn == 1){ // si no es el turno de B, que sus threads descansen
            pthread_cond_wait(&condB, &lock);
        }
        
        //que duerman los threads de B que no estan lock

        if (turn == 0 && counter >= decrement) { // si es el turno de B, que reste lo que le toca
            printf("Player B decreasing %d \n", decrement);
            counter -= decrement;
            lseek(fd, 0, SEEK_SET);
            write(fd, &counter, sizeof(int)); //Update game.dat
            printf("Counter = %d\n", counter);
            turn = 1; // Cambiamos turno --> Important to make it BEFORE the signal 
        }
        pthread_mutex_unlock(&lock); // liberamos el lock
        pthread_cond_broadcast(&condA); // despertamos los otros threads
    }
    free(arg);
    //REVISAR CONDITION VARIABLES
}


int main(int argc, char* argv[]){
    // read number of tokens from CLI and write to binary file named game.dat
    int tokens = atoi(argv[1]); // from string to integer
    fd = open("game.dat", O_CREAT | O_RDWR, 0644);
    lseek(fd, 0, SEEK_SET);
    write(fd, &tokens, sizeof(int));

    //Initialize counter
    read(fd, &counter, sizeof(int));

    // Initialize condition and lock
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&condA, NULL);
    pthread_cond_init(&condB, NULL);

    //Creamos los 3 threads de A 
    pthread_t thread_A[NUM_THREADS]; //A[0] = -1; A[1] = -2; A[2] = -3;
    for (int i = 0; i < NUM_THREADS; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i + 1;
        pthread_create(&thread_A[i], NULL, thread_player_A, arg);
    }

    //Crear los 3 threads de B
    pthread_t thread_B[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i + 1;        
        pthread_create(&thread_B[i], NULL, thread_player_B, arg);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread_A[i], NULL);
        pthread_join(thread_B[i], NULL);
    }

    close(fd);

}
