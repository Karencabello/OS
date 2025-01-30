#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// Create a child, the child executes build/01
// The father waits until its execution
int main(int argc, char * argv[]) {
    int n = fork();
    if (n == 0) {
        
        char* args_exec[] = {"01", NULL}; // These are the CLI arguments
        execvp("build/01", args_exec); 
        // After the previous line, the code of this process will change to
        // that of build/01 , if it is correct. Otherwise it will give an error
        printf("Hello!\n"); // This will not be executed unless the execvp fails
    }
    else {
        // Wait will wait until any child dies
        wait(NULL); // We put NULL because we are not interested in the return value
        //waitpid(n, NULL);  // This is equivalent, and will wait for a specific child.
    }
}