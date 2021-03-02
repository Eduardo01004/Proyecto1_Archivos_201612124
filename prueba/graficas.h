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
using namespace std;

class Graficas
{
public:
    Graficas();
    void  GraficarMBR_EBR(QString direccion , QString destino, QString extension);
    void GraficarDisk(QString direccion , QString destino, QString extension);
};

#endif // GRAFICAS_H
