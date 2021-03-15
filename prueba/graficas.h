#ifndef GRAFICAS_H
#define GRAFICAS_H
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
#include "mkdir.h"
using namespace std;

class Graficas
{
public:
    Graficas();
    void  GraficarMBR_EBR(QString direccion , QString destino, QString extension);
    void GraficarDisk(QString direccion , QString destino, QString extension);
    void Inode(QString direccion, QString destino, QString extension,int bm_inodeS,int s_inodecount,int inodestart,int inodesize);
    void Bloques(QString direccion, QString destino, QString extension,int biS,int iS,int biE,int start_super);
    void CrearReporte(int inicio,int final,QString direccion,QString path);
    void graficarSuper(QString direccion, QString destino, QString extension, int start_super);
    void Graficar_Tree(QString direccion, QString destino, QString ext,int start );
    void reporteFile(QString path,QString destino,char *pa_path,int inicio);
    void reporteLS(QString path,int inicio,QString extension,QString destino,int usuario,int grupo,char*pa_path);
    void reportarJournaling(QString destino,QString direccion,int inicio,int inicio_journal);
    Mkdir dirm;
};

#endif // GRAFICAS_H
