#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    int fd = open("Data/test.txt", O_RDONLY);
    char s[100];
    int nBytesRead;
    /* Incorrecte
    while ((nBytesRead = read(fd, s, 10)) > 0) {

        for (int j = 0; j < nBytesRead; ++j) {
            if (s[j] == ',') {
                s[j] = '\0';
                printf("%s\n",s);
                break;
            } 
        }
    }
    */
    int endOfFile = 0;
    while (!endOfFile) {
        char s[100];
        for (int j = 0; j < 100; ++j) {
            nBytesRead = read(fd, &s[j], 1);
            if (nBytesRead == 0) { // Read returns 0 if it reaches the end of the file
                s[j +1] = '\0';
                endOfFile = 1; //end the while
                break;
            }
            if (s[j] == ',') {
                s[j] = '\0';
                break;
            }
        }
        printf("%s\n",s);
    }
    close(fd);
}