#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <dirent.h>

int main(int argc, char *argv[]) {
    char * filename = argv[1];
    int N;
    sscanf(argv[2], "%d", &N);
    int fd = open(filename, O_WRONLY | O_CREAT, 0644);
    printf("Fd %d bytes\n", fd);
    printf("Errno %d\n", errno);
    printf("Strerror %s\n", strerror(errno));


    int i =0;
    char buffer[100];
    char* pbuff = buffer;
    sprintf(buffer, "%d,", i);
    // After first iteration
    // buffer = ['0', ',','\0', Undetermined, Undetermined, Undetermined x100]

    int nBytes = write(fd, buffer, strlen(buffer));
    
    printf("Wrote %d bytes\n", nBytes);
}