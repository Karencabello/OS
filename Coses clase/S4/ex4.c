/* ssume you have to develop a simple resource management system to limit the amount 
of services querying into a database, which for this exercise will be 5. Hence, create 20 
threads  that will aim to access the database, but they will need to wait if there are 
already  5  threads  sending  a  query  to  the  database.  To  simulate the access to the 
database, you can use the following function, Use a semaphore to provide the proper control.*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_RESOURCES 5 
#define NUM_THREADS 20

sem_t *db_semaphore; //Semaphore for resource control

int nThreadsAccessing = 0; 
 
//Function to simulate a database access
int getFromDatabase() { 
   nThreadsAccessing++; 
   printf("Number of Threads accessing the DB = %d\n", nThreadsAccessing); 
   if (nThreadsAccessing > NUM_RESOURCES) _exit(1);  //Safety check
   usleep(rand() % 10000); 
   nThreadsAccessing--; 
   return rand(); 
}

//Function to access database
void *threadFunction(void *arg){
    sem_wait(db_semaphore); //wait time max reached
    getFromDatabase(); //Access database
    sem_post(db_semaphore); //release semaphore
    return NULL;
}

int main(){
    pthread_t threads[NUM_THREADS];
    srand(time(NULL));

    //Initialize semaphore
    db_semaphore = sem_open("/db_semaphore", O_CREAT, 0644, NUM_RESOURCES);


    //Create threads
    for(int i = 0; i < NUM_THREADS; i++){
        pthread_create(&threads[i], NULL, threadFunction, NULL);
    }

    //Join threads
    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    //Destroy semaphore
    sem_close(db_semaphore);

    return 0;
}