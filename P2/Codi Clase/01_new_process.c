#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("My pid is %d \n", getpid());
    int n = fork(); // returns something
    printf("My value is %d, and my pid is %d \n", n, getpid());
}