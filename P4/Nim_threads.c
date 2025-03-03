#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define NUM_THREADS 3

int counter;
int turn = 1; // 1 --> A ; 0 --> B
int winner; // 1 --> A ; 0 --> B
int fd;
pthread_mutex_t lock;
pthread_cond_t condA;
pthread_cond_t condB;


void * thread_player_A(void * arg) {
    // Sleep para que acaben de hacerse
    usleep(10000);
    // Miramos qué hay que restar
    int* decrementp = (int*)arg;
    int decrement = (*decrementp);
    free(arg);
    
    while(1) {
        pthread_mutex_lock(&lock); 
         if (counter <= 0){ // YA TENEMOS GANADOR
            if(winner == 1){
                printf("THREAD: A.%d. I announce Player A is the winner\n", decrement);
            }
            else{
                printf("THREAD: A.%d. I announce Player B is the winner\n", decrement);
            }
            pthread_mutex_unlock(&lock);
            return NULL; // prueba con break
        }

        else if (counter > 0){ // EL JUEGO CONTINÚA
            while(turn == 0){ // si no es el turno de A, que sus threads descansen
                pthread_cond_wait(&condA, &lock);
            }

            if (turn == 1) { // si es el turno de A, que reste lo que le toca
                printf("Player A decreasing %d \n", decrement);
                counter -= decrement;
                printf("COUNTER: %d\n", counter);
                lseek(fd, 0, SEEK_SET);
                write(fd, &counter, sizeof(int)); //Update game.dat
                if(counter <= 0){
                    // Despertamos a todos lo threads. Todos han de anunciar el ganador.
                    winner = 1;
                    pthread_cond_broadcast(&condB);
                    pthread_cond_broadcast(&condA);
                }
                else{
                    turn = 0; // Cambiamos turno 
                    pthread_cond_signal(&condB); // despertamos un thread de B
                }
                pthread_mutex_unlock(&lock); 
                
            }
        }
    } 
    return NULL;
}

void * thread_player_B(void * arg) {
    // Sleep para que acaben de hacerse
    usleep(10000);
    // Miramos qué hay que restar
    int* decrementp = (int*)arg;
    int decrement = (*decrementp);
    free(arg);
    
    while(1) {
        pthread_mutex_lock(&lock); 

        if (counter <= 0){ // YA TENEMOS GANADOR
            if(winner == 1){
                printf("THREAD: B.%d. I announce Player A is the winner\n", decrement);
            }
            else{
                printf("THREAD: B.%d. I announcePlayer B is the winner\n", decrement);
            }
            pthread_mutex_unlock(&lock);
            return NULL; // prueba con break
        }

        else if (counter > 0){ // EL JUEGO CONTINÚA
            while(turn == 1){ // si no es el turno de B, que sus threads descansen
                pthread_cond_wait(&condB, &lock);
            }

            if (turn == 0) { // si es el turno de B, que reste lo que le toca
                printf("Player B decreasing %d \n", decrement);
                counter -= decrement;
                printf("COUNTER: %d\n", counter);
                lseek(fd, 0, SEEK_SET);
                write(fd, &counter, sizeof(int)); //Update game.dat
                if(counter <=0){
                    // Despertamos a todos lo threads. Todos han de anunciar el ganador.
                    winner = 0;
                    pthread_cond_broadcast(&condB);
                    pthread_cond_broadcast(&condA);
                }
                else{
                    turn = 1; // Cambiamos turno 
                    pthread_cond_signal(&condA); // despertamos un thread de A
                }
                pthread_mutex_unlock(&lock); 
            }
        }
    } 
    return NULL;
}


int main(int argc, char* argv[]){
    // read number of tokens from CLI and write to binary file named game.dat
    int tokens = atoi(argv[1]); // from string to integer
    fd = open("game.dat", O_CREAT | O_RDWR, 0644);
    lseek(fd, 0, SEEK_SET);
    write(fd, &tokens, sizeof(int));

    //Initialize counter
    lseek(fd, 0, SEEK_SET);
    read(fd, &counter, sizeof(int));
    //counter = 20;
    printf("Counter initialized to the value of %d\n", counter);

    // Inicializamos el lock y las dos condiciones
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&condA, NULL);
    pthread_cond_init(&condB, NULL);

    //Creamos los 3 threads de A 
    pthread_t thread_A[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i + 1;
        pthread_create(&thread_A[i], NULL, thread_player_A, arg);
    }

    // Creamos los 3 threads de B
    pthread_t thread_B[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i + 1;        
        pthread_create(&thread_B[i], NULL, thread_player_B, arg);
    }

    // Esperamos a que todos lo threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread_A[i], NULL);
        pthread_join(thread_B[i], NULL);
    }

    //VICTORIA
    // Print game result
    printf("Game Over! %s wins!\n", turn == 1 ? "Player B" : "Player A");

    //Cerrar todo 
    close(fd);
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&condA);
    pthread_cond_destroy(&condB);
    
    return 0;
}
