#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define N 1000
#define M 1000
int count = 0;
void* count_thread(void *) {
    for (int i = 0; i < M; ++i){
        int r = count; //load count from memory
        r = r+ 1;
        count = r;
    }
} 
int main(){
    pthread_t tids[N];
    for (int i = 0; i < N; ++i) {
        pthread_create(&tids[i], NULL, count_thread, NULL);
    }
    for (int i = 0; i < N; ++i) {
        pthread_join(tids[i], NULL); // we wait for the thread to finish, but we don't look at its return
    }
    printf("Result is %d\n", count);
}