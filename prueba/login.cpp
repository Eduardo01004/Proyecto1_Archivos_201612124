#include "login.h"

Login::Login()
{
    fit = 0;
    inicioSuper = 0;
    id_grp = 0;
    id_user = 0;
    path = "";

}

void Login::Loguear(int index,FILE *disco,MBR auxmbr,QString direccion,QString usr,QString pass,USERLOG userlog,int index2,int tipo,EBR auxebr){
    superBloque super;
    inodeTable inodo;
    string dic = direccion.toStdString();
    if (tipo == 1){
        fseek(disco,auxmbr.mbr_partition[index].part_start,SEEK_SET);

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
        inicioSuper = auxmbr.mbr_partition[index].part_start;
        fit = auxmbr.mbr_partition[index].part_fit;
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
                        path = direccion;
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
    else {

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
        inicioSuper = index2 + sizeof(EBR);
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

int Login::BuscarG(QString name, int inicio){
    superBloque super;
    inodeTable inodo;
    FILE *disco;
    disco = fopen(path.toStdString().c_str(),"rb+");
    fseek(disco,inicio,SEEK_SET);

    fread(&super,sizeof(superBloque),1,disco);
    fseek(disco,super.s_inode_start+sizeof(inodeTable),SEEK_SET);

    fread(&inodo,sizeof(inodeTable),1,disco);
    fseek(disco,super.s_inode_start+sizeof(inodeTable),SEEK_SET);
    fclose(disco);

    QString contenido = retornarContent(path,super.s_inode_start + sizeof(inodeTable),super.s_block_start);
    cout << contenido.toStdString() << endl;
    QStringList lista1 = contenido.split('\n');
    int grup;
    int grupo = 1;
    for(QString aux : lista1)
    {
        QStringList lista2 = aux.split(',');
        string co = lista2.value(1).toStdString();

        if(co == " G"){
            string caca = " " + name.toStdString();
            if (lista2.value(2).toStdString() == caca){
                return -1;
            }else {
                grup = atoi(lista2.value(0).toStdString().c_str()) + 1;
                grupo = grupo + 1;
                //cout << "grup: "<< grup++ << endl;
                //return grupo;
            }
        }
    }




    return grupo;

}

int Login::CrearGrupo(FILE *disco2,char contenido[64],QString name){
    inodeTable inodo;
    inodeTable inodoUsuario;
    bloqueArchivo archivo;
    superBloque super;
    int posicion = 0;
    int usando = 0;
    int espaciolibre = 0;
    char cadena_obtenida[400];
    int tam_actual = 0;
    int espacio = -1;
    int bloquelibre;

    FILE*disco;
    disco = fopen(name.toStdString().c_str(),"r+b");

    fseek(disco,inicioSuper,SEEK_SET);
    fread(&super,sizeof(superBloque),1,disco);

    fseek(disco,super.s_inode_start+sizeof(inodeTable),SEEK_SET);
    fread(&inodo,sizeof(inodeTable),1,disco);

    for (int i = 0; i < 12; i++){
        if (inodo.i_block[i] != -1){
            posicion = inodo.i_block[i];

        }else break;
    }
    memset(cadena_obtenida,0,sizeof(cadena_obtenida));
    fseek(disco,super.s_block_start + sizeof(bloqueArchivo)*posicion,SEEK_SET);
    fread(&archivo,sizeof(bloqueArchivo),1,disco);

    usando = retornartam(strlen(archivo.b_content));
    strcpy(cadena_obtenida,contenido);
    tam_actual=strlen(cadena_obtenida);

    if (tam_actual <= usando){
        int respuesta = gr.llenar1(disco,super,archivo,inodoUsuario,posicion,cadena_obtenida,tam_actual);
        return respuesta;
    }else {
       char fase1[40]="";
       char fase2[40]="";
       int registros = 0;
       for(registros = 0; registros < usando; registros++){
           char aux[4]={0};
           sprintf(aux,"%c",cadena_obtenida[registros]);
           strcat(fase1,aux);
       }
       strcat(archivo.b_content,fase1);
       for(registros = registros; registros < tam_actual; registros++){
           char aux[4]={0};
           sprintf(aux,"%c",cadena_obtenida[registros]);
           strcat(fase2,aux);
       }
       int respuesta = gr.llenar2(disco,super,archivo,inodoUsuario,posicion,fase2,tam_actual,inicioSuper,'F');
       return respuesta;

    }
}

void Login::marcar(superBloque super,FILE *disco){
    fseek(disco,super.s_bm_block_start,SEEK_SET);
    char buffer;
    for (int x = 0; x < super.s_blocks_count; x++){
        fread(&buffer,sizeof(char),1,disco);
        if (buffer == '0'){
            buffer = '1';
            fseek(disco,-sizeof(char),SEEK_CUR);
            fwrite(&buffer,sizeof(char),1,disco);
            return;
        }
    }

}
int Login::retornarinodo(superBloque super, FILE*disco){
     char buffer;
    fseek(disco,super.s_bm_inode_start,SEEK_SET);
    for (int x = 0; x < super.s_inodes_count; x++){
        fread(&buffer,sizeof(char),1,disco);
        if (buffer == '0'){
            return x;
        }
    }

}


int Login::retornartam(int numero){
    return 63 - numero;

}
