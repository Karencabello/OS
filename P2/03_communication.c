#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Create a program that creates a child
// The child computes 2*m, and sends it to the parent 
// Through the exit status.
int main(int argc, char* argv[]) {
    int myVar = 10;
    printf("My pid is %d \n", getpid());
    int n = fork(); // returns something
    if (n == 0) { // This code will only be executed by the child
        printf("I am the child, the value of myVar is %d \n", myVar);
        _exit(18); //equivalent to return in the main, but we ca
        return 42; // we send only 8 bytes

    } else { // This code will only be executed by the father
        myVar = 100;
        int ret;
        int pid_child_wait = wait(&ret); // Waits for a child to finish, returns -1 if no child        {
        // The return value includes
        printf("Now I can end, my child last message was: %d \n", 
                    WEXITSTATUS(ret));
    }
}