/* The task is to 
    read a series of numbers from a file, encoded in ASCII as a string, 
    convert them to a binary-encoded integer, 
    and  write them  in  a  second  file.  
The  code  will  be  concurrent,  using  a  father  and  a child processes. 
    The father will 
        read  numbers one by one from the input file, 
        convert them to integers 
        and pass them through a pipe to the son. 
    The son  will 
        read from the pipe 
    and write each integer to the output file. 
The two processes will end when 
    the input file has been completely read, 
    and all integers have been processed. 
The path of the input and output files is given as the first and  second command-line arguments.  
a) Write a function that reads a string from a source identified by a filedescriptor, 
until a ”\0” byte marking the end of the string is read. You cannot use lseek, 
and you cannot read ahead of the end of the string. 
b)  Implement the rest of the program, using the previous function. Make sure 
that you avoid deadlocks related to pipe communication*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

// gcc ex4.c -o ex4 file1 file 2

// a)
char *read_string(int filedescriptor) {
    char *buffer = malloc(1024) ;
    if (!buffer) {
        perror("malloc error");
        return NULL;
    }

    char c;
    int bytes_read;
    int i = 0;

    while(1) {
        bytes_read = read(filedescriptor, &c, 1);

        if(bytes_read <= 0 || c == '\0'){
            break;
        }
        buffer[i++] = c; 
    }

    buffer[i] = '\0';
    return buffer;

}

int main(int argc, char* argv[]){

    if (argc < 3) {
        fprintf(stderr, "Uso: %s <archivo_entrada> <archivo_salida>\n", argv[0]);
        exit(1);
    }

    // Variables
    int fd_in, fd_out;
    char* buffer = NULL;
    int pipe_fd[2];

    // Crear pipe
    if (pipe(pipe_fd) == -1) {
        perror("Error creando pipe");
        exit(1);
    }
    
    
    // Get files
    fd_in = open(argv[1], O_RDONLY);
    if (fd_in == -1) {
        perror("Error abriendo archivo de entrada");
        exit(1);
    }

    fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out == -1) {
        perror("Error abriendo archivo de salida");
        close(fd_in);
        exit(1);
    }


    // child process
    int n = fork();
    if (n < 0) {
        perror("Error en fork");
        close(fd_in);
        close(fd_out);
        exit(1);
    }

    if (n == 0){
        // Child
        close(pipe_fd[1]); // Cerramos escritura pipe

        int num; 
        // Read from pipe i write en file 2
        while(read(pipe_fd[0], &num, sizeof(int)) > 0){
            printf("Hijo: Recibí el número %d\n", num);
            write(fd_out, &num, sizeof(int));
        }

        // close todo

        close(pipe_fd[0]);
        close(fd_out);
        exit(0);
    }

    else{
        // Father
        close(pipe_fd[0]); // Cerramos lectura

        char *buffer;

        //read from file 1
        while((buffer = read_string(fd_in)) && buffer[0] != '\0'){
            int num = atoi(buffer);
            printf("Padre: Enviando número %d\n", num);
            write(pipe_fd[1], &num, sizeof(int));
            free(buffer);
        }

        // Close todo
        close(fd_in);
        close(pipe_fd[1]);

        wait(NULL); // Esperamos que hijo termine

    }
   
    return 0; 
}