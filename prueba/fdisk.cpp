#include "fdisk.h"

Fdisk::Fdisk()
{

}
int posicion_logica(QString path);
void Fdisk::AutomataFdisk(QString lexema,QString token, int flag){
    switch (estadoFdisk) {
        case 0:
            estadoFdisk = 1;
        break;
        case 1:
            if (lexema == "size") estadoFdisk = 2;
            else if (lexema == "unit") estadoFdisk = 3;
            else if (lexema == "path") estadoFdisk = 4;
            else if (lexema == "type") estadoFdisk = 5;
            else if (lexema == "fit") estadoFdisk = 6;
            else if (lexema == "name") estadoFdisk = 7;
            else if (lexema == "delete"){
                estadoFdisk = 8;
                deletefdisk = 1;
            }else if (lexema == "add") estadoFdisk = 9;
            else if (lexema == "finInstruccion"){
                if (pathfdisk){
                    if (deletefdisk){//si viene delete se elimina
                        estadoFdisk = 0;
                        flag = 0;
                        EliminarParticion(pathfd,name,tipodelete);

                    }
                    else {//sino se crea una particion
                        if (flag_size){ //tiene que venir un size
                            if (flag_name){//name obligatorio
                                estadoFdisk = 0;
                                //flag = 0;
                                if (typefdisk == "p"){
                                    CrearParticionPrimaria(pathfd,unitmkdisk,tamanio,typefdisk,name,fitfdisk);
                                }else if (typefdisk == "e") CrearExtendida(pathfd,unitmkdisk,tamanio,typefdisk,name,fitfdisk);
                                else if (typefdisk == "L") CrearLogica(pathfd,unitmkdisk,tamanio,typefdisk,name,fitfdisk);



                            }else {
                                estadoFdisk = 1000;
                                cout << "Parametro Name es obligatorio" << endl;
                            }

                        }else {
                            estadoFdisk = 1000;
                            cout << "Parametro size Obligatorio para crear una particion. " << endl;
                        }

                    }

                }else {
                    estadoFdisk = 1000;
                    cout << "Hace falta el parametro path, Parametro obligatorio. " << endl;
                }

            }else {
                cout << " No se reconoce el comando:" << token.toStdString()<< endl;
                estadoFdisk = 1000;
            }
        break;
        case 2:
        if (lexema == "numero"){ // este es el size
            tamanio = atoi(token.toStdString().c_str());
            if (tamanio > 0){
                flag_size = 1;
                estadoFdisk = 1;
            }else {
                cout << "EL valor de size es menor a 0: " << endl;
                estadoFdisk =1000;
            }


        }else {
            cout << " No se reconoce el comando:" << token.toStdString()<< endl;
            estadoFdisk = 1000;
        }

        break;
        case 3:
        if (lexema == "palabra"){ // para los units
            if (token == "B" || token == "b"){
                estadoFdisk = 1;
                flag_unit = 1;
                unitmkdisk = 'b';
            }else if (token == "K" || token == "k"){
                estadoFdisk = 1;
                flag_unit = 1;
                unitmkdisk = 'k';
            }else if (token == "M" || token == "m"){
                estadoFdisk = 1;
                flag_unit = 1;
                unitmkdisk = 'm';
            }else {
                estadoFdisk = 1000;
                cout << " No se reconoce el valor del parametro unit: " << token.toStdString()<< endl;
            }
        }else {
            estadoFdisk = 1000;
            cout << " No se reconoce el valor del parametro unit: " << token.toStdString()<< endl;
        }
        break;
        case 4:
        if (lexema == "palabra"){ // este es para el path
            estadoFdisk = 1;
            pathfdisk = 1;
            pathfd = token;
        }else {
            estadoFdisk = 1000;
            cout << " No se reconoce la ruta: " << token.toStdString()<< endl;
        }
        break;
        case 5:
        if (lexema == "palabra"){ // para el tipo de particion
            if (token == "P" || token == "p"){
                estadoFdisk = 1;
                tipo = 1;
                typefdisk = "p";
            }else if (token == "E" || token == "e"){
                estadoFdisk = 1;
                tipo = 1;
                typefdisk = "e";
            }else if (token == "L" || token == "l"){
                estadoFdisk = 1;
                tipo = 1;
                typefdisk = "L";
            }else {
                estadoFdisk = 1000;
                cout << " No se reconoce el valor de particion: " << token.toStdString()<< endl;

            }
        }else {
            cout << " No se reconoce el tipo de particion: " << token.toStdString()<< endl;
            estadoFdisk = 1000;
        }
        break;
        case 6:
        if (lexema == "palabra"){ // para el tipo de ajuste
            if (token == "BF" || token == "bf"){
                estadoFdisk = 1;
                ajuste = 1;
                fitfdisk = "B";
            }else if (token == "FF" || token == "ff"){
                estadoFdisk = 1;
                ajuste = 1;
                fitfdisk = "F";
            }else if (token == "WF" || token == "wf"){
                estadoFdisk = 1;
                ajuste = 1;
                fitfdisk = "W";
            }else {
                estadoFdisk = 1000;
                cout << " No se reconoce el valor de ajuste: " << token.toStdString()<< endl;

            }
        }else {
            cout << " No se reconoce el tipo: " << token.toStdString()<< endl;
            estadoFdisk = 1000;
        }
        break;
        case 7:
        if (lexema == "palabra"){//para el nombre de la particion
            flag_name = 1;
            estadoFdisk = 1;
            name = token;
        }else {
            cout << " No se reconoce el nombre: " << token.toStdString()<< endl;
            estadoFdisk = 1000;
        }
        break;
        case 8:
        if (token == "Fast" || token == "fast"){
            estadoFdisk = 1;
            tipodelete = "fast";
        }else if (token == "Full" || token == "full"){
            estadoFdisk = 1;
            tipodelete = "full";
        }else {
            estadoFdisk = 0;
            flag = 0;
            cout << " No se reconoce el tipo: " << token.toStdString()<< endl;
        }
        break;
       }

}

