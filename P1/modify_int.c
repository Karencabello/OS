#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int modify(char encoded, int pos, char value){
    //encoded --> whether the file is encoded as text (1), or in binary (0)
    //pos --> the  position  of  the  integer  to modify
    //value --> the new value to be written

    //Mirar si es binerio o texto
    if (encoded == '.dat'){  //Mirar mejor esto
        int fd = open(encoded, O_WRONLY | O_CREAT, 0644);
        //buscar donde escribir
        lseek(fd, pos * 4, SEEK_SET);
        //escribir
        int ok = write(fd, &value, strlen(value));

    }
    else if(encoded == '.txt'){
        //mirar tamaño enteros 

    }
    else printf("ERROR"); return 0; // MIRAR AIXÒ ABANS D'ENTREGAR


}