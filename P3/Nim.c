#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

//Create child process

//Programar el juego en general 
    //Name of tokens en el game.dat
    //Padre primero
    //El numero de tokens que puden sacar --> rand()%3 +1 (1,2,3)
    //El que saca el ultimo token gana --> My pid is x, i'm the (father/son), and i have lost
//en game.dat tambièn se guarda el número de tokens que quedan por sacar (como enteros binario)
//SIGUSR1 --> cambiar de turno (el que acaba de tirar)
//SIGUSR2 --> terminar el juego (lo envia el que gana)

/* ------------------------------------------------------------------------------- */
pid_t pid;
volatile int turn = 0; 
//0 --> padre 
//1 --> hijo 

int turn(int file){
    //Lee el numero de tokens que quedan
    int tokens = read(file, &tokens, sizeof(int));
    //Generamos los tokens que vamos a mover
    int tokens_to_move = rand()%3 + 1;
    //Restamos el numero de toquens que quedan ahora
    tokens = tokens - tokens_to_move;
    //Actualizamos el fichero --> 
    lseek(file, 0, SEEK_SET);
    write(file, &tokens, sizeof(int));
    //Miramos si alguien ha ganado/cambiamos de turno
    if(tokens <= 0){
        //Ha ganado
        signal(SIGUSR2,signal_handler);
    }
    else{
        //Cambia de turno
        signal(SIGUSR1,signal_handler);
    }
}

void signal_handler(int sig){
    char loser[7];
    int pid_loser;
    if(sig == SIGUSR1){
        if(turn == 0){ //Padre --> hijo
            turn++ ;
        }
        else{ //Hijo --> padre
            turn--;
        }
    }
    else if(sig == SIGUSR2){
        if(turn == 0){
            strcpy(loser, "Son");
            pid_loser = pid;
            printf("My pid is %d, I am the %s, and I have lost\n", pid_loser, loser);

        }
        else if(turn == 1){
            strcpy(loser, "Father");
            pid_loser = getppid(); //pid del padre
            printf("My pid is %d, I am the %s, and I have lost\n", pid_loser, loser);

        }
    }
}



int main (int argc, char *argv[]) {
    //abrir file
    int fd = open("game.dat", O_RDWR | O_CREAT, 0644);


    //Create child process
    pid = fork();

    //Joc
    if (pid == 0) { //fill
        sleep(1);

    }
    else { //pare

    }

    //CLOSE FILE!!!!!!!!!!!!!!!!!!!

}