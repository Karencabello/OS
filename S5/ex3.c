/*main will 
    create seven processes that will search for “gold” in a text file. 
    The name of the file that each child will open will be passed through a pipe 
    (one pipe for each child process). Then, the children will search for appearances of 
    the substring “gold” in their file, and whenever they find it, they will send it to the main 
    through another pipe, indicating the position and filename where it was found (this 
    pipe  shared  among  the  different  children).  To  pass information you can use the 
    structure GoldPosition below. 
Then, the main will read from the pipe all instances of 
found gold, and write them in the standard in/output. Remember to use the adequate 
functions to compare and copy strings, and that strings need to have a ’\0’ at the end. 
a)  Create a function checkStringAtPos that, given a file descriptor, a position and 
a string, returns 1 if the file contains that string at the given position, -1 if the 
string would fall outside of the range of the file, and 0 otherwise. 
b)  Using the previous function, implement the described code for the main and 
the children. Remember to properly close all pipes. */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define N 7
#define GOLD "gold"

typedef struct { // you can modify i f you want 
    char fileName [100] ; 
    int pos ; 
} GoldPosition;

int checkStringAtPos(int fd, int pos, const char* str){
    char buffer[5]; // gold --> 4 chars + \0

    //Seek position
    lseek(fd, pos, SEEK_SET);

    // Read content for position 
    read(fd, buffer, strlen(str));

    //End srtring
    buffer[strlen(str)] = '\0';

    // Compare
    if (strcmp(buffer, str) == 0){
        return 1;
    } 
    else {
        return 0;
    }
}

// Searches gold in the file --> child process
void searchForGOld(int readpipe, int wripepipe){
    GoldPosition result;
    int fd;
    int pos = 0;
    char fileName[100];

    // Read name from pipe
    read(readpipe, fileName, sizeof(fileName));

    // open the file
    fd = open(fileName, O_RDONLY);

    // Search for gold
    while (1){
        int found = checkStringAtPos(fd, pos, GOLD);
        if (found == 1){
            // Send it to main
            result.pos = pos;
            strncpy(result.fileName, fileName, sizeof(result.fileName));
            result.fileName = fileName;
        }
    }

}