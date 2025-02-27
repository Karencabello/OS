#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* thread_function(void *arg) {
    int m= (int) arg;
    printf("I have received %d from the argument!", (int) arg);
    return  (void * )(m *m); // Remember to do the casting!
}

void f_squared(int n, int *r ){
    *r = n*n;
}

int main(int argc, char*argv[]) {
    pthread_t t;
    // 2nd argument: always NULL in this course (it is the settings of the thread)
    // 4th argument: what the f_thread will receive.
    int n = 42;
    pthread_create(&t, NULL, thread_function, (void *) n); // Remember to do the casting to int! 
    printf("Hi, from the 'parent', my pid is %d!\n", getpid());

    void * res_from_thread; // We need to make sure that we have enough bytes to get the result from pthread_join
    // In 64 bits systems: a pointer (like void *) is 8 bytes, and an int 4
    //int m = 23;
    //int res_from_thread;
    //int k = 12;
    pthread_join(t, (void **) &res_from_thread); // wait thread t, otherwise, the main process will end, and kill its threads
    //printf("Res = %d (%d %d)\n", (int) res_from_thread, m, k);
}