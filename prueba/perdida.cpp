#include "perdida.h"

Perdida::Perdida()
{

}

void Perdida::MetodoLoss(int inicio, int final,QString direccion){
    FILE* disco;
    disco = fopen(direccion.toStdString().c_str(),"rb+");
    if (disco != nullptr){
        char byte = '0';
        fseek(disco,inicio,SEEK_SET);
        for(int i = inicio; i < final; i++){
            fputc(byte,disco);
        }
        fclose(disco);
        cout << "FATAL ERROR: PERDIDA DE DATOS \n" << endl;

    }else cout << "No existe el disco " << endl;


}
