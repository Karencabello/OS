#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>


int main(int argc, char* argv[]) {
    int fd = open("Data/test.dat", O_RDONLY, 0644);
    int n;
    while(read(fd, &n, sizeof(int)) > 0){
        printf("%d\n", n);
    }
    close(fd);
}