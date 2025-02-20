// Create a son that executes the instruction "sleep 5"
// Then, create a second son that kills the son, if it lasts for more than 3s
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

// fork
// char* argv_sleep[] = {"sleep", "5", NULL};
// execvp("sleep", argv_sleep);
// sleep
// kill

int main(int argc, char* argv[]) {
    int n = fork();
    if (n == 0){
        char* argv_sleep[] = {"sleep", "5", NULL};
        execvp("sleep", argv_sleep);
        _exit(1);
    }
    int n2 = fork();
    if (n2 == 0){
        sleep(3);
        kill(n, SIGKILL);
        _exit(0);
    }
    
    wait(NULL);
    wait(NULL);
}