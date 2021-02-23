#ifndef MOUNT_H
#define MOUNT_H
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

class Mount
{
public:
    Mount();
    void AutomataMount(QString lexema, QString token , int flag);

    int estadomount = 0;
    int flag_mount_path = 0;
    int flag_mount_name = 0;

    QString pathmount = "";
    QString namemount = "";
};

#endif // MOUNT_H
