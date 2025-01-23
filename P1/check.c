// This file prints the integeres stored in a binary file.
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int file = open("file.dat", O_RDONLY);
    int value;
    int position = 1; // Start reading from position 1

    while (read(file, &value, sizeof(int)) > 0) {
        printf("Position %d: %d\n", position, value);
        position++;
    }

    close(file);
    return 0;
}