int Fdisk::Size(char tipo,int size){
    int bites = 1024;
    if (tipo != 0){
        if (tipo == 'k' || tipo == 'K'){
             bites = size * 1024;
             return bites;
        }
        else if ( tipo == 'm' || tipo == 'M') {
            bites = size * 1024 * 1024;
            return bites;
        }
        else{
            bites = size;
            return bites;
        }
    }else{
        bites = size * 1024;
        return bites;
    }
}

bool Fdisk::BuscarParticion(QString nombre,QString direccion){
    int contador = 0;
    FILE *disco = disco=fopen(direccion.toStdString().c_str(),"rb+");
    if (disco == nullptr) cout << "El disco no existe" << endl;
    else {
        MBR auxmbr;
        fseek(disco,0,SEEK_SET);
        fread(&auxmbr,sizeof(MBR),1,disco);
        int tamano = ftell(disco);
        for (int x = 0 ; x < 4; x++){
            if (auxmbr.mbr_partition[x].part_type== 'P'){
                if (strcmp(nombre.toStdString().c_str(), auxmbr.mbr_partition[x].part_name) == 0){
                    fclose(disco);
                    return true;
                }
            }else if (auxmbr.mbr_partition[x].part_type== 'E'){
                    contador = x;
                if (strcmp(nombre.toStdString().c_str(), auxmbr.mbr_partition[x].part_name) == 0){
                    fclose(disco);
                    return true;
                }else{
                    EBR ebr;
                    fseek(disco,auxmbr.mbr_partition[x].part_start,SEEK_SET);

                    int suma = (auxmbr.mbr_partition[x].part_size + auxmbr.mbr_partition[x].part_start);
                    while((fread(&ebr,sizeof(ebr),1,disco))!= 0 && tamano < suma){
                        if(strcmp(ebr.part_name,nombre.toStdString().c_str()) == 0){
                            fclose(disco);
                            return true;
                            break;
                        }
                    if(ebr.part_next != -1){
                    }else{
                        fclose(disco);
                        return false;
                        break;
                    }
                }
                }
            }
        }
    }
    fclose(disco);
    return false;
}


