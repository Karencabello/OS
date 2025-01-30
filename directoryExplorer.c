/* 
1. open a directory, and list all its content 
2. When it finds a file ending in .dat  it will create a son, 
3. that son will:
    3.1. open that file
    3.2. read all its content as integers
    3.3. write the total sum of these integers in a pipe to the father 
    3.4 and finish.
4. The father will: 
    4.1. continue the exploration of the directory without waiting for its childs. 
    4.2. Then, when it has visited all the content in the directory, it will read 
    all the content written in the pipe
    4.3. and print it in the standard output. 
    4.4. then finish when all of its childs have also finished.
HINTS:
    * Notice that several processes can write simultaneously in the same pipe. 
    * Beware when you read from pipes, that you must close its reading endpoint 
    before writing.
Allowed function calls: 
    * open, close, read, write, dup2, fork, wait, waitpid, pipe, getpid, 
getppid, printf, scanf, malloc, free. You are not allowed to use any other functions 
that directly or indirectly use a system call (i.e. fwrite). 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
// Usage: /.directoryExplorer directory

int son(){
    //Funciones que hace el hijo
        //Abrimos file
        //Leemos file como int
        //sumamos int
        //Escribimos en pipe
        //exit

}

int father(){
    int fd[2];
    pipe(fd);
    //Father
        //read pipe
        //printf
        //Cuando todos acaben --> exit
}





int main(int argc, char* argv[]) {
    // 1. Abrir el directorio
    // Coge directorio argumentos CLI
    char * filename = argv[1];
    
    // Abre el directorio especificado en filename.
    DIR* d = opendir(filename);

    // Declara un puntero para leer los archivos del directorio.
    struct dirent* fileInDir;

    // Itera sobre los elementos del directorio.
    while ((fileInDir = readdir(d)) != NULL) {
        //List files --> PRINT
        //if dir == .dat
        //Creamos hijo
        //llamar a funcion son
        //exit son
        //llamar a la funcion padre
        //exit padre
        

    }   
}