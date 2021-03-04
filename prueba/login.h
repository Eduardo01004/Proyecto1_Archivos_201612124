#ifndef LOGIN_H
#define LOGIN_H
#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include "qdebug.h"
#include "qstring.h"
#include <stdlib.h>
#include <ctime>
#include <dirent.h>
#include <QStringList>
#include "Structs.h"
#include "mkgrp.h"
using namespace std;

class Login
{
public:
    Login();
    void Loguear(int index,FILE *disco,MBR auxmbr,QString direccion,QString usr,QString pass,USERLOG userlog,int index2,int tipo,EBR ebr);
    QString retornarContent(QString direccion,int inicioI,int InicioB);
    int BuscarG(QString path, int inicio);
    int CrearGrupo(FILE *disco,char contenido[64],QString direccion);
    int retornarinodo(superBloque super, FILE*disco);
    void marcar(superBloque super,FILE *disco);
    int retornartam(int numero);
    char fit;
    int inicioSuper;
    QString path;
    int id_user;
    int id_grp;
    Mkgrp gr ;
};

#endif // LOGIN_H
