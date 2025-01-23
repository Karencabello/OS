#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

//ASSUMPTION POSITIONS START AT 1

void modifyFbinary(int fd, int position, int new_value) {
    // int lseek(int fd, int offset, SEEK_SET/SEEK_CUR/SEEK_END)
    lseek(fd, (position-1)*sizeof(int), SEEK_SET);
    // int write(int fd, void* array, int nBytes)
    write(fd, &new_value, sizeof(new_value));
}

void bytesAndLengthOld(int fd, int position, char newVal[], int* bytesUntilpos, int* digitsOld){
    int bytes = 0;                // counter: counts the number of bytes that we read
    int nBytesRead;               // used to count the number of bytes read by the function read()
    bool found = false;           // indicates if we have already reached the position of the element of interest
    int pos = 1;                  // counter: counts the positions (integers) that the file has (until we reach the element of interest)
    *digitsOld = 0;               // counter: counts the digits/chars that the old element at the position of interest has
    //bytesUntilpos               // counter: counts the number of bytes that come before the element of interest
    
    off_t fileSize = lseek(fd, 0, SEEK_END); 
    char s;  
    lseek(fd, 0, SEEK_SET);   
    for(off_t i=0; i<fileSize; i++) {
        int nBytesRead = read(fd, &s, 1);
        // check if we have reached the target position
        if(pos == position && s !=','){
            *bytesUntilpos = bytes;
            found = true;
        }

        if(found) {
            if('0' <= s && s <= '9'){
                (*digitsOld)++;
            } 
            else if(s == ',' || i == fileSize-1){
                break;
            }
        }
        else {
            bytes++;
        }

        // Update current position and number of bytes read
        if(s == ','){
            pos++;
        }
    }
}

void modifyFtext(int fd, int position, char* newVal) {
    // identify the number of bytes to read until the comma (included) before the number of interest at position N = position
    int bytesUntilpos;
    // identify the number of bytes (char --> digits) of the OLD value at the position of interest --> digitsOld
    int digitsOld;
    bytesAndLengthOld(fd, position, newVal, &bytesUntilpos, &digitsOld);
    // identify the number of char(digits) of the NEW value --> digitsNew
    int digitsNew = strlen(newVal);
    // Get the file size (off_t has a greater capacity than int)
    off_t fileSize = lseek(fd, 0, SEEK_END); 
    //Dynamic array
    char* buff = (char*)malloc(fileSize - bytesUntilpos - digitsOld); 
    //char buff[fileSize - bytesUntilpos]; this is not possible because the length of the array would be decided at runtime

    if(digitsOld == digitsNew) {
       lseek(fd, bytesUntilpos, SEEK_SET);
       write(fd, newVal, digitsNew); 
    }

    else {
        lseek(fd, bytesUntilpos + digitsOld, SEEK_SET); // we want to read all content after the old values (but we don't care about the old value)
        int nBytesRead = read(fd, buff, fileSize - bytesUntilpos - digitsOld);
        buff[nBytesRead] = '\0';
        lseek(fd, bytesUntilpos, SEEK_SET);
        write(fd, newVal, digitsNew); // replace old value
        if(nBytesRead>0){ // if the buffer is not empty
            write(fd, buff, fileSize-bytesUntilpos-digitsOld);
        }
        if(digitsOld > digitsNew) {
            ftruncate(fd, bytesUntilpos + digitsNew + nBytesRead);
        }
    }
    free(buff);
}


int main(int argc, char* argv[]) {

    // READ CLI ARGUMENTS
    // 1) the file is in binary or txt
    char* fileType = argv[1];
    // 2) the input file
    char* filename = argv[2];
    // 3) new position
    int position;
    sscanf(argv[3], "%d", &position);
    // 4) value to substitute
    int new_val;
    sscanf(argv[4], "%d", &new_val); // use sscanf to convert from string to intefer
    char* newVal = argv[4];          // maintain it as a string

    // OPEN FILE
    int fd = open(filename, O_RDWR | O_CREAT, 0644);

    // MODIFY VALUE
    if(strcmp(fileType, "-binary") ==0){
        modifyFbinary(fd, position, new_val);
    }
    else{
        modifyFtext(fd, position, newVal);
    }

    close(fd);
    return 0;
}
