#include "mount.h"

Mount::Mount()
{

}
QStringList mount;
QStringList letters;
QStringList disk;
static QList<int> counts;
QString retornarPath2(QString path);

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
            estadomount = 0;
            if (flag_mount_path == 1){
                if (flag_mount_name == 1){
                    FILE *disco = fopen(pathmount.toStdString().c_str(),"rb+");
                    fseek(disco,0,SEEK_SET);
                    if (disco != nullptr){
                        MBR auxmbr;
                        fseek(disco,0,SEEK_SET);
                        fread(&auxmbr,sizeof(MBR),1,disco);

                        int indice = BuscarParticionP(auxmbr,namemount);
                        int encontrado = false;
                        if (indice != -1){
                            auxmbr.mbr_partition[indice].part_status='2';
                            fseek(disco,0,SEEK_SET);
                            fwrite(&auxmbr,sizeof(MBR),1,disco);
                            fseek(disco,0,SEEK_SET);
                            fclose(disco);

                            for (int i = 0; i < mount.size(); i++){
                                QStringList data = mount.at(i).split(',',QString::SkipEmptyParts);
                                if (data.at(0) == pathmount && data.at(1) == namemount){
                                    encontrado = true;
                                    cout << "Particion ya montada " <<endl;
                                    estadomount = 0;
                                    flag = 0;
                                }
                            }
                            if(!encontrado)
                            {
                                QString id = getId(pathmount);
                                mount.append(pathmount + "," + namemount + "," + id);
                                cout << id.toStdString() << endl;
                                nodolist *aux = lista->BMount(namemount);
                                if (aux != nullptr) cout << "Paticion ya montada" << endl;
                                else{
                                    nodolist *nodo =new nodolist(pathmount,namemount,'a','5',id);
                                    lista->Insertar(nodo);
                                    cout << "Particion Montada con exito" << endl;
                                    lista->Mostrar();
                                }
                            }


                        }else {//esto es una logica
                            MBR mbr;
                            FILE *disco2;
                            int encontrado2 = false;
                            disco2 = fopen(pathmount.toStdString().c_str(),"rb+");
                            if (disco2 != nullptr){
                                fseek(disco2,0,SEEK_SET);
                                fread(&mbr,sizeof(MBR),1,disco2);
                                EBR ebr;
                                int indice2 = BuscarL(mbr,namemount,ebr,pathmount,disco2);
                                if (indice2 != -2){
                                    fseek(disco2,indice2,SEEK_SET);
                                    fread(&ebr,sizeof(EBR),1,disco2);
                                    ebr.part_status = '2';
                                    fseek(disco2,indice2,SEEK_SET);
                                    fwrite(&ebr,sizeof(EBR),1,disco2);
                                    fseek(disco2,0,SEEK_SET);
                                    fclose(disco2);
                                    for (int i = 0; i < mount.size(); i++){
                                        QStringList data = mount.at(i).split(',',QString::SkipEmptyParts);
                                        if (data.at(0) == pathmount && data.at(1) == namemount){
                                            encontrado2 = true;
                                            cout << "Particion ya montada " <<endl;
                                            estadomount = 0;
                                            flag = 0;
                                        }
                                    }
                                    if(!encontrado2)
                                    {
                                        QString id = getId(pathmount);
                                        mount.append(pathmount + "," + namemount + "," + id);
                                        nodolist *aux = lista->BMount(namemount);
                                        if (aux != nullptr) cout << "Paticion ya montada" << endl;
                                        else{
                                            nodolist *nodo =new nodolist(pathmount,namemount,'a','5',id);
                                            lista->Insertar(nodo);
                                            cout << "Particion Logica Montada con exito" << endl;
                                            lista->Mostrar();
                                        }
                                    }
                                }else {
                                    cout << "Error no existe la particion logica" << endl;
                                    estadomount = 0;
                                    flag = 0;
                                }

                            }else cout << "NO existe el DIsco a buscar " << endl;


                        }

                    }else {
                        cout << "no existe el disco" << endl;
                        estadomount = 0;
                        flag = 0;
                    }


                }else cout << "Parametro name obligatorio " <<endl;

            }else cout << "Parametro path obligatorio " << endl;

        }else cout << "no se reconoce el comando:1 " << token.toStdString() << endl;
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

