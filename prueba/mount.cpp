#include "mount.h"

Mount::Mount()
{

}

void Mount::AutomataMount(QString lexema, QString token , int flag){
    switch (estadomount) {
    case 0:
        estadomount = 1;
        break;
    case 1:
        if (lexema == "path"){
            flag_mount_path = 1;
            estadomount = 2;
        }else if (lexema == "name" ){
            flag_mount_name = 1;
            estadomount = 3;
        }else if (lexema == "finInstruccion"){

        }else cout << "no se reconoce el comando " << token.toStdString() << endl;
        break;
    case 2:
        if (lexema == "palabra"){
            pathmount = token;
            estadomount = 1;
        }else {
            cout << "no se reconoce la ruta" << endl;
            estadomount = 1000;
        }
        break;
    case 3:
        if (lexema == "palabra"){
            namemount = token;
            estadomount = 1;
        }else {
            cout << "no se reconoce el nombre" << endl;
            estadomount = 1000;
        }
        break;

    }

}
