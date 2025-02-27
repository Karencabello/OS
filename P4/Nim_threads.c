#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>


//Handle_turn i handle_victory 

int main(int argc, char* argv[]){
    // read number of tokens from CLI and write to binary file named game.dat
    int tokens = atoi(argv[1]); // from string to integer
    fd = open("game.dat", O_CREAT | O_RDWR, 0644);
    write(fd, &tokens, sizeof(int));

    // game
}