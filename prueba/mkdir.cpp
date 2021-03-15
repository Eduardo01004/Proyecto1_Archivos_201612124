#include "mkdir.h"

Mkdir::Mkdir()
{

}

QString Mkdir::RetornarRuta(QString ruta){
    QString path = ruta;
    path.replace("\"","");
    QStringList p = path.split('/');

    QString name = p.last();
    return name;

}

int Mkdir::BuscarCoA(FILE *DiscoEnUSo,char *path, int inicio,int *numeracion){
    inodeTable InodoUso;
    bloqueCarpetas BC;
    superBloque sb;
    /*QString path = ruta;
    path.replace("\"","");
    QStringList p = path.split('/');*/
    int pos = 0;
    char *listapath[100];
    memset(listapath,0,sizeof(listapath));
    const char s[2] = "\/";
    char *token;
    token = strtok(path, s);

    int LugarInododo = 0;

    int contador=0;
    QList<string> lista = QList<string>();
    while(token != nullptr){
            contador = contador +1;
            lista.append(token);
            token = strtok(nullptr,"/");
        }

    fseek(DiscoEnUSo,inicio,SEEK_SET);
    fread(&sb,sizeof(superBloque),1,DiscoEnUSo);
    LugarInododo = sb.s_inode_start;
    for (int i = 0; i < contador; i++){
        fseek(DiscoEnUSo,LugarInododo,SEEK_SET);//
        fread(&InodoUso,sizeof(inodeTable),1,DiscoEnUSo);
        bool flag = false;
        for (int j = 0; j < 12; j++){
            if (InodoUso.i_block[j] != -1){
                fseek(DiscoEnUSo,inicio,SEEK_SET);
                fread(&sb,sizeof(superBloque),1,DiscoEnUSo);
                fseek(DiscoEnUSo,sb.s_block_start + (sizeof(bloqueCarpetas) * InodoUso.i_block[j]),SEEK_SET);
                fread(&BC,sizeof(bloqueCarpetas),1,DiscoEnUSo);
                for (int y = 0; y < 4; y++){
                    /*QString name = "";
                    for(char c : BC.b_content[y].b_name)
                    {
                        if(c != NULL)
                        {
                        name.append(c);
                        }
                    }*/
                    if ((i == contador - 1)&&(strcasecmp(BC.b_content[y].b_name,lista.at(i).c_str()) == 0)){
                        *numeracion = BC.b_content[y].b_inodo;
                        return 1;
                    }else if ((i != contador - 1)&&(strcasecmp(BC.b_content[y].b_name,lista.at(i).c_str()) == 0)) {
                        fseek(DiscoEnUSo,inicio,SEEK_SET);
                        fread(&sb,sizeof(superBloque),1,DiscoEnUSo);
                        LugarInododo = sb.s_inode_start + (sizeof(inodeTable)*BC.b_content[y].b_inodo);
                        flag = true;
                        break;

                    }
                    if (flag == true) {
                        break;
                    }

                }
                if (flag == true ) {
                    break;
                }

            }
        }
    }

    return 0;
}

