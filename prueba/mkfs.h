#ifndef MKFS_H
#define MKFS_H
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
#include "nodolist.h"
#include "listamount.h"
#include <math.h>
using namespace std;

class Mkfs
{
public:
    Mkfs();
    void AutomataMkfs(QString lexema,QString token,int flag);
    int Calcular_estructuras(int tamano,QString tipo);
    void FormatearEXT2(int inicio,int fin,int tamano,QString direccion);
    void FormatearEXT3(int inicio,int fin,int tamano,QString direccion);
    int estadomkfs = 0;
    int flag_idmkfs = 0;
    int flag_tipomkfs = 0;
    int flag_msmkfs = 0;


    QString idmkfs = "";
    QString tipomkfs = "";
    QString msmkfs = "";
    listamount *los = new listamount();


};

#endif // MKFS_H
