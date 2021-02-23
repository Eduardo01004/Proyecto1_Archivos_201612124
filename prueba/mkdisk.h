#ifndef MKDISK_H
#define MKDISK_H
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
#include "main.h"
using namespace std;

class Mkdisk
{
public:
    Mkdisk();
    int estadomkdisk = 0;
    int fit_mkdisk = 0;
    void automataMkdisk(QString,QString,int);
    void CrearDisco(QString);
};

#endif // MKDISK_H
