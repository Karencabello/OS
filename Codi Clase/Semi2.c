#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

/* ---------------- EXERCICE 1 ---------------- */
// Write a C program that creates a child process and each of them prints their own PID, and that of their parent. Remember that you can create a child process by using the function fork and you can get the PID values with getpid and getppid.

/*int a;

int main(int argc, char* argv[]){
    int b;
    int *p2 = malloc(4);
    int *p = &a;
    *p = 3;
    int n = fork();
    if (n == 0) {
        sleep(1);
        int x = getppid();
        printf("I am the son %d and my father's PID is %d \n", *p, x);
    }
    else if (n != 0) {
        *p = 5;
        printf("I am the father %d and my son's PID is %d\n", *p,n);
    }
    wait(NULL);
} */

/* ---------------- EXERCICE 2 ---------------- */
/*int main(int argc, char* argv[]) {
    //Variables
    int fd;
    int pid;
    char *ftx = "fitxer.txt";
    char parent[] = "Hello\n";
    char child[] = "World\n";

    //Obrim fitxer
    fd = open(ftx , O_WRONLY | O_CREAT, 0644);

    //Gestionem error
    if(fd == -1){
        perror("Error");
        close(fd);
    }
    //Creem child process
    pid = fork();
    //És el fill
    if(pid == 0){
        //fem dormir
        int c = getpid() % 10000; //utilitzem ultims 4 digits
        usleep(c * 1000); //En milisegons
        //Escribim en fitxer
        write(fd, &child, strlen(child));
    }
    //És el pare
    else{
        //fem dormir
        int c = getpid() % 10000; //utilitzem ultims 4 digits
        usleep(c * 1000); //En milisegons
        //Escribim en fitxer
        write(fd, &parent, strlen(parent));
    }
    close(fd);
}*/
/* ---------------- EXERCICE 3 ---------------- */
/*int main(int argc, char* argv[]) {
    //Variables
    int pid;
    
    //Creem els fills
    for(int i = 0; i <= 100; i++){
        pid = fork();
        //És el fill
        if(pid == 0){
            sleep(1);
            printf("Child %d finished\n", getpid());
            exit(0); // Child exits
        }
    }
    //És el pare
     for (int i = 0; i < 100; i++) {
        wait(NULL);
    }
    
    printf("All children have finished.\n");
    return 0;
}*/
//Modify the program so that the waiting of 1 second is done by processes started with execvp which simply run the shell command “sleep 1”
/*int main(int argc, char* argv[]) {
    //Variables
    int pid;
    
    //Creem els fills
    for(int i = 0; i <= 100; i++){
        pid = fork();
        //És el fill
        if(pid == 0){
            char *args[] = {"sleep", "1", NULL};
            execvp("sleep", args); // Replace the child process with "sleep 1"
            exit(0); // Child exits
        }
    }
    //És el pare
     for (int i = 0; i < 100; i++) {
        wait(NULL);
    }
    
    printf("All children have finished.\n");
    return 0;
}*/
//Modify the program so that the waiting is of a 1 millisecond (use usleep), but the father needs to wait until a child finishes, before starting the next one.
/*int main(int argc, char* argv[]) {
    //Variables
    int pid;
    
    //Creem els fills
    for(int i = 0; i <= 100; i++){
        int n = pid = fork();
        //És el fill
        if(pid == 0){
            usleep(1 * 1000);
            printf("Child %d finished\n", getpid());
            exit(0); // Child exits
        }
        //És el pare
        else{
            wait(NULL);
            printf("Child %d has finished\n",n);
        }

    }     
    printf("All children have finished.\n");
    return 0;
}*/

/* ---------------- EXERCICE 4 ---------------- */
/*int main(int argc, char* argv[]) {
    //Variables
    int d[2];
    int pid;
    int parent_pid;
    //fem pipe
    pipe(d);
    //Creem el fill
    pid = fork();

    if(pid == 0){
        close(d[1]); //Tanquem escritura
        read(d[0], &parent_pid, sizeof(parent_pid)); //Llegim el pid del pare
        close(d[0]);//Tanquem lectura
        printf("El PID del meu pare és %d\n", parent_pid);
    }
    else{
        close(d[0]); //Tanquem lectura
        parent_pid = getpid(); //Mirem propi pid
        write(d[1], &parent_pid, sizeof(parent_pid)); //Escrivim el nostre pid
        close(d[1]); //Tanquem escritura
        printf("El meu PID es %d\n", getpid());
    }
    wait(NULL);//Esperem a que el procès acabi
    printf("Comunication finished");
}*/
/* ---------------- EXTRA EXERCICES ---------------- */
/* ---------------- EXERCICE 1 ---------------- */
/*int main(int argc, char* argv[]) {
 
}*/