int Mount::BuscarParticionP(MBR mbr,QString nombre){
    int retorno = -1;
    for (int i = 0; i < 4; i++){
        if(mbr.mbr_partition[i].part_status != '-1'){
            if (strcmp(mbr.mbr_partition[i].part_name,nombre.toStdString().c_str()) == 0 ) retorno = i;
        }
    }
    return retorno;

}

int Mount::BuscarL(MBR mbr,QString nombre,EBR ebr,QString direccion,FILE *disco){
    int retorno = -2;
    int poslogica = 0;
    bool encontrado = false;
    for (int i = 0; i < 4; i++){
        if (mbr.mbr_partition[i].part_type == 'E'){
            poslogica = i;
            encontrado = true;
        }
    }
    if (encontrado == true){
        EBR auxebr;
        fseek(disco,mbr.mbr_partition[poslogica].part_start,SEEK_SET);

        int existe = 0;
        int peso = ftell(disco);
        int tamano = mbr.mbr_partition[poslogica].part_start + mbr.mbr_partition[poslogica].part_size;


        while(fread(&auxebr,sizeof(EBR),1,disco) != 0 && peso < tamano  ){
            if (strcmp(auxebr.part_name,nombre.toStdString().c_str()) == 0){
                retorno = peso - sizeof(EBR);
                break;
            }else if (auxebr.part_next != -1){
            }else break;
        }

    }else cout << "No se existe una logica " << endl;
    return retorno;
}

QString Mount::getId(QString path)
{
    letters.clear();
    letters  << "a" << "b" << "c" << "d" << "e"
             << "f" << "g" << "h" << "i" << "j"
             << "k" << "l" << "m" << "n" << "o"
             << "p" << "q" << "r" << "s" << "t"
             << "u" << "v" << "w" << "x" << "y"
             << "z";
    if(!disk.contains(path))
    {
        disk.append(path);
        counts.append(1);
    }
    for (int i = 0; i < disk.size(); i++){
        if(disk[i] != path){
            return  "24"  + QString::number(++counts[i]) + letters[i];
        }
    }
    return "";
}


void Mount::AutomataRep(QString lexema,QString token,int flag){
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
                        QString direccion = lista->Direccion(idrep);
                        QString ext = lista->Extension(pathrep);
                        if (direccion != ""){
                            estadorep = 0;
                            QString dato = retornarPath2(pathrep);
                            mkdir(dato.toStdString().c_str(), 0777);
                            if (flag_tipo == 0) gra->GraficarMBR_EBR(direccion,pathrep,ext);
                            else if (flag_tipo == 1 ) gra->GraficarDisk(direccion,pathrep,ext);



                        }else cout << "NO existe el id: " << idrep.toStdString() << endl;

                    }else cout << "Parametro id obligatorio" << endl;

                }else cout << "Parametro path es obligatorio" << endl;

            }else cout << "Parametro name es obligatorio " << endl;

        }else cout << "Atributo para el Parametro Rep no reconocido:" << token.toStdString() <<endl;
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

