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
#include "mkfs.h"


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
    int flag_unmount = 0;
    int flag_mkfs = 0;
    int flag_login = 0;
    int flag_mkgrp = 0;
    int flag_mkusr = 0;
    int flag_mkdir = 0;

    Mkdisk *mk = new Mkdisk();
    Rmdisk *rm = new Rmdisk();
    Fdisk *fd = new Fdisk();
    Mount *mo = new Mount();
    REP *re = new REP();
    Mkfs *mkf = new Mkfs();



};

#endif // SINTACTICO_H
