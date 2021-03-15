#include "login.h"

Login::Login()
{
    fit = 0;
    inicioSuper = 0;
    id_grp = 0;
    id_user = 0;
    path = "";
    inicio_journal = 0;
    usuario = "";

}

int Login::Loguear(int index,FILE *disco,MBR auxmbr,QString direccion,QString usr,QString pass,USERLOG userlog,int index2,int tipo,EBR auxebr){
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
        fit = 'F';//auxmbr.mbr_partition[index].part_fit;
        inicio_journal = auxmbr.mbr_partition[index].part_start + sizeof(superBloque);

        QString contenido = retornarContent(dic.c_str(),super.s_inode_start + sizeof(inodeTable),super.s_block_start);
        QStringList lista1 = contenido.split('\n');
        QString grupo = "";
        for(QString aux : lista1)
        {
            QStringList lista2 = aux.split(',');
            string co = lista2.value(1).toStdString();
            if( co == "U"){
                string usuario = "" + usr.toStdString();
                cout << usuario << " == " << lista2.value(3).toStdString() << endl;
                if(lista2.value(3).toStdString() == usuario){
                    string password = "" + pass.toStdString();
                    if (lista2.value(4).toStdString() == password){
                        userlog.id_user = lista2.value(0).toInt();
                        grupo = lista2.value(2);
                        path = direccion;
                        cout << "Sesion Iniciada con exito. Con el nombre de usuario:"<< lista2.value(3).toStdString() <<endl;
                        return 1;
                    }else {

                        cout << "ERROR contrasena mal escrita" << endl;
                        return 2;
                    }
                }
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
                        id_user = lista2.value(0).toInt();
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
                    id_grp =lista2.value(0).toInt();
                }else cout << "Error EL Grupo no existe" << endl;
            }
        }



    }

}

