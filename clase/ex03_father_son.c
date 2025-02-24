#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int f) {
    printf("I am %d, and I received a signal\n", getpid());
}

int main(int argc, char* argv[]) {
    signal(SIGINT, handler);
    printf("My pid is %d\n", getpid());
    int n = fork();
    if (n == 0) {
        printf("I am the son, and I am a bit lazy\n");
        int ret_sleep = sleep(5);
        printf("I am the son, and I woke up from the nap. Value is %d\n", ret_sleep);

    }
    else {
        printf("I am the father, and I am a bit lazy\n");
        sleep(1);
        printf("I am the father, and I woke up from the nap\n");
        // kill does not kill a process, just sends a signal to another process
        // (Which might or might not kill the process)
        kill(n, SIGINT); // This sends a signal to another process
    }
    wait(NULL);

}