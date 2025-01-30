#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void my_handler(int f){
    char s[100];
    printf("Are you sure you want to exit?\n");
    scanf("%s", s);
    if (s[0] == 'Y')
        _exit(1);
    else
        return;
}

int main (int argc, char *argv[]) {
    signal(SIGINT, my_handler); // Pass the function my_handler as an argument
    // A function behaves like a pointer. It is the memory address of the  start of the function
    printf("Starting the loop\n");
    while(1) {
        sleep(1);
        printf("Hi! My pid is %d\n", getpid());
    }
}