QString retornarPath2(QString path){
    QStringList ruta = path.split('/', QString::SkipEmptyParts);
    QString Directorio = "";
    for(int a = 0; a<ruta.count()-1; a++){
        Directorio += "/" + ruta[a];
        DIR *dir = opendir(Directorio.toUtf8());
        if(!dir){
            QString comando = "mkdir " + Directorio;
            auto result = mkdir(Directorio.toUtf8().constData(),  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        }
    }
    return Directorio;
}


void Mount::automataUnmount(QString lexema, QString token, int flag){
    switch (estadounmount) {
    case 0:
        if (token == "unmount") estadounmount = 1;
        break;
   case 1:
        if (lexema == "id") estadounmount = 2;
        else {
            estadounmount = 0;
            flag = 0;
            cout << "Error parametro id no esta" << endl;
        }
        break;
    case 2:
        if (lexema == "palabra"){
            estadounmount = 3;
            idunmount = token;
        }else {
            estadounmount = 0;
            flag = 0;
            cout << "se esperaba otro tipo de valor " << endl;
        }
        break;
    case 3:
        if (token == "finInstruccion"){
            estadomount = 0;
            flag = 0;
            if (lista->Delete(idunmount) == 0) cout << "Error esta unidad no esta montada" << endl;
            else cout << "Unidad Desmontada con exito" << endl;
        }

    }

}


void Mount::AutomataMkfs(QString lexema,QString token,int flag){
    switch (estadomkfs) {
    case 0:
        estadomkfs = 1;
        break;
    case 1:
        if (lexema == "id"){
            flag_idmkfs = 1;
            estadomkfs = 2;
        }else if (lexema == "type"){
            flag_tipomkfs = 1;
            estadomkfs = 3;
        }else if (lexema == "fs"){
            flag_msmkfs = 1;
            estadomkfs = 4;
        }else if (lexema == "finInstruccion"){
            estadomkfs = 0;
            if (flag_idmkfs){
                if (flag_tipomkfs == 0){
                    tipomkfs = "full";
                }
                if (flag_msmkfs == 0){
                    msmkfs = "3fs";
                }
                nodolist *nodo = lista->BMount(idmkfs);
                if (nodo != nullptr){
                    MBR auxmbr;
                    FILE *disco = fopen(nodo->direccion.toStdString().c_str(),"rb+");
                    if (disco != nullptr){
                        fseek(disco,0,SEEK_SET);
                        fread(&auxmbr,sizeof(MBR),1,disco);
                        int index = BuscarParticionP(auxmbr,nodo->name);
                        if (index != -1){
                            if (flag_msmkfs == 0 ) {
                                mks->FormatearEXT3(auxmbr.mbr_partition[index].part_start,(auxmbr.mbr_partition[index].part_start+auxmbr.mbr_partition[index].part_size),auxmbr.mbr_partition[index].part_size,nodo->direccion);
                            }else{
                                flag_msmkfs = 0;
                                mks->FormatearEXT2(auxmbr.mbr_partition[index].part_start,(auxmbr.mbr_partition[index].part_start+auxmbr.mbr_partition[index].part_size),auxmbr.mbr_partition[index].part_size,nodo->direccion);
                            }
                            fclose(disco);
                        }else {
                            FILE *disco2;
                            MBR auxmbr;
                            disco2 = fopen(nodo->direccion.toStdString().c_str(),"rb+");
                            fseek(disco2,0,SEEK_SET);
                            fread(&auxmbr,sizeof(MBR),1,disco2);
                            EBR ebr2;
                            int index2 = BuscarL(auxmbr,nodo->name,ebr2,nodo->direccion,disco2);
                            if (index2 != -2){
                                EBR ebr;
                                cout << "index" << index2<<endl;
                                fseek(disco2,index2,SEEK_SET);
                                fread(&ebr,sizeof(EBR),1,disco2);

                                cout << "tamano" << ebr.part_size - sizeof(EBR)<< endl;
                                if (flag_msmkfs == 0 ) {
                                    mks->FormatearEXT3(ftell(disco2),ftell(disco2) + ebr.part_size - sizeof(EBR),ebr.part_size - sizeof(EBR),nodo->direccion);
                                }else{
                                    mks->FormatearEXT2(ftell(disco2),ftell(disco2) + ebr.part_size - sizeof(EBR),ebr.part_size - sizeof(EBR),nodo->direccion);
                                }
                                fclose(disco2);

                            }else{
                                cout << "Error no se encuentra la particion " << endl;
                            }


                        }
                    }else cout << "El disco no existe" << endl;


                }else cout << "No existe la particion para mkfs" << endl;


            }else {
                estadomkfs = 0;
                flag = 0;
                cout << "Error se esperaba un id" << endl;
            }


        }else {
            estadomkfs = 0;
            flag = 0;
            cout << "Atributo para el Parametro MKFS no reconocido:" << token.toStdString() <<endl;
        }
        break;
    case 2:
        if (lexema == "palabra"){
            idmkfs = token;
            estadomkfs = 1;
        }else {
            estadomkfs = 0;
            flag = 0;
            cout << "No se reconoce el id " << endl;
        }
        break;
    case 3:
        if (token == "Fast" || token == "fast" || token == "full" || token == "Full"){
            tipomkfs = token;
            estadomkfs = 1;
        }else {
            estadomkfs = 0;
            flag = 0;
            cout << "No se reconoce el tipo" << endl;
        }
        break;
    case 4:
        if (token == "2fs" || token == "2Fs" || token == "3fs" || token == "3Fs"){
            if (token == "3fs") flag_msmkfs = 0;
            msmkfs = token;
            estadomkfs = 1;
        }else {
            estadomkfs = 0;
            flag = 0;
            cout << "No se reconoce el sistema de archivos a formatear " << endl;
        }


    }

}


void Mount:: AutomataLogin(QString lexema,QString token,int flag){
    switch (estadologin) {
    case 0:
        estadologin = 1;
        break;
    case 1:
        if (lexema == "usr")estadologin = 2;
        else if (lexema == "pwd") estadologin = 3;
        else if (lexema == "id") estadologin = 4;
        else if (lexema == "finInstruccion") {
            estadologin = 0;
            if (flag_idlog){
                if (flag_usr){
                    if (flag_pass){
                        if (!flag_session){
                            nodolist *aux = lista->BMount(idlog);
                            if (aux != nullptr){
                                FILE *disco;
                                MBR auxmbr;
                                disco = fopen(aux->direccion.toStdString().c_str(),"rb+");
                                if (disco != nullptr){
                                    fseek(disco,0,SEEK_SET);
                                    fread(&auxmbr,sizeof(MBR),1,disco);

                                    int index = -1;
                                    for (int i = 0; i < 4; i++){
                                        if(auxmbr.mbr_partition[i].part_status != '1'){
                                            if(strcmp(auxmbr.mbr_partition[i].part_name,aux->name.toStdString().c_str()) == 0  ){
                                                index = i;
                                                break;
                                            }
                                        }
                                    }
                                    if (index != -1){
                                        EBR eb;
                                        if (usr == "root") flag_root = 1;
                                        log->Loguear(index,disco,auxmbr,aux->direccion,usr,pass,userlog,0,1,eb);
                                        estadologin = 0;
                                        flag = 0;
                                        flag_session = 1;

                                    }else {
                                        FILE *disco;
                                        MBR auxmbr;
                                        disco = fopen(aux->direccion.toStdString().c_str(),"rb+");
                                        fseek(disco,0,SEEK_SET);
                                        fread(&auxmbr,sizeof(MBR),1,disco);
                                        EBR ebr;
                                        int index2 = BuscarL(auxmbr,aux->name,ebr,aux->direccion,disco);
                                        if (index2 != -2){
                                            log->Loguear(0,disco,auxmbr,aux->direccion,usr,pass,userlog,index2,2,ebr);

                                        }else {
                                            cout << "Error no existe la particion logica" << endl;
                                            estadologin = 0;
                                            flag = 0;

                                        }


                                    }


                                }else cout << "El disco no existe" << endl;



                            }else {
                                estadologin = 0;
                                flag = 0;
                                cout << "La particion no esta montada a la que intenta Loguearse" << endl;

                            }

                        }else {
                            estadologin = 0;
                            flag = 0;
                            cout << "Ya hay una sesion Activa!!!!!" << endl;
                        }

                    }else {
                        estadologin = 0;
                        flag = 0;
                        cout << "Parametro pwd obligatorio" << endl;
                    }

                }else {
                    estadologin = 0;
                    flag = 0;
                    cout << "Parametro usr obligatorio" << endl;
                }

            }else {
                estadologin = 0;
                flag = 0;
                cout << "Parametro ID obligatorio" << endl;
            }

        }else {
            estadologin = 0;
            flag = 0;
            cout << "Atributo para el Parametro Login no reconocido:" << token.toStdString() <<endl;
        }
        break;
    case 2:
        if (lexema == "palabra"){
            usr = token;
            flag_usr = 1;
            estadologin = 1;
        }else {
            estadologin = 0;
            flag = 0;
            cout << "No se reconoce el valor " << token.toStdString() <<endl;
        }
        break;
    case 3:
        estadologin = 1;
        pass = token;
        flag_pass = 1;
        break;
    case 4:
        if (lexema == "palabra"){
            idlog = token;
            flag_idlog = 1;
            estadologin = 1;
        }else {
            estadologin = 0;
            flag = 0;
            cout << "No se reconoce el valor " << token.toStdString() <<endl;
        }
        break;

    }

}


void Mount:: AutomataLogout(){
    if (flag_session){
        flag_session = 0;
        flag_root = 0;
        log->path = "";
        log->inicioSuper = -1;
        log->id_user = -1;
    }else cout << "Error no existe ninguna sesion a la cual hacerle Logout" << endl;
}


void Mount::AutomataMkgrp(QString lexema, QString token,int flag){
    switch (estadomkgrp) {
    case 0:
        estadomkgrp = 1;
        break;
    case 1:
        if (lexema == "name"){
            estadomkgrp = 2;
        }else {
            estadomkgrp = 0;
            flag = 0;
            cout << "Error se esperaba el parametro name" << endl;
        }
        break;
    case 2:
        if(lexema == "palabra"){
            estadomkgrp = 3;
            mk_name = token;
        }else {
            estadomkgrp = 0;
            flag = 0;
            cout << "Error se esperaba el parametro name" << endl;
        }
        break;
    case 3:
        if (lexema == "finInstruccion"){
            if (flag_session){
                if (flag_root){
                    estadomkgrp = 0;
                    int buscar = log->BuscarG(mk_name,log->inicioSuper);
                    if (buscar == -1) cout << "El nombre del grupo ya existe " <<endl;
                    else {
                        FILE *disco;
                        disco=fopen(log->path.toStdString().c_str(),"r+b");
                        if (strlen(mk_name.toStdString().c_str()) < 10){
                            char temp[66]="\0";
                            memset(temp,0,sizeof(temp));
                            sprintf(temp,"%s%d",temp,buscar);
                            strcat(temp,", G, ");
                            strcat(temp,mk_name.toStdString().c_str());
                            strcat(temp,"\n");
                            int res = log->CrearGrupo(disco,temp,log->path);
                            if (res == 1) cout << "grupo creado con exito " <<endl;
                            else cout << " no se pudo crear el grupo" << endl;

                        }else cout << "El nombre del grupo excede los 10 caracteres" <<endl;

                    }


                }else {
                    estadomkgrp = 0;
                    flag = 0;
                    cout << "Error solo el usuario root puede ejecutar el comando Mkgrp "<< endl;
                }

            }else {
                estadomkgrp = 0;
                flag = 0;
                cout << "Error se necesita tener una sesion Activa" << endl;
            }

        }else {
            estadomkgrp = 0;
            flag = 0;
            cout << "Error no se reconoce el comando" << endl;
        }
    }


}



void Mount::AutomataMkusr(QString lexema,QString token, int flag){
    switch (estadomkusr) {
    case 0:
        estadomkusr = 1;
        break;
    case 1:
        if (lexema == "usr"){
            estadomkusr = 2;
            flag_usr_usr = 1;
        }else if(lexema == "pwd"){
            estadomkusr = 3;
            flag_pwd_usr = 1;
        }else if (lexema == "grp"){
            estadomkusr = 4;
            flag_usr_grp = 1;
        }else if (lexema == "finInstruccion"){
            estadomkusr = 0;
            if (flag_session){
                if (flag_root){
                    if (flag_usr_usr && flag_usr_grp && flag_pwd_usr){
                        estadomkusr = 0;
                        int buscar = log->BuscarG(grp_usr,log->inicioSuper);
                        if ( buscar == -1){ // que si existe
                            int user = log->BuscarU(usr_usr,log->inicioSuper);
                            if ( user == -1 ) cout << "EL usuario ya existe " <<endl;
                            else {
                                FILE *disco;
                                disco=fopen(log->path.toStdString().c_str(),"r+b");
                                //QString contenido = QString::number(user) + ", U, "+grp_usr+", "+usr_usr+", "+pass_usr+"\n";
                                char enviar[66];
                                memset(enviar,0,sizeof(enviar));
                                sprintf(enviar,"%d, U, %s, %s, %s\n",user,grp_usr.toStdString().c_str(),usr_usr.toStdString().c_str(),pass.toStdString().c_str());
                                int res = log->CrearGrupo(disco,enviar,log->path);
                                if (res == 1) cout << "Usuario creado con exito" << endl;
                                else cout << "NO se pudo crear el usuario "<<endl;
                            }

                        }else{
                            estadomkusr = 0;
                            flag = 0;
                            cout << "Error el grupo no existe"<< endl;
                        }


                    }else {
                        estadomkusr = 0;
                        flag = 0;
                        cout << "Error parametros incorrectos "<< endl;
                    }

                }else {
                    estadomkusr = 0;
                    flag = 0;
                    cout << "Error solo el usuario root puede ejecutar el comando Mkusr"<< endl;
                }

            }else {
                estadomkusr = 0;
                flag = 0;
                cout << "Error no hay una sesion activa."<< endl;
            }

        }else {
            estadomkusr = 0;
            flag = 0;
            cout << "Error no se reconoce el comando. "<< endl;
        }
        break;
    case 2:
        if (lexema == "palabra"){
            estadomkusr = 1;
            usr_usr = token;
        }else {
            estadomkusr = 0;
            flag = 0;
            cout << "Error no se reconoce la palabra "<< endl;
        }
        break;
    case 3:
        pass_usr = token;
        estadomkusr = 1;
        break;
    case 4:
        if(lexema == "palabra"){
            grp_usr = token;
            estadomkusr = 1;
        }else {
            estadomkusr = 0;
            flag = 0;
            cout << "error no se reconoce la palabra "<< endl;
        }
        break;
    }
}


void Mount::AutomataMkdir(QString lexema, QString token, int flag){
    switch (estadomkdir) {
    case 0:
        estadomkdir = 1;
        break;
    case 1:
        if (lexema == "p"){
            estadomkdir = 1;
            flag_mkdir_p = 1;
        }else if (lexema == "path"){
            estadomkdir = 2;
            flag_mkdir_path = 1;
        }else if (lexema == "finInstruccion"){
            estadomkdir = 0;
            if (flag_session){
                if (flag_mkdir_path){
                    superBloque sb;
                    FILE *DiscoEnUso;
                    DiscoEnUso = fopen(log->path.toStdString().c_str(),"rb+");
                    if (DiscoEnUso != nullptr){
                        int numeracion = 0;
                        fseek(DiscoEnUso,log->inicioSuper,SEEK_SET);
                        fseek(DiscoEnUso,0,SEEK_SET);
                        fread(&sb,sizeof(superBloque),1,DiscoEnUso);
                        string aux = path_mkdir.toStdString();
                        char auxPath[500];
                        strcpy(auxPath,aux.c_str());
                        int coco = dir.BuscarCoA(DiscoEnUso,auxPath,log->inicioSuper,&numeracion);
                        cout << coco << "buscar " <<endl;
                        if (coco == 1) cout << "ya existe el directorio"<< endl;
                        else if (coco == 0){
                           // cout << "se crea car[eta" <<endl;
                            string aux = path_mkdir.toStdString();
                            char auxPath[500];
                            strcpy(auxPath,aux.c_str());
                            int respuesta = dir.CrearCarpeta(DiscoEnUso,'F',0,log->inicioSuper,auxPath,flag_mkdir_p);
                            if (respuesta == 4) cout << "La direccion no existe  y no se puede crear por falta del comando P" <<endl;
                            else cout << "Se creo la carpeta con exito" <<endl;
                            fclose(DiscoEnUso);
                        }
                    }else cout << "NO existe el disco "<<endl;




                }else {
                    cout << "parametro path obligatorio" <<endl;
                    estadomkdir = 0;
                    flag = 0;
                }

            }else {
                cout << "error no hay una session activa" <<endl;
                estadomkdir = 0;
                flag = 0;
            }

        }else {
            cout << "no se reconoce el comando" <<endl;
            estadomkdir = 0;
            flag = 0;
        }
        break;
    case 2:
        if (lexema == "palabra"){
           path_mkdir = token;
           estadomkdir = 1;
        }else {
            cout << "error se esperaba un path" <<endl;
            estadomkdir = 0;
            flag = 0;
        }
        break;
    }

}
