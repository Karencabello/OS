#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char*argv[]) {
    if (argc !=2) {
        printf("I need 2 arguments\n");
        _exit(1);
    }
    //int fd = open(argv[1], O_WRONLY | O_CREAT, 0644);

    int n = fork();
    int fd = open(argv[1], O_WRONLY | O_CREAT, 0644); // what happens if the open is before the fork?

    if (n == 0) {
        srand(getpid()); // initialise the random generator
        //Code that only the child will execute  
        char *message = "ABCDEFGH\n";
        for (int i = 0; i < strlen(message); ++i) {
            write(fd, message + i, 1);
            usleep(rand()%1000 * 100);
        }
    }
    else {
        srand(getpid());
        //Code that only the father will execute  
        char *message = "0123456789\n";
        for (int i = 0; i < strlen(message); ++i) {
            write(fd, message + i, 1);
             usleep(rand()%1000 * 100);
        }    
    }
}