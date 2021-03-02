#ifndef LISTAMOUNT_H
#define LISTAMOUNT_H
#include <nodolist.h>
#include <iostream>
using namespace std;

class listamount
{
public:
    listamount();
    nodolist *primero;
    nodolist *ultimo;
    void Insertar(nodolist *nodo);
    void partP(QString,QString);
    int Delete(QString name);
    int Buscar(QString nombre, QString path);
    void Mostrar();

    nodolist* BMount(QString nombre);
    QString Direccion(QString id);
    QString Extension(QString );
    void A(QString id);

};

#endif // LISTAMOUNT_H
