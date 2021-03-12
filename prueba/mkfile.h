#ifndef MKFILE_H
#define MKFILE_H
#include <QCoreApplication>
#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include "qdebug.h"
#include "qstring.h"
#include <stdlib.h>
#include <ctime>
#include <math.h>
#include <libgen.h>
#include <dirent.h>
#include <QStringList>
#include "Structs.h"
#include "mkgrp.h"
#include "mkdir.h"
using namespace std;

class mkfile
{
public:
    mkfile();
    int CrearArchivo(FILE *DiscoEnUso,char *path,char fit,bool indicadorp,int inicio_particion,int size,QString cont,bool flag_root);
    Mkdir dirm;
};

#endif // MKFILE_H
