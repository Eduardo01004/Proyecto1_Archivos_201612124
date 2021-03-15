#ifndef LISTAORDENADA_H
#define LISTAORDENADA_H
#include "nodoordenado.h"
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

class Listaordenada
{
public:
    Listaordenada( );
    int InsertarOrdenado(int id,QString cadena);
    NodoOrdenado *primero;
    NodoOrdenado *ultimo;
};

#endif // LISTAORDENADA_H
