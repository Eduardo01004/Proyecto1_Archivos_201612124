#ifndef NODOLIST_H
#define NODOLIST_H
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

class nodolist
{
public:
    nodolist(QString direccion,QString name, char letra, int tipo,QString id);
    QString direccion;
    QString name;
    char letra;
    int tipo;
    QString id;
    nodolist *siguiente;
    nodolist *anterior;
};

#endif // NODOLIST_H
