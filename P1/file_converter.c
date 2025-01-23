#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

// Usage: file_converter -binary|-text inputfile outputfile

// Esta función convierte de binario a texto
int convert2text(char *path , char *newpath){
    //abrimos file binario modo lectura
    int fbin = open(path, O_RDONLY, 0644); 
    //Comprobamos que ok
    if(fbin == -1){ 
        perror("ERROR"); 
        close(fbin); 
        return 0;
    }
    // Abrimos file tipo txt en modo escritura
    int ftext = open(newpath, O_WRONLY | O_CREAT, 0644);
    //Comprobamos que ok
    if(ftext == -1){
        perror("ERROR"); 
        close(ftext); 
        return 0;
    }
    
    off_t fileSize = lseek(fbin, 0, SEEK_END);  // Calculamos el número de bytes que ocupa el ficherp
    off_t numbers = fileSize/sizeof(int);       // Calculamos el total de entero que incluye el fichero
    //printf("filesize %ld\n", fileSize);

    char buffer[12];      // Guardará la representación en cadena del entero (asumimos que no tendrá más de 11 dígitos -pos 12: \0-)
    char comma = ',';
    int num;              // Guardará el entero que leamos por iteración

    lseek(fbin, 0, SEEK_SET); // Nos posicionamos al principio del fichero
    for(off_t i=0; i<numbers; i++) {
        read(fbin, &num, sizeof(int)); // Leemos los 4 bytes que representan al entero de la iteración del fichero en binario
        sprintf(buffer, "%d", num);  // Convierte el número a cadena
        write(ftext, buffer, strlen(buffer)); // Escribimos el número en texto en el documento txt
        if(i != numbers -1){
            write(ftext, &comma, sizeof(comma)); // Si no estamos al final del documento, añadimos una coma
        }

    }

/*
    unsigned char buffer;
    char comma = ',';
    char num[12]; 
    while(read(fbin, &buffer, sizeof(buffer)) == sizeof(buffer)){
        //quan \0 cambiem per , i escrivim  
        if(buffer == '\0'){
            write(ftext, &comma, sizeof(comma));
        }
        else{
            sprintf(num, "%u", buffer);  // Convierte el número a cadena
            write(ftext, num, strlen(num));
        }
    }  
*/

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
    if(ftext == -1){ 
        perror("ERROR"); 
        return 0;
    }
    //creamos file binario modo escritura
    int fbin = open( newpath, O_WRONLY | O_CREAT, 0644);
    //Comprovamos que ok 
    if(fbin == -1){ 
        perror("ERROR"); 
        close(fbin); 
        return 0;
    }

    //Variables
    char buffer[1024]; // Leer archivo de texto
    char number[16]; //acumular numeros como texto
    int bytes_leidos; //Bytes leidos del archivo
    int num_index = 0; //indice dentro de number
    int value;  

    while((bytes_leidos = read(ftext, buffer, sizeof(buffer))) > 0){
        for(int i = 0; i < bytes_leidos; i++){
            if(buffer[i] == ','){ //quan , canviem per \0 i escrivim 
                if(num_index > 0){
                    number[num_index] = '\0'; //terminamos cadena
                    value = atoi(number); //convertimos a entero
                    write(fbin, &value, sizeof(value)); //escribimos entero en binario
                    num_index = 0; //reiniciamos para el proximo numero
                }
            }
            else if(buffer[i] >= '0' && buffer[i] <= '9'){
                //Acumulamos los caracteres del numero al valor
                if (num_index < (int)sizeof(number) - 1) {
                number[num_index++] = buffer[i];
                }else{
                    //Si el numero es mayor que 1024, error
                    perror("ERROR"); 
                    close(fbin);
                    close(ftext); 
                    return 0; 

                }
            }    
        }
    }   
    // Procesar el último número (si el archivo no termina en coma)
    if (num_index > 0) {
        number[num_index] = '\0';
        value = atoi(number);
        write(fbin, &value, sizeof(value));
    }
    //CERRAR
    close(ftext);
    close(fbin);
    return 1;
    
}

int main(int argc, char* argv[]){
    //Leemos CLI
    char *type = argv[1];
    char *path = argv[2];
    char *newpath = argv[3];

    int conversion;
    //Miramos tipo y hacemos la acción
    if(strcmp(type,"-binary") == 0){
        conversion = convert2text(path , newpath);
    }
    else if(strcmp(type,"-text") == 0){
        conversion = convert2bin(path , newpath);
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
