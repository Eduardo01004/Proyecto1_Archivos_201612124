#ifndef PERDIDA_H
#define PERDIDA_H
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

class Perdida
{
public:
    Perdida();
    void MetodoLoss(int inicio, int final , QString direccion);
};

#endif // PERDIDA_H
