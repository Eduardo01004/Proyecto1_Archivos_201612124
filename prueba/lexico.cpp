#include "lexico.h"

#include <QCoreApplication>
#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include "qdebug.h"
#include "qstring.h"
#include <ctime>
#include <stdio.h>
#include <stdlib.h>

const char* palabrasReservadas(char palabra[400]);

void Lexico:: Concatenar(char *p,QString direccion){
    char palabra[400];
    int espacio = 0;
    int recien = 0;
    int contador = 0;
    int salto = 0;
    int comillas = 0;
    int comentario = 0;
    int simbolo = 0;

    bool guardar = false;
    bool numero = false;
    bool letra = false;
    memset(palabra, 0, sizeof(palabra));
    FILE *archivo;
    archivo = fopen(direccion.toStdString().c_str(),"w");
    while (*p && *p != '\0' ){
            if (*p == 32){ // espacio en blanco
                if (espacio == 0 && recien == 0){
                    if (comillas == 1 || comentario == 1){
                        palabra[contador] = *p;
                        contador++;
                        guardar = true;
                        numero = false;
                    }else {
                        if (numero == true) {
                            fprintf(archivo,"%s,%s\n",palabra,"numero");
                        }
                       else {
                           fprintf( archivo,"%s,%s\n", palabra , palabrasReservadas(palabra) );
                       }
                        memset(palabra, 0, sizeof(palabra));
                        palabra[0] = '\0';
                        contador = 0;
                        letra = false;
                    }
                    espacio = 1;
                    recien = 0;
                    salto = 1;
                }
            }else if (*p == 61){
                if (comentario == 0){
                    palabra[contador] = *p;
                    contador++;
                    fprintf(archivo,"%s,%s\n",palabra,palabrasReservadas(palabra));
                    memset(palabra, 0, sizeof(palabra));
                    simbolo = 0;
                    letra = 0;
                    guardar = false;
                    palabra[0] = '\0';
                    contador = 0;
                    espacio = 0;
                    recien = 1;
                }

            }else if (*p > 47 && *p < 58){
                guardar = true ;
                if (comentario == 1 || comillas == 1 || letra == true) numero == false;
                else numero = true;
                palabra[contador] = *p;
                contador++;
                recien = 0;
                }else if (*p == 10 || *p == 8 || *p == 9 || *p == 11 || *p == 13){

                }else {
                    if (*p == 34){
                        if (comillas == 0) comillas = 1;
                        else comillas = 0;
                    }else {
                        if (*p == 35) comentario = 1;
                        if (*p != 32 && *p != 10 && *p != 9){
                            palabra[contador] = *p;
                            contador++;
                        }
                    }
                    guardar = true;
                    numero = false;
                    letra = true;
                    espacio = 0;
                    recien = 0;
                    salto = 0;
                }
            p++;
        }
        if (guardar == true ){

            if (numero == true){
                fprintf(archivo,"%s,%s\n",palabra,"numero");
                memset(palabra, 0, sizeof(palabra));
                contador = 0;
                guardar = false;
            }else if (comentario == 1){
                fprintf(archivo,"%s,%s\n",palabra,"comentario");
                memset(palabra, 0, sizeof(palabra));
                contador = 0;
                guardar = false;
            }else if (espacio != 1){
                fprintf(archivo,"%s,%s\n",palabra,palabrasReservadas(palabra));
                memset(palabra, 0, sizeof(palabra));
                contador = 0;
                guardar = false;

            }else if (salto != 1){
                fprintf(archivo,"%s,%s\n",palabra,palabrasReservadas(palabra));
                memset(palabra, 0, sizeof(palabra));
                contador = 0;
                guardar = false;
            }

            fprintf(archivo,"%s,%s\n","finInstruccion","finInstruccion");
        }
        fclose(archivo);
}



