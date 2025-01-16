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
    //abrimos file binario
    int fbin = open(path, O_RDONLY, 0644); 
    //Comprovamos que ok
    if(fbin == -1); printf("ERROR"); close(fbin); return 0;
    //creamos file binario
    int ftext = open(newpath, O_WRONLY | O_CREAT, 0644);
    //Comprovamos que ok
    if(ftext == -1); printf("ERROR"); close(ftext); return 0;

    //quan \0 cambiem per , i escrivim  
    //CERRAR
}

//Esta función convierte de text a binario
int convert2bin(char *path, char *newpath){
    //abrimos file texto
    int ftext = open(path, O_RDONLY, 0644);
    //Comprovamos que ok 
    if(ftext == -1); printf("ERROR"); return 0;
    //creamos file binario
    int fbin = open( newpath, O_WRONLY | O_CREAT, 0644);
    //Comprovamos que ok 
    if(fbin == -1); printf("ERROR"); close(fbin); return 0;

    //Variables
    char buffer[1024];
    int bytes_leidos;

    //

    //atoi text a enter
    //quan , cambiem per \0 i escrivim 
    //CERRAR
}

int main(int argc, char* argv[]){
    //Leemos CLI
    char *action = argv[1];
    char *type = argv[2];
    char *path = argv[3];
    char *newpath = argv[4];

    int conversion;
    //Miramos tipo y hacemos la acción
    if(type == '-binary'); conversion = convert2text(&path , &newpath);
    if(type == '-text'); conversion = convert2bin(&path , &newpath);
}