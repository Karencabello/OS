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

void son(){
    //Funciones que hace el hijo
        //Abrimos file
        //Leemos file como int
        //sumamos int
        //Escribimos en pipe
        //exit!!!!!!!!!

}

void father(int fd){
    //Variables
    char buffer[1024]; //Leer contenido de la pipe
    ssize_t bytesRead; 
    //read pipe hasta que no haya más datos
    while((bytesRead == read(fd, buffer, sizeof(buffer))) > 0){
        //Imprimimos lo que hemos leido
        printf("%s\0", buffer);
    }
        //Cuando todos acaben --> exit
        exit(0);
}





int main(int argc, char* argv[]) {
    //Gesstion de errores argumentos
    if (argc < 2) {
        perror('Argumentos incorrectos');
        return 1;
    }
    // 1. Abrir el directorio
    // Coge directorio argumentos CLI
    char * filename = argv[1];
    
    // Abre el directorio especificado en filename.
    DIR* d = opendir(filename);
    //Gestión de errores al abrir el directorio
    if (d == NULL) {
        perror("No se pudo abrir el directorio");
        return 1;
    }
    //Creamos la pipe
    int fd[2];
    pipe(fd);

    // Declara un puntero para leer los archivos del directorio.
    struct dirent* fileInDir;

    // Itera sobre los elementos del directorio.
    while ((fileInDir = readdir(d)) != NULL) {
        //List files --> PRINT
        printf('List of files in %c:\n', filename);
        printf("%s\n", fileInDir->d_name);
        //if dir == .dat
        if (strstr(fileInDir->d_name, ".dat") != NULL) {
            //Usamos strstr para buscar el .dat en el nombre del archivo y mientras sea diferente a NULL,
            //es decir, que no se encuentran más .dat , ejecutamos el codigo.

            //Creamos hijo
            pid_t pid = fork(); //Cada vez que encuentre un archivo .dat llamara a fork y por lo tanto, creara un hijo
            if (pid == 0) {
                close(fd[0]); //Cerramos pipe lectura antes de ejutar son
                //TAMBIEN SE PUEDE HACER DENTRO DE SON NS Q ES MEJOR
                //llamar a funcion son
                son(fileInDir->d_name, fd[1]); //Ajustar atributos y tal
                //Exit son en la función son
            }
            //El padre no hace nada y continua con la ejecución
        }
        
        //llamar a la funcion padre
        //exit padre
        

    }  
    //Cerramos el directorio
    closedir(d);
    //Como ya han acabado todos los hijos empecemos con el padre
    //Cerramos pipe escritura
    close(fd[1]);
    //Llamamos a la función padre
    padre(fd[0]);
    //Cerramos pipe lectura
    close(fd[0]);
    //Acabamos
    return 0;
}