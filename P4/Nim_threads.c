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
int winner = -1; // -1: No winner yet, 1 --> A, 0 --> B
int fd;
pthread_mutex_t lock;
pthread_cond_t condA, condB;

void *thread_player_A(void *arg) {
    usleep(10000);
    int decrement = *((int *)arg);
    free(arg);
    
    while (1) {
        pthread_mutex_lock(&lock);
        
        if (counter <= 0) {
            if (winner == 0)
                printf("THREAD: A.%d. I announce Player B is the winner\n", decrement);
            else if (winner == 1)
                printf("THREAD: A.%d. I announce Player A is the winner\n", decrement);
            pthread_mutex_unlock(&lock);
            return NULL;
        }
        
        while (turn == 0 && counter > 0)
            pthread_cond_wait(&condA, &lock);

        if (counter > 0 && turn == 1 && counter >= decrement) {
            printf("Player A decreasing %d (before: %d)\n", decrement, counter);
            counter -= decrement;
            printf("COUNTER after A: %d\n", counter);
            
            if (counter <= 0) {
                winner = 1;
                pthread_cond_broadcast(&condA);
                pthread_cond_broadcast(&condB);
            }
            else
            {
                pthread_cond_signal(&condB);
            }
            turn = 0;
        }
        
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *thread_player_B(void *arg) {
    usleep(10000);
    int decrement = *((int *)arg);
    free(arg);
    
    while (1) {
        pthread_mutex_lock(&lock);
        
        if (counter <= 0) {
            if (winner == 0)
                printf("THREAD: A.%d. I announce Player B is the winner\n", decrement);
            else if (winner == 1)
                printf("THREAD: A.%d. I announce Player A is the winner\n", decrement);            
            pthread_mutex_unlock(&lock);
            return NULL;
        }
        
        while (turn == 1 && counter > 0)
            pthread_cond_wait(&condB, &lock);

        if (counter > 0 && turn == 0 && counter >= decrement) {
            printf("Player B decreasing %d (before: %d)\n", decrement, counter);
            counter -= decrement;
            printf("COUNTER after B: %d\n", counter);
            
            if (counter <= 0) {
                winner = 0;
                pthread_cond_broadcast(&condA);
                pthread_cond_broadcast(&condB);
            }
            else
            {
                turn = 1;
                pthread_cond_signal(&condA);
            }
            
        }
        
        pthread_mutex_unlock(&lock);
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
    
    pthread_t thread_A[NUM_THREADS], thread_B[NUM_THREADS];
    
    for (int i = 0; i < NUM_THREADS; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i + 1;
        pthread_create(&thread_A[i], NULL, thread_player_A, arg);
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i + 1;
        pthread_create(&thread_B[i], NULL, thread_player_B, arg);
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread_A[i], NULL);
        pthread_join(thread_B[i], NULL);
    }
    
    printf("Game Over! Player %c wins!\n", winner == 1 ? 'A' : 'B');
    
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&condA);
    pthread_cond_destroy(&condB);
    
    return 0;
}
