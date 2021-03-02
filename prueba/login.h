#ifndef LOGIN_H
#define LOGIN_H
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

class Login
{
public:
    Login();
    void Loguear(int index,FILE *disco,MBR auxmbr,QString direccion,QString usr,QString pass,USERLOG userlog,int index2,int tipo,EBR ebr);
    QString retornarContent(QString direccion,int inicioI,int InicioB);
};

#endif // LOGIN_H
