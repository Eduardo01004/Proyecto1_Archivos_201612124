#include "listamount.h"

listamount::listamount()
{
    primero = nullptr;
    ultimo = nullptr;

}

void listamount:: Insertar(nodolist *nodo){
    if (primero == nullptr){
        primero = ultimo = nodo;
    }else{
        ultimo->siguiente = nodo;
        nodo->anterior = ultimo;
        ultimo = nodo;
    }

}

void listamount::partP(QString direccion , QString nombre){
    FILE *file;
    cout << "caca" <<endl;
    if (file = fopen(direccion.toStdString().c_str(),"rb+")){
        cout << "encontro dicos" << endl;
        MBR mbr;
        fseek(file,0,SEEK_SET);
        fread(&mbr, sizeof(MBR),1,file);
        for (int i = 0; i < 4; i++){
            if (strcmp(mbr.mbr_partition[i].part_name,nombre.toStdString().c_str()) == 0){
                cout << "entro aqui" << endl;
                mbr.mbr_partition[i].part_status = '0';
                fseek(file,0,SEEK_SET);
                fwrite(&mbr,sizeof(MBR),1,file);
                fclose(file);
                return;

            }
        }
    }
}

int listamount:: Delete(QString name){
   nodolist *aux = primero;
   if (name == aux->id){
       primero = aux->siguiente;
       free(aux);
       //partP(aux->direccion,aux->name);
       return 1;
   }else {
       aux = ultimo;
       if (name == aux->id){
           ultimo = aux->anterior;
           ultimo->siguiente = nullptr;
           free(aux);
           //partP(aux->direccion,aux->name);
           return 1;
       }
       else{
           aux = primero;
           while (aux != nullptr){
               if (name == aux->id){
                    nodolist *nodo = aux->anterior;
                    nodo->siguiente = aux->siguiente;
                    aux->siguiente->anterior = nodo;
                    free(aux);
                    //partP(aux->direccion,aux->name);
                    return 1;
               }
               aux = aux->siguiente;

           }
       }
   }
   return 0;

}



nodolist* listamount::BMount(QString nombre){
    nodolist *aux = primero;
    if (aux != nullptr){
        while(aux != nullptr){
            if (aux->id == nombre){
                return aux;
            }
            aux = aux->siguiente;
        }
    }
    return nullptr;

}

QString listamount::Direccion(QString id){
    nodolist *aux = primero;
    while (aux != nullptr) {
        if (id == aux->id){
            return aux->direccion;
        }
        aux = aux->siguiente;
    }
    return "";
}

QString listamount::Extension(QString dato){
    string valor = dato.toStdString();
        string punto = ".";
        size_t pos = 0;
        while((pos = valor.find(punto))!=string::npos){
            valor.erase(0,pos+punto.length());
        }
        return QString::fromStdString(valor);
}

void listamount::Mostrar(){
    nodolist *aux = primero;
    if (aux != nullptr){
        cout << "Estado de las particiones Montadas: " << endl;
        while (aux != nullptr) {
            cout << " Nombre: ";
            cout << aux->name.toStdString()<< endl;
            cout << " Identificador: "<< aux->id.toStdString() << endl;
            aux = aux->siguiente;

        }
    }else cout << "NO hay particiones Montadas " << endl;
}

void listamount::A(QString id){
    nodolist *aux = primero;
    if (aux != nullptr){
        while (aux != nullptr) {
            if (id == aux->id) cout << " Identificador: "<< aux->id.toStdString() << endl;
            aux = aux->siguiente;

        }
    }else cout << "NO hay particiones Montadas " << endl;
}