QString Login::retornarContent(QString direccion,int inicioI,int InicioB){
    char* result="";
    FILE *disco = fopen(direccion.toStdString().c_str(),"rb+");
    FILE *discoaux =fopen(direccion.toStdString().c_str(),"rb+");
    QString contenido = "";
    if(disco != nullptr){
        if (discoaux != nullptr){
            superBloque super;
            inodeTable inodo;
            fseek(disco,inicioSuper,SEEK_SET);
            fread(&super,sizeof(superBloque),1,disco);
            fseek(disco,super.s_inode_start + static_cast<int>(sizeof(inodeTable)),SEEK_SET);
            fread(&inodo,sizeof(inodeTable),1,disco);
            int flag = 0;
            int contador = 0;
            //while(flag == 0){
                for (int i =0; i <15; i++){
                if (inodo.i_block[i] != -1){

                    bloqueArchivo archivo;
                    fseek(discoaux,super.s_block_start,SEEK_SET);
                    fread(&archivo,sizeof(bloqueArchivo),1,discoaux);
                    for(int j = 0; j < inodo.i_block[i]; j++){
                        fread(&archivo,sizeof(bloqueArchivo),1,discoaux);
                     }
                    QString aux(archivo.b_content);
                    contenido.append(aux);

                }
                //contador++;
            }

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
    //fseek(disco,super.s_inode_start+sizeof(inodeTable),SEEK_SET);
    fclose(disco);

    QString contenido = retornarContent(path,super.s_inode_start + sizeof(inodeTable),super.s_block_start);
    //cout << contenido.toStdString() << endl;
    QStringList lista1 = contenido.split('\n');
    int grup;
    int grupo = 1;
    for(QString aux : lista1)
    {
        QStringList lista2 = aux.split(',');
        string co = lista2.value(1).toStdString();

        if(co == "G"){
            string caca = "" + name.toStdString();
            if (lista2.value(2).toStdString() == caca){
                return -1;
            }else {
                grup = atoi(lista2.value(0).toStdString().c_str()) + 1;
                grupo = grupo + 1;
            }
        }
    }




    return grupo;

}

int Login::CrearGrupo(FILE *disco,QString  contenido,QString name){
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

    fseek(disco,inicioSuper,SEEK_SET);
    fread(&super,sizeof(superBloque),1,disco);

    fseek(disco,super.s_inode_start+sizeof(inodeTable),SEEK_SET);
    fread(&inodo,sizeof(inodeTable),1,disco);

    for (int i = 0; i < 12; i++){
        if (inodo.i_block[i] != -1){
            posicion = inodo.i_block[i];

        }
    }
    memset(cadena_obtenida,0,sizeof(cadena_obtenida));
    fseek(disco,super.s_block_start + sizeof(bloqueArchivo)*posicion,SEEK_SET);
    fread(&archivo,sizeof(bloqueArchivo),1,disco);

    usando = retornartam(strlen(archivo.b_content));
    strcpy(cadena_obtenida,contenido.toStdString().c_str());
    tam_actual = strlen(cadena_obtenida);
    if (tam_actual <= usando){
        int respuesta = gr.llenar1(disco,super,archivo,inodoUsuario,posicion,cadena_obtenida,tam_actual);
        return 1;
    }else {
       char fase1[40]="";
       char fase2[40]="";
       memset(fase1,0,sizeof(fase1));
       memset(fase2,0,sizeof(fase2));
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
       //int respuesta = gr.llenar2(disco,super,archivo,inodoUsuario,posicion,fase2,tam_actual,inicioSuper,'F');
       int superblock = super.s_block_start;
       int superinodo = super.s_inode_start;
       int bitlibe;
      fseek(disco,super.s_block_start + (sizeof(bloqueArchivo) * posicion),SEEK_SET);
      fwrite(&archivo,sizeof(bloqueArchivo),1,disco);
      bloqueArchivo archAux;
      strcpy(archAux.b_content,fase2);
      if (fit == 'F'){
          int bitlibe2 = gr.firsFit(disco,inicioSuper);
          if (bitlibe2 == -1) cout << " no se puede crear " <<endl;
          else bitlibe = gr.firsFit(disco,inicioSuper);

      }else if (fit == 'W'){

      }else if (fit == 'B'){

      }
      fseek(disco,super.s_block_start+ (sizeof(bloqueArchivo)*bitlibe),SEEK_SET);
      fwrite(&archAux,sizeof(bloqueArchivo),1,disco);

      fseek(disco,super.s_bm_block_start + (bitlibe*sizeof(char)),SEEK_SET);
      fputc('2',disco);

      fseek(disco,super.s_inode_start + sizeof(inodeTable),SEEK_SET);
      fread(&inodoUsuario,sizeof(inodeTable),1,disco);

      inodoUsuario.i_size = inodoUsuario.i_size + tam_actual;
      time_t t = time(nullptr);
      tm *now = localtime(&t);
      string dateC = to_string(now->tm_mday) + "/" + to_string((now->tm_mon+1)) + "/" + to_string((now->tm_year + 1900)) + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min);
      strcpy(inodoUsuario.i_mtime, dateC.c_str());
      inodoUsuario.i_block[posicion] = bitlibe;

      fseek(disco,super.s_inode_start+ sizeof(inodeTable),SEEK_SET);
      fwrite(&inodoUsuario,sizeof(inodeTable),1,disco);

      int superfi = super.s_first_blo;
      int superfree = super.s_free_block_count;

      super.s_first_blo = super.s_first_blo + 1;
      super.s_free_block_count = super.s_free_block_count- 1;
      fseek(disco,inicioSuper,SEEK_SET);
      fwrite(&super,sizeof(superBloque),1,disco);
      return 1;
    }
    return 1;
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


int Login::BuscarU(QString name, int inicio){
    superBloque super;
    inodeTable inodo;
    FILE *disco;
    disco = fopen(path.toStdString().c_str(),"rb+");
    fseek(disco,inicio,SEEK_SET);

    fread(&super,sizeof(superBloque),1,disco);
    fseek(disco,super.s_inode_start+sizeof(inodeTable),SEEK_SET);

    fread(&inodo,sizeof(inodeTable),1,disco);
    //fseek(disco,super.s_inode_start+sizeof(inodeTable),SEEK_SET);
    //fclose(disco);

    QString contenido = retornarContent(path,super.s_inode_start + sizeof(inodeTable),super.s_block_start);
    //cout << contenido.toStdString() << endl;
    QStringList lista1 = contenido.split('\n');
    int grup;
    int grupo = 1;
    for(QString aux : lista1)
    {
        QStringList lista2 = aux.split(',');
        string co = lista2.value(1).toStdString();
        if(co == "U"){
            string caca = "" + name.toStdString();
            if (lista2.value(3).toStdString() == caca){
                return -1;
            }else {
                usuario = lista2.value(3);

                grup = atoi(lista2.value(0).toStdString().c_str()) + 1;
                grupo = grupo + 1;
            }
        }

    }

    return grupo;

}