int Mkdir::CrearCarpeta(FILE *DiscoEnUso,char fit,int indicador,int inicio,char* path,int flag_P,QString path2){
    char Unionpath[500];
        char COPIAUnionpath[500];
        inodeTable InodoUso,OTROBA;
        bloqueCarpetas BC,BC2,NueC;
        superBloque sb;
        int contador = 0;
        int contador2 = 0;
        int iblock = 0;
        int iblock2 = 0;
        int comprobacionRuta = 0;
        int LugarInododo = 0;

        QList<string> lista = QList<string>();
        char copiaPath[500];
        char copiaPath2[500];
        char LarutaDelaCarpeta[500];
        char nombreCarpeta[80];
        char *listapath2[500];
        memset(listapath2,0,sizeof(listapath2));
        strcpy(copiaPath,path);
        strcpy(copiaPath2,path);
        strcpy(LarutaDelaCarpeta,dirname(copiaPath));
        strcpy(copiaPath,path);
        strcpy(nombreCarpeta,basename(copiaPath));
        char *token = strtok(path,"/");
        int cont = 0;

        while(token != nullptr){
                contador = contador +1;
                lista.append(token);
                token = strtok(nullptr,"/");
            }
        fseek(DiscoEnUso,inicio,SEEK_SET);
        fread(&sb,sizeof(superBloque),1,DiscoEnUso);
        if (contador == 1){
                int mover = 0;
                int mover2 = 0;
                int libre = 0;
                fseek(DiscoEnUso,sb.s_inode_start+ (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                fread(&InodoUso,sizeof(inodeTable),1,DiscoEnUso);

                for(iblock=0;iblock<12;iblock++){
                    if(InodoUso.i_block[iblock] != -1){
                        fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueCarpetas)*InodoUso.i_block[iblock]),SEEK_SET);
                        fread(&BC,sizeof(bloqueCarpetas),1,DiscoEnUso);
                        for(iblock2=0;iblock2<4;iblock2++){
                            if(BC.b_content[iblock2].b_inodo != -1){
                                libre = 0;
                            }else{
                                libre = 1;
                                mover = iblock2;
                                mover2 = iblock;
                                break;
                            }
                        }
                    }
                }


                if(libre == 1){
                        char auxbituno = '1';
                        int bitLibre = 0;
                        int bitlibre2 = 0;
                        int a =0;
                        if (fit == 'F'){
                            int bitlibe2 = firsFitInodo(DiscoEnUso,inicio);
                            if (bitlibe2 == -1) cout << " no se puede crear1 " <<endl;
                            else bitLibre = firsFitInodo(DiscoEnUso,inicio);
                        }else if (fit == 'W'){

                        }else if (fit == 'B'){

                         }
                        BC.b_content[mover].b_inodo = bitLibre;
                        strcpy(BC.b_content[mover].b_name,nombreCarpeta);
                        fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueCarpetas)*InodoUso.i_block[mover2]),SEEK_SET);
                        fwrite(&BC,sizeof(bloqueCarpetas),1,DiscoEnUso);
                        OTROBA.i_uid = 0;
                        OTROBA.i_gid = 0;
                        OTROBA.i_size= 0;
                        time_t t = time(nullptr);
                        tm *now = localtime(&t);
                        string dateC = to_string(now->tm_mday) + "/" + to_string((now->tm_mon+1)) + "/" + to_string((now->tm_year + 1900)) + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min);
                        strcpy(OTROBA.i_atime, dateC.c_str());
                        strcpy(OTROBA.i_ctime, dateC.c_str());
                        strcpy(OTROBA.i_mtime, dateC.c_str());
                        if (fit == 'F'){
                            int bitlibe2 = firsFit(DiscoEnUso,inicio);
                            if (bitlibe2 == -1) cout << " no se puede crear2 " <<endl;
                            else bitlibre2 = firsFit(DiscoEnUso,inicio);
                        }else if (fit == 'W'){

                        }else if (fit == 'B'){

                        }
                        OTROBA.i_block[0] = bitlibre2;
                        for(a = 1; a < 15; a++){
                            OTROBA.i_block[a] = -1;
                        }
                        OTROBA.i_type = '0';
                        OTROBA.i_perm = 664;
                        fseek(DiscoEnUso,sb.s_inode_start +(sizeof(inodeTable)*bitLibre),SEEK_SET);
                        fwrite(&OTROBA,sizeof(inodeTable),1,DiscoEnUso);
                        BC2.b_content[0].b_inodo = bitLibre;
                        strcpy(BC2.b_content[0].b_name,".");
                        BC2.b_content[1].b_inodo = LugarInododo;
                        strcpy(BC2.b_content[1].b_name,"..");
                        BC2.b_content[2].b_inodo = -1;
                        strcpy(BC2.b_content[2].b_name,"");
                        BC2.b_content[3].b_inodo = -1;
                        strcpy(BC2.b_content[3].b_name,"");
                        fseek(DiscoEnUso,sb.s_block_start +(sizeof(bloqueCarpetas)*bitlibre2),SEEK_SET);
                        fwrite(&BC2,sizeof(bloqueCarpetas),1,DiscoEnUso);
                        fseek(DiscoEnUso,sb.s_bm_block_start + (bitlibre2*sizeof(char)),SEEK_SET);
                        fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                        fseek(DiscoEnUso,sb.s_bm_inode_start + (bitLibre*sizeof(char)),SEEK_SET);
                        fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                        fseek(DiscoEnUso,inicio,SEEK_SET);
                        fread(&sb,sizeof(superBloque),1,DiscoEnUso);
                        sb.s_free_block_count = sb.s_free_block_count - 1;
                        sb.s_free_inodes_count = sb.s_free_inodes_count - 1;
                        sb.s_first_ino = sb.s_first_ino +1;
                        sb.s_first_blo = sb.s_first_blo +1;
                        fseek(DiscoEnUso,inicio,SEEK_SET);
                        fwrite(&sb,sizeof(superBloque),1,DiscoEnUso);
                        strcpy(Unionpath,copiaPath2);//sas
                        return 6;
                }else if (libre == 0){
                    char auxbituno = '1';
                    int bitLibre = 0;
                    int bitlibre2 = 0;
                    int segui = 0;
                    int a = 0;
                    if (fit == 'F'){
                        int bitlibe2 = firsFit(DiscoEnUso,inicio);
                        if (bitlibe2 == -1) cout << " no se puede crear3 " <<endl;
                        else bitlibre2 = firsFit(DiscoEnUso,inicio);
                    }else if (fit == 'W'){

                    }else if (fit == 'B'){

                    }
                    fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                    fread(&InodoUso,sizeof(inodeTable),1,DiscoEnUso);
                    for(int segui = 0 ; segui < 12 ; segui++){
                        if(InodoUso.i_block[segui] == -1){
                            iblock2 = segui;
                            break;
                        }
                    }
                        InodoUso.i_block[iblock2]=bitlibre2;
                        fseek(DiscoEnUso,sb.s_inode_start,SEEK_SET);
                        fwrite(&InodoUso,sizeof(inodeTable),1,DiscoEnUso);
                        fseek(DiscoEnUso,sb.s_bm_block_start + (bitlibre2*sizeof(char)),SEEK_SET);
                        fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                        if (fit == 'F'){
                            int bitlibe2 = firsFitInodo(DiscoEnUso,inicio);
                            if (bitlibe2 == -1) cout << " no se puede crear4 " <<endl;
                            else bitLibre = firsFitInodo(DiscoEnUso,inicio);
                        }else if (fit == 'W'){

                        }else if (fit == 'B'){

                        }
                        NueC.b_content[0].b_inodo = bitLibre;
                        strcpy(NueC.b_content[0].b_name,nombreCarpeta);
                        NueC.b_content[1].b_inodo =-1;
                        strcpy(NueC.b_content[1].b_name,"");
                        NueC.b_content[2].b_inodo = -1;
                        strcpy(NueC.b_content[2].b_name,"");
                        NueC.b_content[3].b_inodo = -1;
                        strcpy(NueC.b_content[3].b_name,"");
                        fseek(DiscoEnUso,sb.s_block_start +(sizeof(bloqueCarpetas)*bitlibre2),SEEK_SET);
                        fwrite(&NueC,sizeof(bloqueCarpetas),1,DiscoEnUso);
                        fseek(DiscoEnUso,sb.s_bm_inode_start + (bitLibre*sizeof(char)),SEEK_SET);
                        fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                        OTROBA.i_uid = 0;
                        OTROBA.i_gid = 0;
                        OTROBA.i_size= 0;
                        time_t t = time(nullptr);
                        tm *now = localtime(&t);
                        string dateC = to_string(now->tm_mday) + "/" + to_string((now->tm_mon+1)) + "/" + to_string((now->tm_year + 1900)) + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min);
                        strcpy(OTROBA.i_atime, dateC.c_str());
                        strcpy(OTROBA.i_ctime, dateC.c_str());
                        strcpy(OTROBA.i_mtime, dateC.c_str());
                        if (fit == 'F'){
                            int bitlibe2 = firsFit(DiscoEnUso,inicio);
                            if (bitlibe2 == -1) cout << " no se puede crear5 " <<endl;
                            else bitlibre2 = firsFit(DiscoEnUso,inicio);
                        }else if (fit == 'W'){

                        }else if (fit == 'B'){

                        }
                        OTROBA.i_block[0] = bitlibre2;
                        for(a = 1; a < 15; a++){
                            OTROBA.i_block[a] = -1;
                        }
                        OTROBA.i_type = '0';
                        OTROBA.i_perm = 664;
                        fseek(DiscoEnUso,sb.s_inode_start +(sizeof(inodeTable)*bitLibre),SEEK_SET);
                        fwrite(&OTROBA,sizeof(inodeTable),1,DiscoEnUso);
                        BC2.b_content[0].b_inodo  =bitLibre;
                        strcpy(BC2.b_content[0].b_name,".");
                        BC2.b_content[1].b_inodo = LugarInododo;
                        strcpy(BC2.b_content[1].b_name,"..");
                        BC2.b_content[2].b_inodo = -1;
                        strcpy(BC2.b_content[2].b_name,"");
                        BC2.b_content[3].b_inodo = -1;
                        strcpy(BC2.b_content[3].b_name,"");
                        fseek(DiscoEnUso,sb.s_bm_block_start + (bitlibre2*sizeof(char)),SEEK_SET);
                        fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                        fseek(DiscoEnUso,sb.s_block_start +(sizeof(bloqueCarpetas)*bitlibre2),SEEK_SET);
                        fwrite(&BC2,sizeof(bloqueCarpetas),1,DiscoEnUso);
                        fseek(DiscoEnUso,inicio,SEEK_SET);
                        fread(&sb,sizeof(superBloque),1,DiscoEnUso);
                        sb.s_free_block_count = sb.s_free_block_count - 2;
                        sb.s_free_inodes_count = sb.s_free_inodes_count - 1;
                        sb.s_first_blo = sb.s_first_blo +2;
                        sb.s_first_ino = sb.s_first_ino +1;
                        fseek(DiscoEnUso,inicio,SEEK_SET);
                        fwrite(&sb,sizeof(superBloque),1,DiscoEnUso);
                        strcpy(Unionpath,copiaPath2);
                    return 6;
                }

            }
        else if (contador > 1){
                int SaberExiste = BuscarCoA(DiscoEnUso,LarutaDelaCarpeta,inicio,&LugarInododo);//ptrp
                if(SaberExiste == 0){
                    if(flag_P == 0){
                        return 4;
                    }else{
                        memset(Unionpath,0,sizeof(Unionpath));
                        for(contador2 = 0;contador2<contador;contador2++){
                            strcat(Unionpath, "\/");
                            //strcat(Unionpath,listapath2[contador2]);
                            strcat(Unionpath,lista.at(contador2).c_str());
                            strcpy(COPIAUnionpath,Unionpath);
                            int lovio = BuscarCoA(DiscoEnUso,Unionpath,inicio,&LugarInododo);
                            if(lovio == 0){
                                CrearCarpeta(DiscoEnUso,fit,0,inicio,COPIAUnionpath,0,path2);
                            }
                        }
                    }
                }else if(SaberExiste == 1){
                    int mover = 0;
                    int mover2 = 0;
                    int libre = 0;
                    fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                    fread(&InodoUso,sizeof(inodeTable),1,DiscoEnUso);
                    for(iblock=0;iblock<12;iblock++){
                        if(InodoUso.i_block[iblock] != -1){
                            fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueCarpetas)*InodoUso.i_block[iblock]),SEEK_SET);
                            fread(&BC,sizeof(bloqueCarpetas),1,DiscoEnUso);
                            if(iblock == 0){
                                for(iblock2 = 2; iblock2 < 4; iblock2++){
                                    if(BC.b_content[iblock2].b_inodo != -1){
                                        libre = 0;
                                    }else{
                                        libre = 1;
                                        mover = iblock2;
                                        mover2 = iblock;
                                        break;
                                    }
                                }
                            }else{
                                for(iblock2=0;iblock2<4;iblock2++){
                                    if(BC.b_content[iblock2].b_inodo != -1){
                                        libre = 0;
                                    }else{
                                        libre = 1;
                                        mover = iblock2;
                                        mover2 = iblock;
                                        break;
                                    }
                                }
                            }

                        }
                    }
                    if(libre == 1){
                            char auxbituno = '1';
                            int bitLibre = 0;
                            int bitlibre2 = 0;
                            int a =0;
                            if (fit == 'F'){
                                int bitlibe2 = firsFitInodo(DiscoEnUso,inicio);
                                if (bitlibe2 == -1) cout << " no se puede crear6 " <<endl;
                                else bitLibre = firsFitInodo(DiscoEnUso,inicio);
                            }else if (fit == 'W'){

                            }else if (fit == 'B'){

                            }
                            BC.b_content[mover].b_inodo = bitLibre;
                            strcpy(BC.b_content[mover].b_name,nombreCarpeta);
                            fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueCarpetas)*InodoUso.i_block[mover2]),SEEK_SET);
                            fwrite(&BC,sizeof(bloqueCarpetas),1,DiscoEnUso);
                            OTROBA.i_uid = 0;
                            OTROBA.i_gid = 0;
                            OTROBA.i_size= 0;
                            time_t t = time(nullptr);
                            tm *now = localtime(&t);
                            string dateC = to_string(now->tm_mday) + "/" + to_string((now->tm_mon+1)) + "/" + to_string((now->tm_year + 1900)) + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min);
                            strcpy(OTROBA.i_atime, dateC.c_str());
                            strcpy(OTROBA.i_ctime, dateC.c_str());
                            strcpy(OTROBA.i_mtime, dateC.c_str());
                            if (fit == 'F'){
                                int bitlibe2 = firsFit(DiscoEnUso,inicio);
                                if (bitlibe2 == -1) cout << " no se puede crear7 " <<endl;
                                else bitlibre2 = firsFit(DiscoEnUso,inicio);
                            }else if (fit == 'W'){

                            }else if (fit == 'B'){

                            }
                            OTROBA.i_block[0] = bitlibre2;
                            for(a = 1; a < 15; a++){
                                OTROBA.i_block[a] = -1;
                            }
                            OTROBA.i_type = '0';
                            OTROBA.i_perm = 664;
                            fseek(DiscoEnUso,sb.s_inode_start +(sizeof(inodeTable)*bitLibre),SEEK_SET);
                            fwrite(&OTROBA,sizeof(inodeTable),1,DiscoEnUso);
                            BC2.b_content[0].b_inodo =bitLibre;
                            strcpy(BC2.b_content[0].b_name,".");
                            BC2.b_content[1].b_inodo =LugarInododo;
                            strcpy(BC2.b_content[1].b_name,"..");
                            BC2.b_content[2].b_inodo =-1;
                            strcpy(BC2.b_content[2].b_name,"");
                            BC2.b_content[3].b_inodo =-1;
                            strcpy(BC2.b_content[3].b_name,"");
                            fseek(DiscoEnUso,sb.s_block_start +(sizeof(bloqueCarpetas)*bitlibre2),SEEK_SET);
                            fwrite(&BC2,sizeof(bloqueCarpetas),1,DiscoEnUso);
                            fseek(DiscoEnUso,sb.s_bm_block_start + (bitlibre2*sizeof(char)),SEEK_SET);
                            fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                            fseek(DiscoEnUso,sb.s_bm_inode_start + (bitLibre*sizeof(char)),SEEK_SET);
                            fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                            fseek(DiscoEnUso,inicio,SEEK_SET);
                            fread(&sb,sizeof(superBloque),1,DiscoEnUso);
                            sb.s_free_block_count = sb.s_free_block_count - 1;
                            sb.s_free_inodes_count = sb.s_free_inodes_count - 1;
                            sb.s_first_ino = sb.s_first_ino +1;
                            sb.s_first_blo = sb.s_first_blo +1;
                            fseek(DiscoEnUso,inicio,SEEK_SET);
                            fwrite(&sb,sizeof(superBloque),1,DiscoEnUso);
                            strcpy(Unionpath,copiaPath2);
                            return 6;
                    }else if (libre == 0){
                            char auxbituno = '1';
                            int bitLibre = 0;
                            int bitlibre2 = 0;
                            int segui = 0;
                            int a = 0;
                            if (fit == 'F'){
                                int bitlibe2 = firsFit(DiscoEnUso,inicio);
                                if (bitlibe2 == -1) cout << " no se puede crear8 " <<endl;
                                else bitlibre2 = firsFit(DiscoEnUso,inicio);
                            }else if (fit == 'W'){

                            }else if (fit == 'B'){

                            }
                            fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                            fread(&InodoUso,sizeof(inodeTable),1,DiscoEnUso);
                            for(segui=0;segui <12;segui++){
                                if(InodoUso.i_block[segui]==-1){
                                    iblock2 = segui;
                                    break;
                                }
                            }
                            InodoUso.i_block[iblock2]=bitlibre2;
                            fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                            fwrite(&InodoUso,sizeof(inodeTable),1,DiscoEnUso);
                            fseek(DiscoEnUso,sb.s_bm_block_start + (bitlibre2*sizeof(char)),SEEK_SET);
                            fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                            if (fit == 'F'){
                                int bitlibe2 = firsFitInodo(DiscoEnUso,inicio);
                                if (bitlibe2 == -1) cout << " no se puede crear " <<endl;
                                else bitLibre = firsFitInodo(DiscoEnUso,inicio);
                            }else if (fit == 'W'){

                            }else if (fit == 'B'){

                            }
                            NueC.b_content[0].b_inodo =bitLibre;
                            strcpy(NueC.b_content[0].b_name,nombreCarpeta);
                            NueC.b_content[1].b_inodo =-1;
                            strcpy(NueC.b_content[1].b_name,"");
                            NueC.b_content[2].b_inodo =-1;
                            strcpy(NueC.b_content[2].b_name,"");
                            NueC.b_content[3].b_inodo =-1;
                            strcpy(NueC.b_content[3].b_name,"");
                            fseek(DiscoEnUso,sb.s_block_start +(sizeof(bloqueCarpetas)*bitlibre2),SEEK_SET);
                            fwrite(&NueC,sizeof(bloqueCarpetas),1,DiscoEnUso);
                            fseek(DiscoEnUso,sb.s_bm_inode_start + (bitLibre*sizeof(char)),SEEK_SET);
                            fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                            OTROBA.i_uid = 0;
                            OTROBA.i_gid = 0;
                            OTROBA.i_size= 0;
                            time_t t = time(nullptr);
                            tm *now = localtime(&t);
                            string dateC = to_string(now->tm_mday) + "/" + to_string((now->tm_mon+1)) + "/" + to_string((now->tm_year + 1900)) + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min);
                            strcpy(OTROBA.i_atime, dateC.c_str());
                            strcpy(OTROBA.i_ctime, dateC.c_str());
                            strcpy(OTROBA.i_mtime, dateC.c_str());
                            if (fit == 'F'){
                                int bitlibe2 = firsFit(DiscoEnUso,inicio);
                                if (bitlibe2 == -1) cout << " no se puede crear9 " <<endl;
                                else bitlibre2 = firsFit(DiscoEnUso,inicio);
                            }else if (fit == 'W'){

                            }else if (fit == 'B'){

                            }
                            OTROBA.i_block[0] = bitlibre2;
                            for(a = 1; a < 15; a++){
                                OTROBA.i_block[a] = -1;
                            }
                            OTROBA.i_type = '0';
                            OTROBA.i_perm = 664;
                            fseek(DiscoEnUso,sb.s_inode_start +(sizeof(inodeTable)*bitLibre),SEEK_SET);
                            fwrite(&OTROBA,sizeof(inodeTable),1,DiscoEnUso);
                            BC2.b_content[0].b_inodo =bitLibre;
                            strcpy(BC2.b_content[0].b_name,".");
                            BC2.b_content[1].b_inodo =LugarInododo;
                            strcpy(BC2.b_content[1].b_name,"..");
                            BC2.b_content[2].b_inodo =-1;
                            strcpy(BC2.b_content[2].b_name,"");
                            BC2.b_content[3].b_inodo =-1;
                            strcpy(BC2.b_content[3].b_name,"");
                            fseek(DiscoEnUso,sb.s_bm_block_start + (bitlibre2*sizeof(char)),SEEK_SET);
                            fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                            fseek(DiscoEnUso,sb.s_block_start +(sizeof(bloqueCarpetas)*bitlibre2),SEEK_SET);
                            fwrite(&BC2,sizeof(bloqueCarpetas),1,DiscoEnUso);
                            fseek(DiscoEnUso,inicio,SEEK_SET);
                            fread(&sb,sizeof(superBloque),1,DiscoEnUso);
                            sb.s_free_block_count = sb.s_free_block_count - 2;
                            sb.s_free_inodes_count = sb.s_free_inodes_count - 1;
                            sb.s_first_blo = sb.s_first_blo +2;
                            sb.s_first_ino = sb.s_first_ino +1;
                            fseek(DiscoEnUso,inicio,SEEK_SET);
                            fwrite(&sb,sizeof(superBloque),1,DiscoEnUso);
                            strcpy(Unionpath,copiaPath2);
                            return 6;
                    }
                }
            }

}

