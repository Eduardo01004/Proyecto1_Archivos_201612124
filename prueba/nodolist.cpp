#include "nodolist.h"

nodolist::nodolist(QString direccion,QString name, char letra, int tipo,QString id)
{
    this->direccion = direccion;
    this->name = name;
    this->letra = letra;
    this->tipo = tipo;
    this->id = id;
    this->siguiente = nullptr;
    this->anterior = nullptr;
}
