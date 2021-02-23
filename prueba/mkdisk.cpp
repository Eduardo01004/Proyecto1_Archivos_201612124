#include "mkdisk.h"
    #include <QDir>

int pathmkdisk = 0;
int sizemkdisk = 0;
int fitmkdisk = 0;
int unitmkdisk = 0;

int tammkdisk = 0;
QString pathmk = "";
QString umk = "";
char fitmk = 0;

Mkdisk::Mkdisk()
{

}

void Mkdisk:: automataMkdisk(QString lexema,QString token,int flag){

    switch (estadomkdisk) {
        case 0:
            if (lexema == "mkdisk") estadomkdisk = 1;
            else {
                cout << " No se reconoce el comando: 2 " << token.toStdString()<< endl;
                estadomkdisk = 1000;
            }
        break;

        case 1:
            if (lexema == "size") estadomkdisk = 2;

            else if (token == "finInstruccion") {
                if (pathmkdisk){
                    if (sizemkdisk){
                        flag = 0;
                        estadomkdisk = 0;
                        int total = 1024;
                        CrearDisco(pathmk);
                        MBR mb;
                        mb.mbr_fecha_creacion = time(0);
                        mb.mbr_disksignature =(int)time(nullptr);
                        if (unitmkdisk == 0){
                            mb.mbr_tamano = tammkdisk * 1048576;
                            total = tammkdisk *1024;
                        }else {
                            if (umk == "m" || umk == "M"){
                                mb.mbr_tamano = tammkdisk * 1048576;
                                total = tammkdisk * 1024;
                            }else {
                                mb.mbr_tamano = tammkdisk * 1024;
                                total = tammkdisk;
                            }
                        }

                        if (fit_mkdisk == 1){
                            mb.disk_fit = fitmk;
                            cout << fitmk << endl;
                        }else mb.disk_fit = 'F';

                        for (int i = 0; i < 4; i++){
                           mb.mbr_partition[i].part_status = '0';
                           mb.mbr_partition[i].part_type = '0';
                           mb.mbr_partition[i].part_fit = '0';
                           mb.mbr_partition[i].part_size = 0;
                           mb.mbr_partition[i].part_start = -1;
                           strcpy(mb.mbr_partition[i].part_name,"");
                       }
                        string comando = "dd if=/dev/zero of=\""+pathmk.toStdString()+"\" bs=1024 count="+ std::to_string(total);
                        system(comando.c_str());
                        FILE *fp = fopen(pathmk.toStdString().c_str(),"rb+");
                        fseek(fp,0,SEEK_SET);
                        fwrite(&mb,sizeof(MBR),1,fp);
                        fclose(fp);
                        cout << endl;
                        cout << "Disco creado con exito" << endl;


                    }else {
                        cout << " Error el comando size no esta definido" <<  endl;
                        estadomkdisk = 0;
                        flag = 0;

                    }

                }else {
                    cout << " Error el comando path no esta definido" <<  endl;
                    estadomkdisk = 0;
                    flag = 0;
                }

            }else if (lexema == "path") estadomkdisk = 3;
            else if (lexema == "unit") estadomkdisk = 4;
            else if (lexema == "fit") estadomkdisk = 5;
        break;
        case 2:
            if (lexema == "numero"){
                tammkdisk = token.toInt();
                if (tammkdisk > 0){
                    estadomkdisk = 1;
                    sizemkdisk = 1;
                }else cout << " Size tiene que ser mayor a 0" <<  endl;

            }else {
                cout << " Error tiene que ser un numero " <<  endl;
                estadomkdisk = 0;
                flag = 0;
            }
        break;
        case 3:
            if (lexema == "palabra"){
                pathmkdisk = 1;
                estadomkdisk = 1;
                pathmk = token;
            }else {
                cout << " No se reconoce la direccion" <<  endl;
                estadomkdisk = 0;
                flag = 0;
            }
        break;
        case 4:
            if (lexema == "palabra"){
                if (token == "k" || token == "K"){
                    umk = token;
                    estadomkdisk = 1;
                    unitmkdisk = 1;
                }else if (token == "m" || token == "M"){
                    umk = token;
                    estadomkdisk = 1;
                    unitmkdisk = 1;

                }else {
                    cout << " No se reconoce el parametro unit" <<  endl;
                    estadomkdisk = 0;
                    flag = 0;
                }
            }else {
                cout << " No se reconoce el parametro unit" <<  endl;
                estadomkdisk = 0;
                flag = 0;
            }
        break;

        case 5:
            estadomkdisk = 1;
            flag = 0;
            fit_mkdisk = 1;
            if (lexema == "palabra"){
                if(token == "bf"  || token == "BF" || token == "Bf") fitmk = 'B';
                else if (token == "ff" || token == "FF" || token == "Ff" ) fitmk = 'F';
                else if (token == "wf" || token == "WF" || token == "Wf") fitmk = 'W';
            }else {
                cout << " No se reconoce el parametro fit" <<  endl;
                estadomkdisk = 0;
                flag = 0;
            }
        break;
    }

}


QString retornarPath(QString path){
    QStringList ruta = path.split('/', QString::SkipEmptyParts);
    QString Directorio = "";
    for(int a = 0; a < ruta.count() - 1; a++){
        Directorio += "/" + ruta[a];
        DIR *dir = opendir(Directorio.toUtf8());
        if(!dir){
            QString comando = "mkdir " + Directorio;
            auto result = mkdir(Directorio.toUtf8().constData(),  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        }
    }
    return Directorio;

}
void Mkdisk:: CrearDisco(QString direccion){
    QString rut = retornarPath(direccion);
    string c = "sudo mkdir -p \'"+rut.toStdString()+"\'";
    system(c.c_str());
    string c2 = "sudo chmod -R 777 \'"+rut.toStdString()+"\'";
    system(c2.c_str());
}
