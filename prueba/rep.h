#ifndef REP_H
#define REP_H
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

class REP
{
public:
    REP();
    void AutomataRep(QString lexema,QString token,int flag);
    int estadorep = 0;
    int flag_rep = 0;
    int flag_pat = 0;
    int flag_namerep = 0;
    int flag_tipo;

    QString idrep = "";
    QString pathrep = "";
    QString rutarep = "";
};

#endif // REP_H
