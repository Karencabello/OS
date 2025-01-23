#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *file = fopen(argv[1], "rb");
    int value;
    int position = 1; // Start reading from position 1

    while (fread(&value, sizeof(int), 1, file)) {
        printf("Position %d: %d\n", position, value);
        position++;
    }

    fclose(file);
    return 0;
}
