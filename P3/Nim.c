#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>


// Global variables

int fd;
int receiver_pid;
int current; // 0 for father, 1 for son

// Auxiliary functions

void handle_turn(int sig1){
    int current_tokens;

    //Read number of tokens from the file
    lseek(fd, 0, SEEK_SET);
    read(fd, &current_tokens, sizeof(int));

    // Remove 1,2,3 tokens randomly
    int taken_tokens = rand()%3+1;
    int new_tokens = current_tokens-taken_tokens;

    // for debugging
    printf("I am %d and my pid is %d. I will subtract %d. Now there are %d tokens left\n", current, getpid(), taken_tokens, new_tokens);
    
    //No winner --> update the file with the tokens remaining
    if(new_tokens > 0){
        lseek(fd, 0, SEEK_SET);
        write(fd, &new_tokens, sizeof(int));
        //Send signal to let know the opponent it's their turn
        kill(receiver_pid, SIGUSR1);
    }
    //There's a winner --> send signal to the loser
    else{
        kill(receiver_pid, SIGUSR2);
    }

}
void handle_victory(int sig2){
    int loser_pid = getpid();
    char loser[7];

    //Identify who is the loser
    if(current == 0){
        strcpy(loser, "Father");
    }
    else{
        strcpy(loser, "Son");
    }
    printf("My PID is %d, I am the %s, and I have lost. \n", loser_pid, loser);
    
    //CLOSE FILE
    close(fd);
    exit(0);
}

// Main

int main(int argc, char* argv[]){
    // read number of tokens from CLI and write to binary file named game.dat
    int tokens = atoi(argv[1]); // from string to integer
    fd = open("game.dat", O_CREAT | O_RDWR, 0644);
    write(fd, &tokens, sizeof(int));

    // game
    srand(time(NULL)); 
    int n = fork();
    if(n == 0){ // son process
        receiver_pid = getppid(); // because son sends the signal to father
        current = 1;

        signal(SIGUSR1, handle_turn); // set signal
        signal(SIGUSR2, handle_victory);

        // game loop
        pause(); //Wait for signal
        while(1){
            pause(); //wait turn
    
        }

    }

    else{ // father process
        receiver_pid = n; // because father sends signal to son
        current = 0;

        signal(SIGUSR1, handle_turn);
        signal(SIGUSR2, handle_victory);

        // father makes the first move
        sleep(1); // to ensure son has set signal before father sends it
        
        int current_tokens;
        lseek(fd, 0, SEEK_SET);
        read(fd, &current_tokens, sizeof(int));
        
        //Father removes tokens
        int taken_tokens = rand()%3+1;
        int new_tokens = current_tokens-taken_tokens;
        
        // for debugging
        printf("I am %d and my pid is %d. I will subtract %d. Now there are %d tokens left\n", current, getpid(), taken_tokens, new_tokens);
        
        if(new_tokens > 0){
            //update file with new tokens
            lseek(fd, 0, SEEK_SET);
            write(fd, &new_tokens, sizeof(int));

            //change turn
            kill(receiver_pid, SIGUSR1);
        }
        else{
            //notify son that he lost
            kill(receiver_pid, SIGUSR2);
        }

        // game loop
        while(1){
            pause();
            //Usamos pause en vez de sleep porque es más eficiente para la CPU. Durante el while, la CPU consume recursos
            //y con sleep hacemos que durante x segundos deje de usar la CPU y por lo tanto, consume menos. 
            //El problema es que si la señal tarda más de esos x segundos consumira recursos de la CPU hasta que llegue la señal.
            //En cambio, con pause nos aseguramos de que no consuma nada hasta que la señal llegue y por lo tanto, es más eficiente.
        }
    }

    wait(NULL); // to avoid zombie son

    //close(fd); // close file 
    return 0;
}
