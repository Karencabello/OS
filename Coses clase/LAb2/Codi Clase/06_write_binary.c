#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>


int main(int argc, char* argv[]) {
    int fd = open("Data/test.dat", O_WRONLY | O_CREAT, 0644);
    
    for (int i = 0; i <50; ++i) {
        int n = i;
        write(fd, &n, sizeof(int));
    }
    close(fd);
}