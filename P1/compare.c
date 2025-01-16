/*
This small program allows you to compare two files byte by byte.
Usage: ./compare file1 file2
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>


int main(int argc, char *argv []) {
    if(argc != 3) {
        printf("Incorrect number of arguments: ./compare fileA fileB\n");
    }
    int fd1 = open(argv[1], O_RDONLY);
    int fd2 = open(argv[2], O_RDONLY);
    if(fd1 == -1 || fd2 == -1) {
        printf("Error opening files: %d %d  %s\n", fd1, fd2, strerror(errno));
        return 1;
    }
    char c1, c2;
    int nBytesRead1 = 1, nBytesRead2 = 1;
    for (int i = 0; 1; i++) {
        nBytesRead1 = read(fd1, &c1, 1);
        nBytesRead2 = read(fd2, &c2, 1);
        if(nBytesRead1 == 0 && nBytesRead2 == 0) {
            printf("The files are the same\n");
            break;
        }
        if(nBytesRead1 == 0) {
            printf("File 1 ended before file 2\n");
            break;
        }
        if(nBytesRead2 == 0 ) {
            printf("File 2 ended before file 1\n");
            break;
        }
        if(c1 != c2) {
            printf("Files are different at byte %d\n", i);
            break;
        }
    }
}