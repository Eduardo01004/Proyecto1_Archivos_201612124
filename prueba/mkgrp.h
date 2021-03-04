#ifndef MKGRP_H
#define MKGRP_H
#include <QCoreApplication>
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
using namespace std;

class Mkgrp
{
public:
    Mkgrp();
    void BuscarGrupo(QString name, int iniciosuper);
    int llenar1(FILE *disco,superBloque super,bloqueArchivo archivo,inodeTable inodouser,int posicion, char cadena[400],int tam_actual);
    int llenar2(FILE *disco,superBloque super,bloqueArchivo archivo,inodeTable inodouser,int posicion, char fase2[400],int tam_actual,int iniciosuper,char fit);
    int firsFit(FILE *disco,int inicio);
};

#endif // MKGRP_H
