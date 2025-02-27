/* There is an array (A[10]) in which each position has a random value from 0 to 2. The 
values represent the number of particles that are in a specific moment in a specific 
position (cell). Implement a threaded “particle” move operation: void * move(void 
arg), which arg is a struct containing to integers i and j, so that if in cell i there are 
more than 0 particles, the function moves one of the particles in i to the cell with index 
j.  
a.  Create the number of threads required to move simultaneously a particle from 
each even cell to the next greater odd cell, meaning from cell 0 one particle 
moves to cell 1, from cell 2 one particle moves from cell 3, etc. Do you need any 
synchronization mechanism? 
b.  Now assume 10 particles move, but decide randomly from which cell to which 
other cell they move. Hence no specific order needs to be preserved. Do you 
need any synchronization mechanism? 
 
c.  Now assume that a gust of wind blows from the cell with the lowest index, moving 
particles from all cells to the next higher index cell. From the ninth cell they can’t 
move. Do you need any synchronization mechanism? How would you do it?*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE 10

//Structure to store movement of arguments
typedef struct{
    int i,j;
} MoveArgs;

int A[SIZE]; //Array of particles
pthread_mutex_t mutex[SIZE]; //Mutex for syncronizarion

//Thread function to move a particle from cell i to cell j
void *move(void *arg){
    MoveArgs *args = (MoveArgs *) arg;
    int i = args->i;
    int j = args->j;
    free(arg);

    pthread_mutex_lock(&mutex[i]); //Lock source cell
    if(A[i] > 0){ //If there is a particle to move
        pthread_mutex_lock(&mutex[j]);  //Lock destination cell
        A[i]--; //Decrement particle count in source cell
        A[j]++; //Increment particle count in destination cell
        pthread_mutex_unlock(&mutex[j]);//Unlock destination cell
    }
    pthread_mutex_unlock(&mutex[i]); //Unlock source cell

    return NULL;
}

//Function to print array
void printArray(){
    for(int i = 0; i < SIZE; i++){
        printf("%d ", A[i]);
    }
    printf("\n");
}


int main(int argc, char *argv[]){
    pthread_t threads[SIZE];

    //Initialize array and mutex
    for(int i = 0; i < SIZE; i++){
        A[i] = rand() % 3;
        pthread_mutex_init(&mutex[i], NULL); // Random values between 0 and 2
    }

    printArray();

    //(a) Move particles from even cells to the next odd cell
    //Even index moves particle to next odd index
    for(int k = 0; k < SIZE; k += 2){
        if(k + 1 < SIZE){
            MoveArgs *args = malloc(sizeof(MoveArgs));
        args->i = k;
        args->j = k + 1;
        pthread_create(&threads[k], NULL, move, args);
        }
    }

    for(int k = 0; k < SIZE; k += 2){
        if(k + 1 < SIZE){
            pthread_join(threads[k], NULL);
        }
    }

    printArray();

    // (b) Random movements
    for(int k = 0; k < SIZE; k++){
        int n = rand() % SIZE;
        int f = rand() % SIZE;
        if(n != f){
            MoveArgs *args = malloc(sizeof(MoveArgs));
            args->i = n;
            args->j = f;
            pthread_create(&threads[k], NULL, move, args);
        }
    }

    for(int k = 0; k < SIZE; k++){
        pthread_join(threads[k], NULL);
    }

    printArray();

    // (c) Wind moves particles to the next higher index
    for(int k = 0; k < SIZE - 1; k++){
        MoveArgs *args = malloc(sizeof(MoveArgs));
        args->i = k;
        args->j = k + 1;
        pthread_create(&threads[k], NULL, move, args);
    }

    for (int k = 0; k < SIZE - 1; k++) {
        pthread_join(threads[k], NULL); //Ensures main thread wairs for each created thread
    }

    printArray();

    //Clean mutex
    for(int k = 0; k < SIZE; k++){
        pthread_mutex_destroy(&mutex[k]);
    }
    
    return 0;
}