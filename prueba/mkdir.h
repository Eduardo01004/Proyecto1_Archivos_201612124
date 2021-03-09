#ifndef MKDIR_H
#define MKDIR_H
#include <QCoreApplication>
#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include "qdebug.h"
#include "qstring.h"
#include <stdlib.h>
#include <ctime>
#include <math.h>
#include <libgen.h>
#include <dirent.h>
#include <QStringList>
#include "Structs.h"
#include "mkgrp.h"
using namespace std;

class Mkdir
{
public:
    Mkdir();
    QString RetornarRuta(QString ruta);
    int BuscarCoA(FILE *disco,char *path, int inicio,int *numeracion);
    int CrearCarpeta(FILE *DiscoEnUSo,char fit,int indicador,int inicio,char *path,int flag_P);
    Mkgrp mkg;
    int firsFit(FILE *disco,int inicio);
    int firsFitInodo(FILE *disco,int inicio);
};

#endif // MKDIR_H
