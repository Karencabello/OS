/*increases and decreases a counter using  threads. 
    It involves a thread increasing the counter a random number whenever it reaches 0, 
    and three decreasing threads decreasing the  counter,  
    ** each  one  decreasing  the  counter  by  1,  5  and  10  units  at  a  time respectively. 
A code for the increase part is already given below,  
    a single thread that adds a random number to the counter reaches 0. 
    Assume that there is a global variable end that will be set to one to mark that the program will end when the counter reaches 0.
 
int counter = 0 ;  
int end = 0 ;  
int increment ( void ∗ a ) {  
while ( end == 0 ) {  
if ( counter == 0 ){  
counter += rand ( ) \%1000 ; // add a random number  
}  
} 
 
a)  Are there any race conditions in the increment thread, when the decrement 
threads are executing? If so, identify  the variables involved, the critical region 
(in lines) and which synchronisation tool you would use.  
b)  Implement the decrease function, that will decrease the counter by 1, 5 and 
10  units  at  a  time,  depending  on  the   thread  executing  it.  Avoid  race 
conditions and starvation.  
c)  Implement the main program, that will create the 3 decreasing threads and 
the increment thread, and then waits until all threads finish */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int counter = 0;
int end = 0;
pthread_mutex_t lock;

void* increment ( void* arg ) {  
    while ( end == 0 ) {  
        pthread_mutex_lock(&lock);
        if ( counter == 0 ){  
            counter += rand ( ) % 1000 ; // add a random number  
            printf("Incremented counter to %d\n", counter);
        } 
        pthread_mutex_unlock(&lock);
        usleep(100000); // Prevent waiting
    }
    return NULL;
} 

void* decrement(void* arg){
    int value = *(int*)arg;
    free(arg);

    while ( end == 0 ) {
        pthread_mutex_lock(&lock);
        if ( counter >= value ) { // Only if there is enoug value
            counter -= value;
            printf("Decremented counter to %d\n", counter);
        }
        pthread_mutex_unlock(&lock);
        usleep(50000); // Prevent CPU overload
    }
    return NULL;
}

int main(){
    pthread_t inc_thread, dec_threads[3];
    int values[3] = {1, 5, 10};

    // Create increment thread
    pthread_create(&inc_thread, NULL, increment, NULL);

    // Create decrement thread
    for (int i = 0; i < 3; i++) {
        int* val = malloc(sizeof(int));
        *val = values[i];
        pthread_create(&dec_threads[i], NULL, decrement, val);
    }

    // Simulate
    sleep(5);
    end = 1;

    // Wait for threads
    pthread_join(inc_thread, NULL);
    for (int i = 0; i < 3; i++) {
        pthread_join(dec_threads[i], NULL);
    }

   

    // Clean
    pthread_mutex_destroy(&lock);
    printf("Final counter value %d\n", counter);
    return 0;
}
