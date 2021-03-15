#ifndef JOURNAL_H
#define JOURNAL_H
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
#include "mkgrp.h"
using namespace std;

class Journal
{
public:
    Journal();
    void GuardarJournaling(int accion,int tipo,int permisos,QString nombre,QString contenido,QString usuario, QString direccion,int inicio_journal);
};

#endif // JOURNAL_H
