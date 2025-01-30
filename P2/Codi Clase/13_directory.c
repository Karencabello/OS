#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <dirent.h>

int main(int argc, char *argv[]) {
    char * filename = argv[1];
    
    DIR* d = opendir(filename);
    struct dirent* fileInDir;
    while ((fileInDir = readdir(d)) != NULL) {
        printf("%s\n", fileInDir->d_name);
        if (fileInDir->d_type == DT_DIR) {
            printf("It's a directory\n");
        }
        if (fileInDir->d_type == DT_REG) {
            printf("It's a file\n");
        }

    }
}