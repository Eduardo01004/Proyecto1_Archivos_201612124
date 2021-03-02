#include "rep.h"


REP::REP()
{

}

void REP::AutomataRep(QString lexema,QString token,int flag){
    switch (estadorep) {
    case 0:
        estadorep = 1;
        break;
    case 1:
        if (lexema == "id"){
            estadorep = 2;
            flag_rep = 1;
        }else if (lexema == "path") {
            estadorep = 3;
            flag_pat = 1;
        }else if (lexema == "name"){
            estadorep = 4;
            flag_namerep = 1;
        }else if (lexema == "ruta"){
            estadorep = 5;
        }else if (lexema == "finInstruccion"){
            if (flag_namerep ){
                if (flag_pat){
                    if (flag_rep){
                        QString direccion = "";//l.lista->Direccion(idrep);
                       // l->lista.A(idrep);
                        if (direccion != ""){

                        }else cout << "NO existe el id: " << idrep.toStdString() << endl;

                    }else cout << "Parametro id obligatorio" << endl;

                }else cout << "Parametro path es obligatorio" << endl;

            }else cout << "Parametro name es obligatorio " << endl;

        }else cout << "Atributo para el Parametro Rep no reconocido" << token.toStdString() <<endl;
        break;
    case 2:
        if (lexema == "palabra"){
            estadorep = 1;
            idrep = token;
        }else {
            estadorep = 1000;
            cout << "EL parametro no se reconoce " << endl;
        }
        break;
    case 3:
        if (lexema == "palabra"){
            estadorep = 1;
            pathrep = token;
        }else {
            estadorep = 1000;
            cout << "No se reconoce la direccion " << endl;
        }
        break;
    case 4:
        if (token == "mbr"){
            estadorep = 1;
            flag_tipo = 0;
        }else if (token == "disk"){
            flag_tipo = 1;
            estadorep = 1;
        }else {
            estadorep = 1000;
            cout << "No se reconoce el parametro para name" << endl;
        }
        break;
    case 5:
        if (lexema == "palabra"){
            rutarep = token;
            estadorep = 1;
        }else {
            cout << "Se esperaba una direccion en la ruta" << endl;
            estadorep = 0;
            flag = 0;
        }
        break;
    }

}
