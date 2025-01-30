#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <dirent.h>

int main(int argc, char *argv[]) {
    // Coge directorio argumentos CLI
    char * filename = argv[1];
    
    // Abre el directorio especificado en filename.
    DIR* d = opendir(filename);

    // Declara un puntero para leer los archivos del directorio.
    struct dirent* fileInDir;

    // Itera sobre los elementos del directorio.
    while ((fileInDir = readdir(d)) != NULL) {
        // Imprime el nombre del archivo o directorio.
        printf("%s\n", fileInDir->d_name);

        // Verifica si el elemento es un directorio.
        if (fileInDir->d_type == DT_DIR) {
            printf("It's a directory\n");
        }
        // Verifica si el elemento es un archivo regular.
        if (fileInDir->d_type == DT_REG) {
            printf("It's a file\n");
        }

    }
}