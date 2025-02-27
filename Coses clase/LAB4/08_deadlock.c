#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define N 10000
#define M 10
int count = 0;
int count_2 = 0;
pthread_mutex_t lock;
pthread_mutex_t lock2;

// Deadlock arises when two threads are waiting to each other to release a lock
void* count_thread(void *) {
    for (int i = 0; i < M; ++i){
        pthread_mutex_lock(&lock2); // This will block, because the locks are in the inverse order in each thread
        usleep(100);
        pthread_mutex_lock(&lock);
        count += 1;
        count_2 += 1;
        pthread_mutex_unlock(&lock);
        pthread_mutex_unlock(&lock2);

    }
} 
void* count_thread2(void *) {
    for (int i = 0; i < M; ++i){
        pthread_mutex_lock(&lock);
        pthread_mutex_lock(&lock2);
        count   += 1;
        count_2 += 1;
        pthread_mutex_unlock(&lock);
        pthread_mutex_unlock(&lock2);

    }
} 


int main(){
    pthread_mutex_init(&lock, NULL);
    pthread_mutex_init(&lock2, NULL);

    pthread_t tids[N];
    for (int i = 0; i < N; ++i) {
        if (i %2 == 0)
            pthread_create(&tids[i], NULL, count_thread, NULL);
        else
            pthread_create(&tids[i], NULL, count_thread2, NULL);

    }
    for (int i = 0; i < N; ++i) {
        pthread_join(tids[i], NULL); // we wait for the thread to finish, but we don't look at its return
    }
    printf("Result is %d\n", count);
}