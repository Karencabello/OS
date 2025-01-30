#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int count = 0;
void my_handler(int f){
    count += 1;
}

int main (int argc, char *argv[]) {
    signal(SIGINT, my_handler); // Pass the function my_handler as an argument
    // A function behaves like a pointer. It is the memory address of the 
    printf("Starting the loop\n");
    // Wait until you have received 5 times the signal SIGINT
    while(count < 5) {
        sleep(1);
        printf("Hi! My pid is %d\n", getpid());
    }
    printf("Exiting the loop\n");
}
