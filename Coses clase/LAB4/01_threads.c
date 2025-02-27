#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
/*
int main(int argc, char* argv[]) {
    int a = 0;
    int n = fork();
    if (n == 0){
        a = 10;
        printf("%d\n", a);
    }
    else{
        printf("%d\n", a);
    }

}*/

void* f_thread(void * a) {
    printf("Hi, my pid is %d!\n", getpid());
}

int main(){
    pthread_t t;
    // 2nd argument: always NULL in this course (it is the settings of the thread)
    // 4th argument: what the f_thread will receive.
    pthread_create(&t, NULL, f_thread, NULL); // Create new thread that executes f_thread, leaves the thread identifier in t
    printf("Hi, from the 'parent', my pid is %d!\n", getpid());
    pthread_join(t, NULL); // wait thread t, otherwise, the main process will end, and kill its threads
}