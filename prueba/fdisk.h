#ifndef FDISK_H
#define FDISK_H
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
class Fdisk
{
public:
    Fdisk();
    void AutomataFdisk(QString,QString,int);
    bool BuscarParticion(QString nombre,QString direccion);
    void CrearParticionPrimaria(QString path,char unit,int size,QString type,QString);
    void CrearExtendida(QString path,char unit,int size,QString type,QString);
    void CrearLogica(QString path,char unit,int size,QString type,QString name,QString fit);
    int Size(char,int);
    bool Existe_Extendida(MBR m,int);
    void LLenarEBR(EBR ebr,char fit,QString name,int inicio, int next,int tam);
    int pos_Extendida(MBR m);

    int estadoFdisk = 0;
    int deletefdisk = 0;
    int flag_size = 0;
    int flag_unit = 0;
    int pathfdisk = 0;

    int tipo = 0;
    int ajuste = 0;
    int flag_name = 0;
    char unitmkdisk = 0;
    QString pathfd = "";
    QString typefdisk = "";
    QString  fitfdisk = "";
    QString tipodelete = "";
    QString name = "";
    int tamanio = 0;
    int tampart = 0;
};

#endif // FDISK_H
