#include "sintactico.h"
#include <QStringList>


Sintactico::Sintactico()
{

}
Ejecutar *el = new Ejecutar();

void Sintactico::Analisis(QString direccion,int flag){
    FILE *archivo;
    archivo=fopen(direccion.toStdString().c_str(),"r");
    char line[400];
    QStringList linea;
    while (fscanf(archivo, "%[^\n]%*c",line) == 1) {
        linea = QString(line).split(",");
        token = linea[0];
        lexema = linea[1];
        if (lexema == "mkdisk"){
            flag_mkdisk = 1;
            flag_rmdisk = 0;
            flag_exec = 0;
            flag_fdisk = 0;
            flag_rep = 0;
            flag_mount = 0;
            flag_unmount = 0;
            flag_login = 0;
        }else if (lexema == "rmdisk"){
            flag_mkdisk = 0;
            flag_rmdisk = 1;
            flag_exec = 0;
            flag_fdisk = 0;
            flag_rep = 0;
            flag_mount = 0;
            flag_unmount = 0;
            flag_login = 0;
        }else if (lexema == "fdisk"){
            flag_mkdisk = 0;
            flag_rmdisk = 0;
            flag_fdisk = 1;
            flag_exec = 0;
            flag_rep = 0;
            flag_mount = 0;
            flag_unmount = 0;
            flag_login = 0;
        }else if (lexema == "exec"){
            flag_mkdisk = 0;
            flag_rmdisk = 0;
            flag_fdisk = 0;
            flag_rep = 0;
            flag_exec = 1;
            flag_mount = 0;
            flag_unmount = 0;
            flag_login = 0;
        }else if (lexema == "rep"){
            flag_mkdisk = 0;
            flag_rmdisk = 0;
            flag_fdisk = 0;
            flag_exec = 0;
            flag_rep = 1;
            flag_mount = 0;
            flag_unmount = 0;
            flag_login = 0;
        }else if (lexema == "mount"){
            flag_mkdisk = 0;
            flag_rmdisk = 0;
            flag_fdisk = 0;
            flag_exec = 0;
            flag_rep = 0;
            flag_mount = 1;
            flag_unmount = 0;
            flag_login = 0;
        }else if (lexema == "unmount"){
            flag_mkdisk = 0;
            flag_rmdisk = 0;
            flag_fdisk = 0;
            flag_exec = 0;
            flag_rep = 0;
            flag_mount = 0;
            flag_unmount = 1;

        }else if(lexema == "mkfs"){
            flag_mkdisk = 0;
            flag_rmdisk = 0;
            flag_fdisk = 0;
            flag_exec = 0;
            flag_rep = 0;
            flag_mount = 0;
            flag_unmount = 0;
            flag_mkfs = 1;
            flag_login = 0;

        }else if (lexema == "login"){
            flag_mkdisk = 0;
            flag_rmdisk = 0;
            flag_fdisk = 0;
            flag_exec = 0;
            flag_rep = 0;
            flag_mount = 0;
            flag_unmount = 0;
            flag_mkfs = 0;
            flag_login = 1;

        }

        if (token == "exit"){
            flag = 0;
            cout << " Saliendo de la aplicacion " << endl;
            exit(1);
        }else if (flag_mkdisk) mk->automataMkdisk(lexema,token,flag_mkdisk);
        else if(flag_rmdisk) rm->automataRmdisk(lexema,token,flag_rmdisk);
        else if (flag_fdisk) fd->AutomataFdisk(lexema,token,flag_fdisk);
        else if (flag_exec){
            char lex[400] = {0};
            for (int i = 0; i < lexema.length(); i++){
                lex[i] = lexema.toStdString()[i];
            }
            char tok[400] = {0};
            for (int i = 0; i < token.length(); i++){
                tok[i] = token.toStdString()[i];
            }
            char dir[400] = {0};
            for (int i = 0; i < direccion.length()+1; i++){
                dir[i] = direccion.toStdString()[i];
            }
            el->Ejec(lex,tok,flag_exec);
        }else if (flag_mount) mo->AutomataMount(lexema,token,flag_mount);
        else if (flag_rep) mo->AutomataRep(lexema,token,flag_rep);
        else if (flag_unmount) mo->automataUnmount(lexema,token,flag_unmount);
        else if (flag_mkfs) mo->AutomataMkfs(lexema,token,flag_mkfs);
        else if (flag_login) mo->AutomataLogin(lexema,token,flag_login);
        else{
            cout << " NO se reconoce el comando 1:" << token.toStdString() << endl;
            break;
        }
    }

}
