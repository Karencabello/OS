#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Make N + 1 threads, assure that threadA executes before threadB
// We want the entree before the main dish 
#define N 10
pthread_mutex_t lock;
int entree = 0;

void * thread_A(void *A) {
    printf("Hi, I am the entree\n");

    pthread_mutex_lock(&lock);
    entree += 1;
    pthread_mutex_unlock(&lock);

}

void * thread_B(void *A) {
    //pthread_mutex_lock(&lock); // Deadlock: thread B waits for threadA, thread A wits for thread B
                                //, need to remove the lock and unlock to make it work!!
    while (entree != N);
    //pthread_mutex_unlock(&lock);

    printf("Hi, I am the main dish\n");
}
int main(int argc, char * argv[]) {
    pthread_mutex_init(&lock, NULL);
    pthread_t t1, t2;
    pthread_t ts[N];
    pthread_create(&t1, NULL, thread_B, NULL);
    sleep(1); // Just to wait
    for (int i = 0; i < N; ++i)
        pthread_create(&ts[i], NULL, thread_A, NULL);

    pthread_join(t1, NULL);
    for (int i = 0; i < N; ++i)
        pthread_join(ts[i], NULL);
}