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
#include "listamount.h"
#include "graficas.h"
#include "mkfs.h"
#include "login.h"
using namespace std;

class Mount
{
public:
    Mount();
    void AutomataMount(QString lexema, QString token , int flag);
    int BuscarParticionP(MBR mbr,QString nombre);
    int BuscarL(MBR mbr,QString nombre,EBR ebr,QString direccion,FILE *disco);
    void AutomataRep(QString lexema,QString token,int flag);
    void automataUnmount(QString lexema, QString token, int flag);
    USERLOG userlog;
    int estadomount = 0;
    int flag_mount_path = 0;
    int flag_mount_name = 0;
    QString pathmount = "";
    QString namemount = "";

    listamount *lista = new listamount();
    Graficas *gra = new Graficas();
    Mkfs *mks = new Mkfs();


    QString getId(QString path);

//------------ para el parametro rep//
    int estadorep = 0;
    int flag_rep = 0;
    int flag_pat = 0;
    int flag_namerep = 0;
    int flag_tipo;

    QString idrep = "";
    QString pathrep = "";
    QString rutarep = "";

    //para unmount

    int estadounmount = 0;
    int flag_idunmount = 0;
    QString idunmount = "";


    //para mkfs
    void AutomataMkfs(QString lexema,QString token,int flag);
    int estadomkfs = 0;
    int flag_idmkfs = 0;
    int flag_tipomkfs = 0;
    int flag_msmkfs = 0;


    QString idmkfs = "";
    QString tipomkfs = "";
    QString msmkfs = "";

    //paralogin
    void AutomataLogin(QString lexema,QString token,int flag);
    Login *log = new Login();
    int estadologin = 0;
    int flag_usr = 0;
    int flag_pass = 0;
    int flag_idlog = 0;
    int flag_session = 0;

    QString usr = "";
    QString pass = "";
    QString idlog = "";

   //
};

#endif // MOUNT_H
