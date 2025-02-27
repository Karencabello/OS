#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int counter;
int turn; // 0 --> A ; 1 --> B

void * thread_player_A(void * arg) {
    // Write here the code to obtain the specific decremement of each thread
    while(counter >= decrement) {

        if (turn == 1 && counter >= decrememnt) {
            printf("Player A decreasing %d \n", decrement);
            counter -= decrement;
            //printf("Counter = %d\n", counter);
            turn = 0; // Important to make it BEFORE the signal
        }
    }
}

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