void Fdisk::CrearParticionPrimaria(QString path,char unit,int size,QString type,QString name,QString fit){
    MBR auxmbr;
    FILE* disco = disco=fopen(path.toStdString().c_str(),"rb+");

    if (disco == nullptr) cout << "No se encuentra el disco" << endl;
    else cout << " " << endl;

    fseek(disco,0,SEEK_SET);// se posiciona en el inicio del disco
    fread(&auxmbr,sizeof( MBR),1,disco);//para leer el struct

    int tam_particion = Size(unit,size);
    bool flag_part = false;
    char buffer = '1';
    int posicion;
    for (int i = 0; i < 4; i++){
        if (auxmbr.mbr_partition[i].part_status == '1'|| auxmbr.mbr_partition[i].part_start == -1 ){
            flag_part = true;
            posicion = i;
            break;
        }
    }

    if (flag_part == false && type != "L"){
        cout << "Ya hay 4 particiciones,primarias" << endl;
        fclose(disco);
    }else {
        if (!BuscarParticion(name,path)){
                if (tam_particion <= auxmbr.mbr_tamano){
                    int inicio = sizeof(MBR);
                    int final = inicio + tam_particion;
                    //int posicion = 0;
                    int tam = 0 ;

                    for (int i = 0; i < 4; i++){
                        if (auxmbr.mbr_partition[i].part_start != -1){
                            inicio = auxmbr.mbr_partition[i].part_start + auxmbr.mbr_partition[i].part_size;
                            final = inicio + tam_particion;
                            //posicion = i + 1 ;
                            tam = tam + auxmbr.mbr_partition[i].part_size;
                        }else break;
                    }
                        if (final <= auxmbr.mbr_tamano){
                            cout << "Tamanio libre: " << (auxmbr.mbr_tamano - tam) << endl;
                            cout << "Nuevo tamanio: " << tam_particion << endl;
                            auxmbr.mbr_partition[posicion].part_type = 'P';

                            if (fit == "W") auxmbr.mbr_partition[posicion].part_fit = 'W';
                            else if (fit == "B") auxmbr.mbr_partition[posicion].part_fit = 'B';
                            else auxmbr.mbr_partition[posicion].part_fit = 'F';
                            if (posicion == 0 )auxmbr.mbr_partition[posicion].part_start = sizeof(MBR);
                            else auxmbr.mbr_partition[posicion].part_start = auxmbr.mbr_partition[posicion-1 ].part_start + auxmbr.mbr_partition[posicion-1].part_size;
                            //auxmbr.mbr_partition[posicion].part_start = inicio;
                            auxmbr.mbr_partition[posicion].part_status = '0';
                            auxmbr.mbr_partition[posicion].part_size = tam_particion;
                            strcpy(auxmbr.mbr_partition[posicion].part_name,name.toStdString().c_str());
                            fseek(disco, 0, SEEK_SET);
                            fwrite(&auxmbr, sizeof (MBR), 1, disco);
                            fseek(disco,auxmbr.mbr_partition[posicion].part_start,SEEK_SET);
                            for (int i = 0; i < tam_particion; i++){
                                fwrite(&buffer,1,1,disco);
                            }
                            cout << "Particion Primaria Creada  con exito" << endl;
                        }else cout << "Ya no hay espacion en la particion" << endl;
                }else cout << "Error<< la particion a guardar excede el tamaño libre." << endl;

        }else cout << "La particion" << name.toStdString() << "Ya existe" << endl;
        fclose(disco);
    }
}



