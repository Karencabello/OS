#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int myVar = 10;
    printf("My pid is %d \n", getpid());
    int n = fork(); // returns something
    /*if (n != 0) //create another child
        n =fork(); */
    if (n == 0) { // This code will only be executed by the child
        printf("I am the child, the value of myVar is %d \n", myVar);
        //printf("I am the child, my pid is %d. My parent pid is %d \n", getpid(), getppid());
    } else { // This code will only be executed by the father
        myVar = 100;
        printf("I am the parent, the value of myVar is %d \n", myVar);
        //printf("I am the parent, my pid is %d. My parent pid is %d \n", getpid(), getppid());
        int pid_child_wait;
        while(   (pid_child_wait = wait(NULL)) != -1) // Waits for a child to finish, returns -1 if no child        {
        {
            printf("Child with pid %d has finished \n", pid_child_wait);
        }
        printf("Now I can end \n");
    }

}