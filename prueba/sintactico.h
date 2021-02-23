#ifndef SINTACTICO_H
#define SINTACTICO_H
#include <QCoreApplication>
#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include "qdebug.h"
#include "qstring.h"
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include "ejecutar.h"
#include "mkdisk.h"
#include "rmdisk.h"
#include "fdisk.h"
#include "rep.h"
#include "mount.h"

using namespace std;


class Sintactico
{
public:

    Sintactico();
    void Analisis(QString,int);
    int Estados = 0;
    QString token = "";
    QString lexema = "";

    int flag_mkdisk = 0;
    int flag_rmdisk = 0;
    int flag_fdisk = 0;
    int flag_exec = 0;
    int flag_rep = 0;
    int flag_mount = 0;

    Mkdisk *mk = new Mkdisk();
    Rmdisk *rm = new Rmdisk();
    Fdisk *fd = new Fdisk();
    REP *re = new REP();
    Mount *mo = new Mount();




};

#endif // SINTACTICO_H
