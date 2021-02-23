#include "rmdisk.h"


QString pathrmdisk = "";

Rmdisk::Rmdisk()
{

}
void DeleteDisk(QString path);
void Rmdisk:: automataRmdisk(QString lexema,QString token ,int flag){
    switch (estadormdisk) {
        case 0:
            if (lexema == "rmdisk") estadormdisk = 1;
            else{
                cout << " No se reconoce el comando " << token.toStdString()<< endl;
                estadormdisk = 1000;
            }
          break;
         case 1:
            if (lexema == "path") estadormdisk = 2;
            else{
                cout << " Parametro path no encontrado"<< endl;
                estadormdisk = 1000;
            }
         break;
         case 2:

            if (lexema == "palabra"){
                estadormdisk = 3;
                pathrmdisk = token;
            }else{
                cout << " No se reconoce la direccion" <<  endl;
                estadormdisk = 1000;;
            }
           break;
          case 3:
              flag = 0;
              estadormdisk = 0;
              DeleteDisk(pathrmdisk);
        break;


    }
}

void DeleteDisk(QString path){
    QString pat = path.replace("\"","");
    ifstream correcto(path.toUtf8(), ifstream::in);
    if (correcto){
        string eleccion;
        cout << "Seguro que desea eliminar el disco "<< pathrmdisk.toStdString() << "? (Y/N) \n";
        cin >> eleccion;
        if (eleccion == "Y" || eleccion == "y"){
          QString comando = "rm \"" + pat + "\"";
          system(comando.toStdString().c_str());
          cout <<"Disco eliminado con exito" << endl;
        }else if (eleccion == "N" || eleccion == "n"){
          cout << "Ha decidido no borrar el disco" << endl;
        }else cout << "Escoga una opcion correcta Y / N: " << endl;

    }else cout << " No existe el archivo a eliminar" <<  endl;
}