void Fdisk::CrearExtendida(QString path,char unit,int size,QString type,QString name,QString fit){
    MBR auxmbr;
    FILE* disco = disco=fopen(path.toStdString().c_str(),"rb+");

    if (disco == nullptr) cout << "No se encuentra el disco" << endl;
    else cout << " " << endl;

    fseek(disco,0,SEEK_SET);// se posiciona en el inicio del disco
    fread(&auxmbr,sizeof( MBR),1,disco);//para leer el struct

    int tam_particion = Size(unit,size);
    bool flag_part = false;
    char buffer = '1';

    for (int i = 0; i < 4; i++){
        if (auxmbr.mbr_partition[i].part_type == 'E'){
            flag_part = true;
            break;
        }
    }

    if (flag_part == true  && type != "L"){
        cout << "Ya hay 1 particion Extendida" << endl;
        fclose(disco);
    }else {
        if (!BuscarParticion(name,path)){
                if (tam_particion <= auxmbr.mbr_tamano){
                    int inicio = sizeof(MBR);
                    int final = inicio + tam_particion;
                    int posicion = 0;
                    int tam = 0 ;
                    for (int i = 0; i < 4; i++){
                        if (auxmbr.mbr_partition[i].part_start == -1 || auxmbr.mbr_partition[i].part_status == '1'){
                            posicion = i;
                            break;
                        }
                    }
                    for (int i = 0; i < 4; i++){
                        if (auxmbr.mbr_partition[i].part_start != -1){
                            inicio = auxmbr.mbr_partition[i].part_start + auxmbr.mbr_partition[i].part_size;
                            final = inicio + tam_particion;
                            //posicion = i + 1 ;
                            tam = tam + auxmbr.mbr_partition[i].part_size;
                        }else break;
                    }
                        if (final <= auxmbr.mbr_tamano){
                            cout << "posicion " << posicion << endl;
                            auxmbr.mbr_partition[posicion].part_type = 'E';

                            if (fit == "W") auxmbr.mbr_partition[posicion].part_fit = 'W';
                            else if (fit == "B") auxmbr.mbr_partition[posicion].part_fit = 'B';
                            else auxmbr.mbr_partition[posicion].part_fit = 'F';

                            if (posicion == 0) auxmbr.mbr_partition[posicion].part_start = sizeof(MBR);
                            else auxmbr.mbr_partition[posicion].part_start = auxmbr.mbr_partition[posicion-1 ].part_start + auxmbr.mbr_partition[posicion-1].part_size;
                            auxmbr.mbr_partition[posicion].part_status = '0';
                            auxmbr.mbr_partition[posicion].part_size = tam_particion;
                            strcpy(auxmbr.mbr_partition[posicion].part_name,name.toStdString().c_str());
                            fseek(disco, 0, SEEK_SET);
                            fwrite(&auxmbr, sizeof (MBR), 1, disco);

                            EBR ebr;
                            fseek(disco, auxmbr.mbr_partition[posicion].part_start, SEEK_SET);
                            ebr.part_start = auxmbr.mbr_partition[posicion].part_start;
                            ebr.part_fit = 'F';
                            ebr.part_status = '0';
                            ebr.part_size = 0;
                            ebr.part_next = -1;
                            strcpy(ebr.part_name,"");
                            int tamebr = tam_particion -  sizeof(EBR);
                            fwrite(&ebr, sizeof (EBR), 1, disco);
                            for (int i = 0; i < tamebr; i++){
                                fwrite(&buffer,1,1,disco);
                            }
                            cout << "Particion Extendida Creada con exito" << endl;
                        }else cout << "Ya no hay espacion en la particion Extendida" << endl;
                }else cout << "Error<< la particion Extendida a guardar excede el tamaño libre." << endl;



        }else cout << "La particion" << name.toStdString() << "Ya existe" << endl;
        fclose(disco);
    }
}

