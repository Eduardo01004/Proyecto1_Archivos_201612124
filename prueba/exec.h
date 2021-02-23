#ifndef EXEC_H
#define EXEC_H
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
#include "lexico.h"
#include "sintactico.h"
using namespace std;

class Exec
{
public:
    Exec();
    void Ejectuar(char lexema[400], char token[400], int flag,char adress[400]);
    int estadoexec = 0;

    //Sintactico *sin2 = new Sintactico();
    Lexico *lexi = new Lexico;

};

#endif // EXEC_H
