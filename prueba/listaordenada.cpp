#include "listaordenada.h"

Listaordenada::Listaordenada()
{
    primero = nullptr;
    ultimo = nullptr;
}

int Listaordenada::InsertarOrdenado(int id,QString cadena){
    NodoOrdenado *nuevo = new NodoOrdenado(id,cadena);
    nuevo->id = id;
    nuevo->contenido = cadena;
    if(primero == nullptr){
            primero = nuevo;
            ultimo = nuevo;
            return 1;
        }else{
            if(primero->id > id){
                nuevo->siguiente=primero;
                primero -> anterior = nuevo;
                primero = nuevo;
                return 1;
            }else if(ultimo -> id < id){
                ultimo->siguiente = nuevo;
                nuevo->anterior = ultimo;
                ultimo = nuevo;
                return 1;
            }else{
                NodoOrdenado *aux = primero;
                while(aux!=nullptr){
                    if(aux->id == id) return 0;
                    else if(aux -> id > id) break;
                    else aux = aux -> siguiente;
                }
                NodoOrdenado* temp=aux->anterior;
                nuevo->anterior = temp;
                temp->siguiente = nuevo;
                nuevo->siguiente = aux;
                aux->anterior = nuevo;
                return 1;

            }

        }

}
