#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file = fopen("file.dat", "wb");
    int values[] = {10, 20, 30, 40, 50}; // sample values to write to the file

    for (int i = 0; i < 5; i++) {
        fwrite(&values[i], sizeof(int), 1, file);
    }

    fclose(file);
    return 0;
}