void Fdisk::CrearLogica(QString path,char unit,int size,QString type,QString name,QString fit){
    FILE *file = fopen(path.toStdString().c_str(), "rb+");
        int poslogica = 0;
        int tam_part = Size(unit,size);
        if (file != nullptr){
            MBR mbr;
            bool encontrado = false;
            fseek(file, 0, SEEK_SET);
            fread(&mbr, sizeof (MBR), 1, file);

            if (Existe_Extendida(mbr,1)){
                poslogica = pos_Extendida(mbr);
                if (!BuscarParticion(name,path)){
                    EBR ebr;
                    fseek(file,mbr.mbr_partition[poslogica].part_start,SEEK_SET);
                    fread(&ebr, sizeof(EBR),1,file);
                    int ebrtam = sizeof(MBR);
                    ebrtam = sizeof(EBR) + ebr.part_start;
                    if (true){
                         int fin = mbr.mbr_partition[poslogica].part_size + mbr.mbr_partition[poslogica].part_start;
                         int auxiliar = posicion_logica(path);
                         if (auxiliar == 0){ // si es la primera logica
                             int start = mbr.mbr_partition[poslogica].part_start;
                             if (tam_part > sizeof(EBR) && tam_part < mbr.mbr_partition[poslogica].part_size){
                                 if (fit == "W") ebr.part_fit = 'W';
                                 else if (fit == "B") ebr.part_fit = 'B';
                                 else ebr.part_fit = 'F';
                                 ebr.part_status = '0';
                                 ebr.part_start = ftell(file);
                                 ebr.part_size = tam_part;
                                 ebr.part_next = -1;
                                 strcpy(ebr.part_name,name.toStdString().c_str());
                                 cout << ebr.part_name << endl;
                                 fseek(file,mbr.mbr_partition[poslogica].part_start,SEEK_SET);
                                 fwrite(&ebr,sizeof(EBR),1,file);
                                 cout << "Tamano:" << tam_part << " Bytes " <<endl;
                                 cout << "inicio de la particion: "<< start << " Bytes " <<endl;
                                 cout << "Particion logica Creada con exito" << endl;
                             }else cout << "La particion Logica es Demasiado Grande " << endl;

                         }else {
                             //fread(&mbr,sizeof(MBR),1,file);
                             int indice = mbr.mbr_partition[poslogica].part_start;
                             int aux2;
                             int flag = 0;
                             int inicio = -1;
                             int final = -1;
                             int total = -1;
                             EBR nuevo;
                             do {
                                 fseek(file,indice,SEEK_SET);
                                 fread(&ebr,sizeof(EBR),1,file);
                                 aux2 = ebr.part_start;
                                 if (ebr.part_next == -1){
                                    inicio = ebr.part_start + ebr.part_size;
                                    final = mbr.mbr_partition[poslogica].part_start + mbr.mbr_partition[poslogica].part_size;
                                    total = inicio + tam_part;
                                    flag = 1;
                                 }else {
                                     if (ebr.part_next != inicio){
                                         indice = ebr.part_next;
                                         flag = 1;
                                     }else indice = ebr.part_next;
                                 }

                             }while(flag == 0);
                             while ((ebr.part_next != -1) && ftell(file) < (mbr.mbr_partition[poslogica].part_size + mbr.mbr_partition[poslogica].part_start)) {
                                 fseek(file,ebr.part_next,SEEK_SET);
                                 fread(&ebr,sizeof(EBR),1,file);
                             }
                             int espacio = ebr.part_size + tam_part;
                             if ((espacio) > (mbr.mbr_partition[poslogica].part_size)){
                                 cout << "No se puede guardar excede el tamano de la particion extendida" << endl;
                             }else {

                             //if (total <= final){
                                 ebr.part_next = ebr.part_start + ebr.part_size;
                                 fseek(file,ftell(file)-sizeof(EBR),SEEK_SET);
                                 fwrite(&ebr,sizeof(EBR),1,file);
                                 fseek(file,ebr.part_start + ebr.part_size,SEEK_SET);

                                 if (fit == "W") ebr.part_fit = 'W';
                                 else if (fit == "B") ebr.part_fit = 'B';
                                 else ebr.part_fit = 'F';

                                 ebr.part_status = '0';
                                 ebr.part_start = ftell(file);
                                 ebr.part_size = tam_part;
                                 ebr.part_next = -1;
                                 strcpy(ebr.part_name,name.toStdString().c_str());
                                 cout << ebr.part_name << endl;
                                 cout << " Tamano:" << tam_part << " Bytes " <<endl;
                                 cout << " inicio de la particion: "<< inicio << " Bytes " <<endl;
                                 cout << " Particion logica Creada con exito" << endl;
                                 fwrite(&ebr,sizeof(EBR),1,file);

                             }

                         }

                    }

                }else cout << "La particion logica a Crear ya existe "<<endl;

            }else cout << "No existe una particion extendida, para crear una logica " << endl;
        fclose(file);

        }else cout << "Error no existe el disco " << endl;

}



