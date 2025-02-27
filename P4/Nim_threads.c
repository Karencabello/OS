#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define NUM_THREADS 3

//Handle_turn i handle_victory 

int counter;
int turn; // 0 --> A ; 1 --> B
int fd;

//Funcion del jugador A, le enviamos los 3 threads y el que llegue primero, pasará y los otros se cerraran.
void * thread_player_A(void * arg) {
    // Sleep para que acaben de hacerse
    ussleep(10000);
    //Miramos que thread A[i] llega primero
        //Haciendo lock y condition variable

    //Los otros dos se tienen que cerrar    

    //Con el A[i] hacemos la gestion y tal
    
    while(counter >= decrement) {

        if (turn == 1 && counter >= decrememnt) {
            printf("Player A decreasing %d \n", decrement);
            counter -= decrement;
            //printf("Counter = %d\n", counter);
            turn = 0; // Important to make it BEFORE the signal
        }
    }

    //Actualizar game.dat
    //Pasar de turno
}

//Funcion del jugador B, le enviamos los 3 threads y el que llegue primero, pasará y los otros se cerraran.
// Can also be done by sending which player is on the struct, which is its cond, and what is the other cond.
void * thread_player_B(void * arg) {// Write here the code to obtain the specific decremement of each thread
    while(counter >= decrement) {
        if (turn == 0  && counter >= decrememnt) {
            printf("Player B decreasing %d, turn = %d \n", decrement, turnA);
            counter -= decrement;
            turn = 1;
        }
    }
    printf("Player B, decrement %d finishing.\n", decrement);
}


int main(int argc, char* argv[]){
    // read number of tokens from CLI and write to binary file named game.dat
    int tokens = atoi(argv[1]); // from string to integer
    fd = open("game.dat", O_CREAT | O_RDWR, 0644);
    write(fd, &tokens, sizeof(int));

    //Initialize counter
    read(fd, &counter, sizeof(int));

    //Creamos los 3 threads de A 
    pthread_t thread_A[NUM_THREADS]; 
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&thread_A[i], NULL, thread_player_A, NULL);
    }

    //Crear los 3 threads de B
    pthread_t thread_B[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&thread_B[i], NULL, thread_player_B, NULL);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread_A[i], NULL);
        pthread_join(thread_B[i], NULL);
    }

    // game




}