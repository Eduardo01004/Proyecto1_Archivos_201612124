#ifndef NODOORDENADO_H
#define NODOORDENADO_H
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

class NodoOrdenado
{
public:
    NodoOrdenado(int id,QString contenido);
    int id;
    QString contenido;
    NodoOrdenado *siguiente;
    NodoOrdenado *anterior;
};

#endif // NODOORDENADO_H
