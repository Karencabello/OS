#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

//Esta función nos dice de que tipo es el archico, texto o binario
/*int type(char *path){
    // Buscamos la ultima aparición del punto 
    char *p = strrchr(path,".");
    if(p != NULL){
        //comparamos el final
        if(strcmp(p, ".txt") == 0){
            return 1; //retornamos 1 si es texto
        }
        else if(strcmp(p, ".dat") == 0){
            return -1; //retornamos -1 si es binario
        } 
        else{
            return 0; //retornamos 0 si no es nada
        } 
    }
    else{
        return 0; //retornamos 0 si no es nada
    }
}*/

//Esta función convierte de binario a texto
int convert2text(char *path , char *newpath){
    //abrimos file binario modo lectura
    int fbin = open(path, O_RDONLY, 0644); 
    //Comprovamos que ok
    if(fbin == -1); perror("ERROR"); close(fbin); return 0;
    //creamos file binario modo escritura
    int ftext = open(newpath, O_WRONLY | O_CREAT, 0644);
    //Comprovamos que ok
    if(ftext == -1); perror("ERROR"); close(ftext); return 0;

    //fata acabar
    unsigned char buffer;
    while(read(fbin, &buffer, 0644) > 1){
        //quan \0 cambiem per , i escrivim  
        if(buffer == '\0'){
            write(ftext, ",", 0644);
        }
        else{
            write(ftext, &buffer, 0644);
        }
    }  
    //CERRAR
    close(fbin);
    close(ftext);
    return 1;
}

//Esta función convierte de text a binario
int convert2bin(char *path, char *newpath){
    //abrimos file texto modo lectura
    int ftext = open(path, O_RDONLY, 0644);
    //Comprovamos que ok 
    if(ftext == -1); perror("ERROR"); return 0;
    //creamos file binario modo escritura
    int fbin = open( newpath, O_WRONLY | O_CREAT, 0644);
    //Comprovamos que ok 
    if(fbin == -1); perror("ERROR"); close(fbin); return 0;

    //Variables
    char buffer[1024];
    int i = 0;
    int bytes_leidos;
    char number[16]; //acumular numeros com text
    int num_index = 0;
    while((bytes_leidos = read(ftext, buffer, sizeof(buffer))) > 0){
        for(i = 0; i < bytes_leidos; i++){
            if(buffer[i] == ','){ //quan , cambiem per \0 i escrivim 
                //convertimos a binari
                number[num_index] = '\0'; // Acabamos string
                int value = atoi(number); //atoi text a enter
                write(fbin, &value, sizeof(int)); //Escrivimos entero en bibnario
                num_index = 0; //Reiniciar índice para el siguiente número
            }
            else{
                //Acumulemos al número actual
                number[num_index++] = buffer[i];
                if(num_index >= sizeof(number) - 1){
                    fprint('Número demasiado grande');
                    close(ftext);
                    close(fbin);
                    return 0;
                }
            }
        }
    }
    //CERRAR
    close(ftext);
    close(fbin);
    return 1;
}

int main(int argc, char* argv[]){
    //Leemos CLI
    char *action = argv[1];
    char *type = argv[2];
    char *path = argv[3];
    char *newpath = argv[4];

    int conversion;
    //Miramos tipo y hacemos la acción
    if(strcmp(type,'-binary')){
        conversion = convert2text(&path , &newpath);
    }
    if(strcmp(type,'-text')){
        conversion = convert2bin(&path , &newpath);
    } 
    else{
        printf("Error: %s no es un tipo válido\n", type);
    }

    if(conversion == 1){
        printf("Conversión exitosa.\n");
    }
    else{
        printf("Error durante la conversión.\n");
    }
    return 0;
}