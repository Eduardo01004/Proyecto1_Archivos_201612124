#include "nodoordenado.h"

NodoOrdenado::NodoOrdenado(int id, QString contenido)
{
    this->id = id;
    this->contenido = contenido;
    this->siguiente = nullptr;
    this->anterior = nullptr;

}
