//This file creates a binary file that stores some integers
#include <fcntl.h>
#include <unistd.h>

int main() {
    int file = open("file.dat", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int values[] = {10,20,30,40,50}; // sample values to write to the file

    for (int i = 0; i < 5; i++) {
        write(file, &values[i], sizeof(int));
    }

    close(file);
    return 0;
}
