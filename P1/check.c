#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file = fopen("file.dat", "rb");
    int value;
    int position = 1; // Start reading from position 1
    
    while (fread(&value, sizeof(int), 1, file)) {
        printf("Position %d: %d\n", position, value);
        position++;
    }

    fclose(file);
    return 0;
}
