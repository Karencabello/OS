// Write a program that opens a file (passed through CLI argument)
// , and writes the numbers from 0 to N, where N is given
// also through CLI argument.
// int open(char * s, O_RDONLY | O_WRONLY | O_CREAT , 0644)
// int write(fd, void * s, int nBytes)
// int sprintf(char *s, char * format, ...)
// int sscanf(char *s, char * format, &n);

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    char * filename = argv[1];
    int N;
    sscanf(argv[2], "%d", &N);
    int fd = open(filename, O_WRONLY | O_CREAT, 0644);

    for (int i = 0; i < N; ++i) {
        char buffer[100];
        char* pbuff = buffer;
        sprintf(buffer, "%d,", i);
        // After first iteration
        // buffer = ['0', ',','\0', Undetermined, Undetermined, Undetermined x100]

        write(fd, buffer, strlen(buffer));
    }

    // discard after current positoin
    ftruncate(fd, lseek(fd, 0, SEEK_CUR));
}