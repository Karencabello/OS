// Usage ./ex4 input.txt output.dat
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

void read_string(int fd, int number[2], int pos) {
    // functionality: given a file descriptor reads a string until /0 and fills gives its integer representation and the number of bytes read.
    int i = 0;
    char c[12];
    int nBytesRead = 0;
    int num;
    
    lseek(fd, pos, SEEK_SET);

    while(1){
        nBytesRead = read(fd, &c[i], sizeof(char));
        i++;
        if(c[i-1] == '\0') {
            number[0] = atoi(c); // integer
            number[1] = i;       // digits read
            break;
        }
    }
}
    

int main(int argc, char* argv[]) {
    char *file1 = argv[1];
    char *file2 = argv[2];
    int fd1 = open(file1, O_RDONLY);
    int fd2 = open(file2, O_WRONLY);
    int number[2];

    int fileSize = lseek(fd1, 0, SEEK_END);

    int p[2];
    pipe(p);

    int pid = fork();
    if(pid == 0) {
        sleep(1);
        //son: reads from pipe, writes integer to file2
        close(p[1]); // close writing channel
        int num;
        int reading = 1;
        lseek(fd2, 0, SEEK_SET);
        while(reading) {
            int nBytesread = read(p[0], &num, sizeof(int));
            printf("Son reads: %d\n", num);
            if(nBytesread == 0) {
                break;
            }
            write(fd2, &num, sizeof(int));
        }
        
    }

    else { 
        //father: reads numbers from input file (one by one), converts, pass as int to son (pipe)
        int bytesLeft = fileSize;
        int bytesRead = 0;
        while(bytesLeft>0) {
            read_string(fd1, number, bytesRead);
            bytesRead += number[1];
            close(p[0]); // close reading channel
            printf("Father reads: %d\n", number[0]);
            write(p[1], &number[0], sizeof(int));
            bytesLeft-= number[1];
        }
        wait(NULL);
    }
}