int Mkdir::firsFit(FILE *disco,int inicio){
    superBloque super;
    int retorno = -1;
    bool bandera = false;
    fseek(disco,inicio,SEEK_SET);
    fread(&super,sizeof(superBloque),1,disco);
    char bitTemporal;
    for (int i = 0; i < super.s_blocks_count; i++){
        fseek(disco,super.s_bm_block_start + i,SEEK_SET);
        char byte;
        bitTemporal = fgetc(disco);
        if (bitTemporal == '0'){
            bandera = true;
            retorno = i;
            break;
        }
    }
    if (bandera == true){
        return retorno;
    }else {
        return -1;
    }

}


int Mkdir::firsFitInodo(FILE *disco,int inicio){
    superBloque super;
    int retorno = -1;
    bool bandera = false;
    fseek(disco,inicio,SEEK_SET);
    fread(&super,sizeof(superBloque),1,disco);
    char bitTemporal;
    for (int i = 0; i < super.s_inodes_count; i++){
        fseek(disco,super.s_bm_inode_start + i,SEEK_SET);
        //char byte;
        //fread(&byte,sizeof (char),1,disco);
        bitTemporal = fgetc(disco);
        if (bitTemporal == '0'){
            bandera = true;
            retorno = i;
            break;
        }
    }
    if (bandera == true){
        return retorno;
    }else {
        return -1;
    }

}