void Fdisk::EliminarParticion(QString path, QString name,QString tipo){
    FILE *file;
    int posicion = -1;
    int primaria = -1;
    int extendida = -1;
    char buffer = '\0';
    file = fopen(path.toStdString().c_str(), "rb+");
    if (file != nullptr){
        MBR mbr;
        fseek(file,0,SEEK_SET);
        fread(&mbr,sizeof(MBR),1,file);

        for (int i = 0; i < 4; i++){
            if (strcmp(mbr.mbr_partition[i].part_name,name.toStdString().c_str()) == 0 && mbr.mbr_partition[i].part_status != '-1'){
                posicion= i;
                break;
            }else if (mbr.mbr_partition[i].part_type == 'E' && mbr.mbr_partition[i].part_status != '-1'){
                extendida = i;
            }
        }

        string opcion = "";
        cout << "Desea Eliminar la Particion? Y | N" << endl;
        cin >> opcion;
        if ((opcion.compare("Y") == 0 || opcion.compare("y") == 0)){
             if(posicion != -1){
                 if (Existe_Extendida(mbr,0) == true){// es una extendida
                     mbr.mbr_partition[posicion].part_status = '1';
                     strcpy(mbr.mbr_partition[posicion].part_name,"");
                     fseek(file,0,SEEK_SET);
                     fwrite(&mbr,sizeof(MBR),1,file);
                     if (tipo == "full" ){
                         fseek(file,mbr.mbr_partition[posicion].part_start,SEEK_SET);
                         fwrite(&buffer,1,mbr.mbr_partition[posicion].part_size,file);
                     }
                     cout << " Particion Extendida Eliminada con exito " << endl;
                     fseek(file , 0 , SEEK_SET);
                     fclose(file);

                 }else {// es una primaria
                     mbr.mbr_partition[posicion].part_status = '1';
                     strcpy(mbr.mbr_partition[posicion].part_name,"");
                     fseek(file,0,SEEK_SET);
                     fwrite(&mbr,sizeof(MBR),1,file);
                     if (tipo == "full" ){
                         fseek(file,mbr.mbr_partition[posicion].part_start,SEEK_SET);
                         fwrite(&buffer,1,mbr.mbr_partition[posicion].part_size,file);
                     }
                     cout << "Particion Primaria Eliminada con exito " << endl;
                     fseek(file , 0 , SEEK_SET);
                     fclose(file);

                 }


        }else {// es una logica
            for (int i = 0; i < 4; i++){
                if (mbr.mbr_partition[i].part_type == 'E'){
                    EBR ebr;
                    fseek(file, mbr.mbr_partition[i].part_start, SEEK_SET);
                    fread(&ebr, sizeof(EBR), 1, file);
                    if (strcmp(ebr.part_name,name.toStdString().c_str()) == 0){
                        ebr.part_status='1';
                        strcpy(ebr.part_name,"\0");
                        fseek(file,ftell(file)-sizeof(ebr),SEEK_SET);
                        fwrite(&ebr,sizeof(ebr),1,file);
                        if (tipo == "full"){
                            fwrite(&buffer,1,ebr.part_size,file);
                        }
                        cout << "Particion Logica Eliminada con exito " << endl;
                        fseek(file , 0 , SEEK_SET);
                        fclose(file);
                    }else cout << "No se encuentra la particion: " << name.toStdString() << endl;
                    break;

                }
            }
        }
      }else if (opcion.compare("N") == 0 || opcion.compare("n") == 0) cout << "Ha decidido no borrar la particion " << endl;
       else cout << "NO se reconoce la opcion " << endl;
    }else cout << "Disco no encontrado "<< endl;

}

int  Fdisk::BuscarnamePart(QString name,MBR mbr){
    int primaria= -1;
    int extendida = -1;
    for (int i = 0; i < 4; i++){
        if (strcmp(mbr.mbr_partition[i].part_name,name.toStdString().c_str()) == 0 && mbr.mbr_partition[i].part_status != '-1'){
            primaria= i;
            return primaria;
        }else if (mbr.mbr_partition[i].part_type == 'E' && mbr.mbr_partition[i].part_status != '-1'){
            extendida = i;
            return extendida;
        }
    }
    return -1;
}
bool Fdisk::Existe_Extendida(MBR m,int pos){
    for(int x = 0; x < 4; x++){
            if(m.mbr_partition[x].part_type == 'E'){
                pos = x;
                return true;
            }
        }
        return false;
}

int Fdisk::pos_Extendida(MBR m){
    int pos = -1;
    for(int x = 0; x < 4; x++){
            if(m.mbr_partition[x].part_type == 'E'){
                pos = x;
                return pos;
            }
        }
        return -1;
}

int posicion_logica(QString path){
    MBR disco;
    int retorno = 0;
    int contador = -1;
    FILE *file;
    file = fopen(path.toStdString().c_str(),"rb");
    if (file != nullptr){
        fseek(file,0,SEEK_SET);
        fread(&disco,sizeof(MBR),1,file);
        for (int i = 0; i < 4; i++){
            if (disco.mbr_partition[i].part_type == 'E'){
                contador = i;
                break;
            }
        }

    }else cout << "No existe el disco" <<endl;
    if (contador != -1){
        fseek(file,disco.mbr_partition[contador].part_start,SEEK_SET);
        int a = -1;
        int b = disco.mbr_partition[contador].part_start;
        do{
            //fseek(file,disco.mbr_partition[contador].part_start,SEEK_SET);
            EBR ebr;
            fread(&ebr,sizeof(EBR),1,file);
            a = ebr.part_next;
            if (ebr.part_status == '0'){
                retorno = retorno + 1;
            }
            b = ebr.part_next;
        }while(a != -1);

    }else retorno = -1;
    return retorno;

}

void Fdisk::LLenarEBR(EBR ebr,char fit,QString name,int inicio, int next,int tam){
    ebr.part_status = '0';
    if (fit != 0) ebr.part_fit = fit;
    else ebr.part_fit = 'WF';
    ebr.part_start = inicio;
    ebr.part_size = tam;
    ebr.part_next = next;
    strcpy(ebr.part_name, name.toStdString().c_str());

}

