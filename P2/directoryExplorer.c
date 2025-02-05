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

// LIBRERÍAS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h> // pid_t
#include <sys/wait.h>  // wait()
// USAGE
//.directoryExplorer directory

// FUNCIONES AUXILIARIES
void son(char* fileName, int fd[2], char* dirName){
    // Abrimos file
    char *fullpath = malloc(strlen(fileName) + strlen(dirName) + 2);  // si pasamos la dirección reltiva (fileName), fileFd es -1 porque  la file está en un directrio diferente al ejecutable
    snprintf(fullpath, strlen(fileName) + strlen(dirName) + 2, "%s/%s", dirName, fileName);
    int fileFd = open(fullpath, O_RDONLY);
    if (fileFd == -1){
        printf("Error abriendo el file\n");
    }
    // Leemos y sumamos todos los enteros del documento
    int fileSize = lseek(fileFd, 0, SEEK_END);
    char buffer[11];
    int num;
    int sum = 0;
    lseek(fileFd, 0, SEEK_SET);
    for(int i=0; i<fileSize; i+=4){
        read(fileFd, &num, sizeof(int));
        sum += num;
    }
    //printf("son sum = %d\n", sum);

    //Escribimos en pipe
    close(fd[0]); // Cerramos pipe de lectura
    sprintf(buffer, "%d", sum);  // Convierte el número a cadena
    write(fd[1], buffer, strlen(buffer)+1); // +1 --> para añadir \0 
    close(fileFd);
    free(fullpath);
    close(fd[1]); 
    exit(0); 

}

void father(int pipes[100][2], int nHijos){
    //Variables
    char num[12];
    int bytesRead; 
    for(int i=0; i<nHijos; i++){
        //Cerramos el canal de escritura
        close(pipes[i][1]);
        //Leemos el contenido de la pipe hasta que no haya más datos
        read(pipes[i][0], num, sizeof(num));
        //Imprimimos lo que hemos leído por standard output
        printf("SUM = %s\n", num);
        close(pipes[i][0]); // Cerramos pipe de lectura
    }
}


// FUNCIÓN MAIN
int main(int argc, char* argv[]) {
    //Gestión de errores argumentos
    if (argc < 2) {
        perror("Argumentos incorrectos");
        return 1;
    }
    // Obtenemos el nombre del directorio leyendo de argumentos CLI
    char * dirname = argv[1];
    
    // Abrimos el directorio especificado en dirname.
    DIR* d = opendir(dirname);
    //Gestión de errores al abrir el directorio
    if (d == NULL) {
        perror("No se pudo abrir el directorio");
        return 1;
    }
    //Creamos la pipe
    int nHijos = 0;
    int pipes[100][2]; // asumimos que no tendremos más de 100 .dat files
    //pipe(fd);

    // Declara un puntero para leer los archivos del directorio.
    struct dirent* fileInDir;

    // List files --> PRINT
    printf("List of files in %s:\n", dirname);
    // Itera sobre los elementos del directorio.
    while ((fileInDir = readdir(d)) != NULL) {
        printf("%s\n", fileInDir->d_name);
        //if dir == "".dat
        if (strstr(fileInDir->d_name, ".dat") != NULL) { // Usamos strstr para buscar el .dat en el nombre del archivo y mientras sea diferente a NULL,
            pipe(pipes[nHijos]);
            pid_t pid = fork(); //Cada vez que encuentre un archivo .dat llamara a fork y por lo tanto, creara un hijo
            if (pid == 0) {
                // Función aux: Escribe en la pipe la suma de los enteros en el archivo .dat
                son(fileInDir->d_name, pipes[nHijos], dirname); //Ajustar atributos y tal
            }
            nHijos++;
        }
    }  
    // Esperamos que todos los hijos escriban en la pipe antes de leer para evitar información errónea
    while(wait(NULL)>0){
    }
    // Cerramos el directorio
    closedir(d);
    //Función aux: imprime por pantalla todas las sumas recibidas de los diferentes hijos
    father(pipes, nHijos);
    return 0;
}