const char* palabrasReservadas(char palabra[400]){
    if(strcmp(palabra, "mkdisk") == 0 || strcmp(palabra, "Mkdisk") == 0 || strcmp(palabra, "mkDisk") == 0 ){
        return "mkdisk";
    }else if(strcmp(palabra, "-size=") == 0 || strcmp(palabra, "-Size=") == 0 ){
        return "size";
    }else if(strcmp(palabra, "-u=") == 0 || strcmp(palabra, "-U=") == 0 ){
        return "unit";
    }else if(strcmp(palabra, "-path=") == 0 || strcmp(palabra, "-Path=") == 0 ){
        return "path";
    }else if (strcmp(palabra, "-f=") == 0 || strcmp(palabra, "-F=") == 0 ) return "fit";
    else if(strcmp(palabra, "rmdisk") == 0 || strcmp(palabra, "Rmdisk") == 0  || strcmp(palabra, "rmDisk") == 0 || strcmp(palabra, "RMDISk") == 0 ){
        return "rmdisk";
    }else if(strcmp(palabra, "fdisk") == 0 || strcmp(palabra, "Fdisk") == 0  || strcmp(palabra, "fDisk") == 0 ){
        return "fdisk";
    }else if(strcmp(palabra, "-type=") == 0 || strcmp(palabra, "-Type=") == 0 ){
        return "type";
    }else if(strcmp(palabra, "-delete=") == 0 || strcmp(palabra, "-Delete=") == 0 ){
        return "delete";
    }else if(strcmp(palabra, "-name=") == 0 || strcmp(palabra, "-Name=") == 0 ){
        return "name";
    }else if(strcmp(palabra, "add") == 0 || strcmp(palabra, "Add") == 0 ){
        return "add";
    }else if(strcmp(palabra, "mount") == 0 || strcmp(palabra, "Mount") == 0 ){
        return "mount";
    }else if(strcmp(palabra, "unmount") == 0 || strcmp(palabra, "Unmount") == 0 ){
        return "unmount";
    }else if(strcmp(palabra, "-id=") == 0 || strcmp(palabra, "-Id=") == 0 ){
        return "id";
    }else if(strcmp(palabra, "rep") == 0 || strcmp(palabra, "Rep") == 0 ){
        return "rep";
    }else if(strcmp(palabra, "Login") == 0 || strcmp(palabra, "login") == 0 ){
        return "login";
    }else if(strcmp(palabra, "-pwd=") == 0 || strcmp(palabra, "-Pwd=") == 0 ){
        return "pwd";
    }else if(strcmp(palabra, "-usr=") == 0 || strcmp(palabra, "-Usr=") == 0 ){
        return "usr";
    }else if(strcmp(palabra, "exec") == 0 || strcmp(palabra, "Exec") == 0 ){
        return "exec";
    }else if(strcmp(palabra, "logout") == 0 || strcmp(palabra, "Logout") == 0 ){
        return "logout";
    }else if(strcmp(palabra, "mkfs") == 0 || strcmp(palabra, "Mkfs") == 0 ){
        return "mkfs";
    }else if(strcmp(palabra, "mkgrp") == 0 || strcmp(palabra, "Mkgrp") == 0 ){
        return "mkgrp";
    }else if(strcmp(palabra, "-fs=") == 0 || strcmp(palabra, "-Fs=") == 0 ){
        return "fs";
    }else if(strcmp(palabra, "rmgrp") == 0 || strcmp(palabra, "Rmgrp") == 0 ){
        return "rmgrp";
    }else if(strcmp(palabra, "mkusr") == 0 || strcmp(palabra, "Mkusr") == 0 ){
        return "mkusr";
    }else if(strcmp(palabra, "-grp=") == 0 || strcmp(palabra, "-Grp=") == 0 ){
        return "grp";
    }else if(strcmp(palabra, "rmusr") == 0 || strcmp(palabra, "Rmusr") == 0 ){
        return "rmusr";
    }else if(strcmp(palabra, "mkdir") == 0 || strcmp(palabra, "Mkdir") == 0 ){
        return "mkdir";
    }else if(strcmp(palabra, "-p") == 0 || strcmp(palabra, "-P") == 0 ){
        return "p";
    }else if(strcmp(palabra, "-r") == 0 || strcmp(palabra, "-R") == 0 ){
        return "r";
    }else if(strcmp(palabra, "mkfile") == 0 || strcmp(palabra, "Mkfile") == 0 ){
        return "mkfile";
    }else if(strcmp(palabra, "-cont=") == 0 || strcmp(palabra, "-Cont=") == 0 ){
        return "cont";
    }else if(strcmp(palabra, "-ruta=") == 0 || strcmp(palabra, "-Ruta=") == 0 ){
        return "ruta";
    }else if(strcmp(palabra, "Loss") == 0 || strcmp(palabra, "loss") == 0 ){
        return "loss";
    }else if(strcmp(palabra, "Recovery") == 0 || strcmp(palabra, "recovery") == 0 ){
        return "recovery";
    }else if(strcmp(palabra, "convert") == 0 || strcmp(palabra, "Convert") == 0 ){
        return "convert";
    }else if(strcmp(palabra, "chmod") == 0 || strcmp(palabra, "Chmod") == 0 ){
        return "chmod";
    }else if(strcmp(palabra, "-r") == 0 || strcmp(palabra, "-R") == 0 ){
        return "r";
    }else if(strcmp(palabra, "-ugo->") == 0 || strcmp(palabra, "-Ugo->") == 0 ){
        return "ugo";
    }else if(strcmp(palabra, "cat") == 0 || strcmp(palabra, "Cat") == 0 ){
        return "cat";
    }else if(strcmp(palabra, "mv") == 0 || strcmp(palabra, "MV") == 0 ){
        return "mv";
    }else if(strcmp(palabra, "-dest=") == 0 || strcmp(palabra, "-Dest=") == 0 ){
        return "dest";
    }else if(strcmp(palabra, "rem") == 0 || strcmp(palabra, "Rem") == 0 ){
        return "rem";
    }else if (strcmp(palabra, "pause") == 0 || strcmp(palabra, "Pause") == 0 ){
        return "pause";
    }
    else{
        return "palabra";
    }

}
