#ifndef RMDISK_H
#define RMDISK_H
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


class Rmdisk
{
public:
    Rmdisk();
    int estadormdisk = 0;
    void automataRmdisk(QString,QString,int);
};

#endif // RMDISK_H
