// Make a program with a father and a son.
// The father writes a message every second (including its PID), and the son every 2
// Whenever the father receives SIGINT, it will also kill his son before exiting.

// #include <signal.h>
// kill (pid, SIGNAME);
// signal(SIGNAME, f_handler);
// sleep
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int pid_son;
void f_handler(int dummy){ 
    kill(pid_son, SIGKILL);
    _exit(1);
}

int main(int argc, char* argv[]){
    int n = fork();
    if (n == 0){
        signal(SIGINT, SIG_IGN);

        while(1) {
            sleep(2);
            printf("I am the child, and my pid is %d\n", getpid());
        }
    }
    else{ 
        pid_son = n;
        signal(SIGINT, f_handler);
        while(1) {
            sleep(1);
            printf("I am the father, and my pid is %d\n", getpid());
        }
    }

}