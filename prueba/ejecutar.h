#ifndef EJECUTAR_H
#define EJECUTAR_H
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
#include "sintactico.h"
#include "Structs.h"
#include "lexico.h"

using namespace std;

class Ejecutar
{
public:
    Ejecutar();
    void Ejec(char lexema[400], char token[400], int flag);
     void Concatenar2(char *p,QString direccion);
    int estadoexec = 0;
    Lexico *lexi = new Lexico();
     char adress[400];
};

#endif // EJECUTAR_H
