#include "login.h"

Login::Login()
{

}

void Login::Loguear(int index,FILE *disco,MBR auxmbr,QString direccion,QString usr,QString pass,USERLOG userlog,int index2,int tipo,EBR auxebr){
    superBloque super;
    inodeTable inodo;
    string dic = direccion.toStdString();
    if (tipo == 1){
        fseek(disco,auxmbr.mbr_partition[index].part_start,SEEK_SET);
        int inicioI = super.s_inode_start + sizeof(inodeTable);
        int inicioB = super.s_block_start;

        fread(&super,sizeof(superBloque),1,disco);
        fseek(disco,super.s_inode_start+sizeof(inodeTable),SEEK_SET);

        fread(&inodo,sizeof(inodeTable),1,disco);
        fseek(disco,super.s_inode_start+sizeof(inodeTable),SEEK_SET);

        time_t t = time(nullptr);
        tm *now = localtime(&t);
        string dateC = to_string(now->tm_mday) + "/" + to_string((now->tm_mon+1)) + "/" + to_string((now->tm_year + 1900)) + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min);
        strcpy(inodo.i_atime, dateC.c_str());

        fwrite(&inodo,sizeof(inodeTable),1,disco);
        fclose(disco);
        QString contenido = retornarContent(dic.c_str(),super.s_inode_start + sizeof(inodeTable),super.s_block_start);
        QStringList lista1 = contenido.split('\n');
        QString grupo = "";
        for(QString aux : lista1)
        {
            QStringList lista2 = aux.split(',');
            string co = lista2.value(1).toStdString();
            if( co == " U"){
                string usuario = " " + usr.toStdString();
                if(lista2.value(3).toStdString() == usuario){
                    string password = " " + pass.toStdString();
                    if (lista2.value(4).toStdString() == password){
                        userlog.id_user = lista2.value(0).toInt();
                        grupo = lista2.value(2);
                        cout << "Sesion Iniciada con exito. Con el nombre de usuario:"<< lista2.value(3).toStdString() <<endl;
                    }else {
                        cout << "ERROR contrasena mal escrita" << endl;
                    }
                }else cout << "Error EL usuario no existe" << endl;
            }
        }
        for(QString aux : lista1)
        {
            QStringList lista2 = aux.split(',');
            string co = lista2.value(1).toStdString();
            if( co == " G"){
                if(lista2.value(2) == grupo){
                    userlog.id_grup = lista2.value(0).toInt();
                }else cout << "Error EL Grupo no existe" << endl;
            }
        }
    }else {

        fseek(disco,index2 + sizeof(auxebr),SEEK_SET);

        fread(&super,sizeof(superBloque),1,disco);
        fseek(disco,super.s_inode_start+sizeof(inodeTable),SEEK_SET);

        fread(&inodo,sizeof(inodeTable),1,disco);
        fseek(disco,super.s_inode_start+sizeof(inodeTable),SEEK_SET);

        time_t t = time(nullptr);
        tm *now = localtime(&t);
        string dateC = to_string(now->tm_mday) + "/" + to_string((now->tm_mon+1)) + "/" + to_string((now->tm_year + 1900)) + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min);
        strcpy(inodo.i_atime, dateC.c_str());

        fwrite(&inodo,sizeof(inodeTable),1,disco);
        fclose(disco);
        QString contenido = retornarContent(dic.c_str(),super.s_inode_start + sizeof(inodeTable),super.s_block_start);
        QStringList lista1 = contenido.split('\n');
        QString grupo = "";
        for(QString aux : lista1)
        {
            QStringList lista2 = aux.split(',');
            string co = lista2.value(1).toStdString();
            if( co == " U"){
                string usuario = " " + usr.toStdString();
                if(lista2.value(3).toStdString() == usuario){
                    string password = " " + pass.toStdString();
                    if (lista2.value(4).toStdString() == password){
                        userlog.id_user = lista2.value(0).toInt();
                        grupo = lista2.value(2);
                        cout << "Sesion Iniciada con exito. Con el nombre de usuario:"<< lista2.value(3).toStdString() <<endl;
                    }else {
                        cout << "ERROR contrasena mal escrita" << endl;
                    }
                }else cout << "Error EL usuario no existe" << endl;
            }
        }
        for(QString aux : lista1)
        {
            QStringList lista2 = aux.split(',');
            string co = lista2.value(1).toStdString();
            if( co == " G"){
                if(lista2.value(2) == grupo){
                    userlog.id_grup = lista2.value(0).toInt();
                }else cout << "Error EL Grupo no existe" << endl;
            }
        }



    }

}

QString Login::retornarContent(QString direccion,int inicioI,int InicioB){
    char* result="";
    FILE *disco = fopen(direccion.toStdString().c_str(),"rb+");
    FILE *discoaux =fopen(direccion.toStdString().c_str(),"rb+");
    char cadena[300]="\0";
    QString contenido = "";
    if(disco != nullptr){
        if (discoaux != nullptr){
            inodeTable inodo;
            fseek(disco,inicioI,SEEK_SET);
            fread(&inodo,sizeof(inodeTable),1,disco);
            int flag = 0;
            int contador = 0;
            while(flag == 0){
                if (inodo.i_block[contador] != -1){
                    char fase[64] = "";
                    memset(fase,0,sizeof(fase));
                    bloqueArchivo archivo;
                    fseek(discoaux,InicioB,SEEK_SET);
                    fread(&archivo,sizeof(bloqueArchivo),1,discoaux);
                    for(int j = 0; j < inodo.i_block[contador]; j++){
                        fread(&archivo,sizeof(bloqueArchivo),1,discoaux);
                     }
                    QString aux(archivo.b_content);
                    contenido.append(aux);

                }else flag = 1;
                contador++;
            }
            char aux;

        }else cout << "El disco no existe" << endl;
        fclose(disco);
        fclose(discoaux);
    }else cout << "El disco no existe" << endl;
    return contenido;
}
