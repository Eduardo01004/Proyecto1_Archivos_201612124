#include "mkfile.h"

mkfile::mkfile()
{

}

int mkfile::CrearArchivo(FILE *DiscoEnUso,char *path,char fit,bool indicadorp,int inicio_particion,int size,QString cont,bool flag_root){
    inodeTable InodoUso,OTROBA,InodoUso2;
    bloqueCarpetas BC,BC2,NueC;
    char Unionpath[500];
    char COPIAUnionpath[500];
    superBloque sb;
    QList<string> lista = QList<string>();
    int contador = 0;
    int contador2 = 0;
    int contador3 = 0;
    int iblock = 0;
    int iblock2 = 0;
    int comprobacionRuta = 0;
    int LugarInododo = 0;
    QString ruta = path;
    //ruta.toStdString().replace("\"","");
    string coco = ruta.toStdString();
    QStringList direccion = ruta.split('/');
    char copiaPath[500];
    char copiaPath2[500];
    char copiaPath3[500];
    char LarutaDelaCarpeta[500];
    char nombreCarpeta[80];
    strcpy(copiaPath,path);
    strcpy(copiaPath2,path);
    strcpy(copiaPath3,path);
    strcpy(LarutaDelaCarpeta,dirname(copiaPath));
    strcpy(copiaPath,path);
    strcpy(nombreCarpeta,basename(copiaPath));
    char *token = strtok(path,"/");
    char bitcero='0';
    char bituno='1';
    int final = size;
    int Indicadorcont = 0;
    string contenido = "";
    string contentSize = "0123456789";

    while(token != nullptr){
        contador = contador +1;
        lista.append(token);
        token = strtok(nullptr,"/");

    }
    //cout << contador <<endl;

    for (int i = 0; i < direccion.size(); i++){
        contador3 = i;
       // cout << direccion.at(i).toStdString() << endl;
    }
    //cout << contador3 << endl;

    //char *llenador = nullptr;
   /*if(cont != ""){
        FILE *ArchivoEnUso = fopen(cont.toStdString().c_str(), "r");
        if(ArchivoEnUso == nullptr){
            return 7;
        }
        size = 0;
        fseek(ArchivoEnUso, 0, SEEK_END);
        size = ftell(ArchivoEnUso);
        rewind(ArchivoEnUso);
        llenador = (char*)malloc((size + 1) * sizeof(*llenador));
        fread(llenador, size, 1, ArchivoEnUso);
        llenador[size] = '\0';
        printf("%s\n", llenador);
        Indicadorcont = 1;
    }*/
    char *llenador = NULL;
    if(cont.length() != 0){
        FILE *ArchivoEnUso = fopen(cont.toStdString().c_str(), "r");
        if(ArchivoEnUso == NULL){
            return 7;
        }
        fseek(ArchivoEnUso, 0, SEEK_END);
        size = ftell(ArchivoEnUso);
        fseek(ArchivoEnUso, 0, SEEK_END);
        for (int i = 0; i < final; i++){
            contenido += static_cast<char>(fgetc(ArchivoEnUso));
        }
    }
   fseek(DiscoEnUso,inicio_particion,SEEK_SET);
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
                   char auxbituno = '2';
                   int bitLibre = 0;
                   int bitlibre2 = 0;
                   int a =0;
                   //bitLibre = grp.buscarBloque(DiscoEnUso,inicio_particion,1,fit);
                   if (fit == 'F'){
                       int bitlibe2 = dirm.firsFitInodo(DiscoEnUso,inicio_particion);
                       if (bitlibe2 == -1) cout << " no se puede crear1 " <<endl;
                       else bitLibre = dirm.firsFitInodo(DiscoEnUso,inicio_particion);
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
                   for(a = 0 ;a < 15; a++){
                       OTROBA.i_block[a] = -1;
                   }
                   OTROBA.i_type = '1';
                   OTROBA.i_perm = 664;

                   fseek(DiscoEnUso,sb.s_inode_start +(sizeof(inodeTable)*bitLibre),SEEK_SET);
                   fwrite(&OTROBA,sizeof(inodeTable),1,DiscoEnUso);
                   fseek(DiscoEnUso,sb.s_bm_inode_start + (bitLibre*sizeof(char)),SEEK_SET);
                   fwrite(&bituno,sizeof(char),1,DiscoEnUso);
                   int verArchivo = dirm.BuscarCoA(DiscoEnUso,copiaPath3,inicio_particion,&LugarInododo);
                   if(verArchivo == 1){
                       memset(Unionpath,0,sizeof(Unionpath));
                       fseek(DiscoEnUso,sb.s_inode_start+(sizeof(inodeTable)*LugarInododo),SEEK_SET);
                       fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                       double aux = 0;
                       int cantidadBloques = 1;
                       int index = 0;
                       int conIndex = 0;
                       int charNum = 0;
                       int caracteres = size;
                       if(size != 0){
                           int x = 0;
                           aux = (double)size/63;
                           cantidadBloques = (int)ceil(aux);
                           bloqueArchivo nBA;

                           for(x = 0; x < cantidadBloques; x++){
                               char auxbit = '2';

                               //int bloqueActual = grp.buscarBloque(DiscoEnUso,inicio_particion,2,fit);
                               int bloqueActual = 0;
                               if (fit == 'F'){
                                   int bitlibe2 = dirm.firsFit(DiscoEnUso,inicio_particion);
                                   if (bitlibe2 == -1) cout << " no se puede crear2 " <<endl;
                                   else bloqueActual = dirm.firsFit(DiscoEnUso,inicio_particion);
                               }else if (fit == 'W'){

                               }else if (fit == 'B'){

                               }

                               fseek(DiscoEnUso,sb.s_bm_block_start+(sizeof(char)*bloqueActual),SEEK_SET);
                               fwrite(&auxbit,sizeof(char),1,DiscoEnUso);
                               memset(nBA.b_content,0,sizeof(nBA.b_content));

                               if(caracteres > 63){
                                   for(index = 0; index < 63; index++){
                                       if(contenido.length() == 0){
                                           nBA.b_content[index] = contenido[charNum];
                                           charNum++;
                                           if(charNum == 10){
                                               charNum = 0;
                                           }
                                       }else {
                                           nBA.b_content[index] = contentSize[conIndex];
                                           conIndex ++;
                                       }
                                   }

                                   fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                   fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                   InodoUso2.i_block[x] = bloqueActual;
                                   InodoUso2.i_size = size;
                                   fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                   fwrite(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                   fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueArchivo)*bloqueActual),SEEK_SET);
                                   fwrite(&nBA,sizeof(bloqueArchivo),1,DiscoEnUso);
                                   caracteres = caracteres - 63;
                               }else{

                                   for(index = 0; index < caracteres; index++){
                                       if(contenido.length() == 0){
                                           nBA.b_content[index] = contenido[charNum];
                                           charNum++;
                                           if(charNum == 10){
                                               charNum = 0;
                                           }
                                       }else {
                                           nBA.b_content[index] = contentSize[conIndex];
                                           conIndex ++;
                                       }
                                   }
                                   fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                   fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                   InodoUso2.i_block[x] = bloqueActual;
                                   InodoUso2.i_size = size;
                                   fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                   fwrite(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                   fseek(DiscoEnUso,sb.s_block_start+(bloqueActual * sizeof(bloqueArchivo)),SEEK_SET);
                                   fwrite(&nBA,sizeof(bloqueArchivo),1,DiscoEnUso);

                               }

                           }

                           fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                           fread(&sb,sizeof(superBloque),1,DiscoEnUso);
                           sb.s_free_block_count = sb.s_free_block_count - cantidadBloques;
                           sb.s_free_inodes_count = sb.s_free_inodes_count - 1;
                           sb.s_first_ino = sb.s_first_ino +1;
                           sb.s_first_blo = sb.s_first_blo +cantidadBloques;
                           fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                           fwrite(&sb,sizeof(superBloque),1,DiscoEnUso);

                       }
                   }
                   strcpy(Unionpath,copiaPath2);
                   return 6;
           }else if (libre == 0){
               char auxbituno = '2';
               int bitLibre = 0;
               int bitlibre2 = 0;
               int segui = 0;
               int asuper = 0;
               //bitlibre2 = grp.buscarBloque(DiscoEnUso,inicio_particion,2,fit);
               if (fit == 'F'){
                   int bitlibe2 = dirm.firsFit(DiscoEnUso,inicio_particion);
                   if (bitlibe2 == -1) cout << " no se puede crear2 " <<endl;
                   else bitlibre2 = dirm.firsFit(DiscoEnUso,inicio_particion);
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
                   fseek(DiscoEnUso,sb.s_inode_start,SEEK_SET);
                   fwrite(&InodoUso,sizeof(inodeTable),1,DiscoEnUso);
                   fseek(DiscoEnUso,sb.s_bm_block_start + (bitlibre2*sizeof(char)),SEEK_SET);
                   fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                   if (fit == 'F'){
                       int bitlibe2 = dirm.firsFitInodo(DiscoEnUso,inicio_particion);
                       if (bitlibe2 == -1) cout << " no se puede crear1 " <<endl;
                       else bitLibre = dirm.firsFitInodo(DiscoEnUso,inicio_particion);
                   }else if (fit == 'W'){

                   }else if (fit == 'B'){

                    }
                   //bitLibre = grp.buscarBloque(DiscoEnUso,inicio_particion,1,fit);
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
                   fwrite(&bituno,sizeof(char),1,DiscoEnUso);
                   OTROBA.i_uid = 0;
                   OTROBA.i_gid = 0;
                   OTROBA.i_size= 0;
                   time_t t = time(nullptr);
                   tm *now = localtime(&t);
                   string dateC = to_string(now->tm_mday) + "/" + to_string((now->tm_mon+1)) + "/" + to_string((now->tm_year + 1900)) + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min);
                   strcpy(OTROBA.i_atime, dateC.c_str());
                   strcpy(OTROBA.i_ctime, dateC.c_str());
                   strcpy(OTROBA.i_mtime, dateC.c_str());
                   for(int a = 0; a < 15; a++){
                       OTROBA.i_block[a] = -1;
                   }
                   fseek(DiscoEnUso,sb.s_inode_start +(sizeof(inodeTable)*bitLibre),SEEK_SET);
                   fwrite(&OTROBA,sizeof(inodeTable),1,DiscoEnUso);
                   fseek(DiscoEnUso,sb.s_bm_inode_start + (bitLibre*sizeof(char)),SEEK_SET);
                   fwrite(&bituno,sizeof(char),1,DiscoEnUso);
                   int verArchivo = dirm.BuscarCoA(DiscoEnUso,copiaPath3,inicio_particion,&LugarInododo);
                   if(verArchivo == 1){
                       memset(Unionpath,0,sizeof(Unionpath));
                       fseek(DiscoEnUso,sb.s_inode_start+(sizeof(inodeTable)*LugarInododo),SEEK_SET);
                       fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                       double aux = 0;
                       int cantidadBloques = 1;
                       int index = 0;
                       int conIndex = 0;
                       int charNum = 0;
                       int caracteres = size;
                       if(size != 0){
                           int x = 0;
                           aux = (double)size/63;
                           cantidadBloques = (int)ceil(aux);
                           bloqueArchivo nBA;

                           for(x = 0; x < cantidadBloques; x++){
                               char auxbit = '2';
                               //int bloqueActual = grp.buscarBloque(DiscoEnUso,inicio_particion,2,fit);
                               int bloqueActual = 0;
                               if (fit == 'F'){
                                   int bitlibe2 = dirm.firsFit(DiscoEnUso,inicio_particion);
                                   if (bitlibe2 == -1) cout << " no se puede crear2 " <<endl;
                                   else bloqueActual = dirm.firsFit(DiscoEnUso,inicio_particion);
                               }else if (fit == 'W'){

                               }else if (fit == 'B'){

                               }
                               fseek(DiscoEnUso,sb.s_bm_block_start+(sizeof(char)*bloqueActual),SEEK_SET);
                               fwrite(&auxbit,sizeof(char),1,DiscoEnUso);
                               memset(nBA.b_content,0,sizeof(nBA.b_content));
                               if(caracteres > 63){
                                   for(index = 0; index < 63; index++){
                                       if(contenido.length() == 0){
                                           nBA.b_content[index] = contenido[charNum];
                                           charNum++;
                                           if(charNum == 10){
                                               charNum = 0;
                                           }
                                       }else {
                                           nBA.b_content[index] = contentSize[conIndex];
                                           conIndex ++;
                                       }
                                   }

                                   fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                   fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                   InodoUso2.i_block[x] = bloqueActual;
                                   InodoUso2.i_size = size;
                                   fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                   fwrite(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                   fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueArchivo)*bloqueActual),SEEK_SET);
                                   fwrite(&nBA,sizeof(bloqueArchivo),1,DiscoEnUso);
                                   caracteres = caracteres - 63;
                               }else{

                                   for(index = 0; index < caracteres; index++){
                                       if(contenido.length() == 0){
                                           nBA.b_content[index] = contenido[charNum];
                                           charNum++;
                                           if(charNum == 10){
                                               charNum = 0;
                                           }
                                       }else {
                                           nBA.b_content[index] = contentSize[conIndex];
                                           conIndex ++;
                                       }
                                   }
                                   fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                   fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                   InodoUso2.i_block[x] = bloqueActual;
                                   InodoUso2.i_size = size;
                                   fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                   fwrite(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                   fseek(DiscoEnUso,sb.s_block_start+(bloqueActual * sizeof(bloqueArchivo)),SEEK_SET);
                                   fwrite(&nBA,sizeof(bloqueArchivo),1,DiscoEnUso);

                               }

                           }
                       }

                   fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                   fread(&sb,sizeof(superBloque),1,DiscoEnUso);
                   sb.s_free_block_count = sb.s_free_block_count - cantidadBloques;
                   sb.s_free_inodes_count = sb.s_free_inodes_count - 1;
                   sb.s_first_blo = sb.s_first_blo +cantidadBloques;
                   sb.s_first_ino = sb.s_first_ino +1;
                   fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                   fwrite(&sb,sizeof(superBloque),1,DiscoEnUso);
                   }
           }
               strcpy(Unionpath,copiaPath2);
               return 6;

       }else if (contador >1){
           int SaberExiste = dirm.BuscarCoA(DiscoEnUso,LarutaDelaCarpeta,inicio_particion,&LugarInododo);
           if(SaberExiste == 0){
               if(indicadorp == 0){
                   cout << "entra aqui" <<endl;
                   return 4;
               }else{
                   memset(Unionpath,0,sizeof(Unionpath));
                   for(contador2 = 0;contador2<contador-1;contador2++){
                       strcat(Unionpath, "\/");
                       strcat(Unionpath,lista.at(contador2).c_str());
                       strcpy(COPIAUnionpath,Unionpath);
                       int lovio = dirm.BuscarCoA(DiscoEnUso,Unionpath,inicio_particion,&LugarInododo);
                       if(lovio == 0){
                           dirm.CrearCarpeta(DiscoEnUso,fit,0,inicio_particion,COPIAUnionpath,indicadorp,copiaPath);
                       }
                   }
               }
           }
           else if(SaberExiste == 1){
               int mover = 0;
               int mover2 = 0;
               int libre = 0;
               fseek(DiscoEnUso,sb.s_inode_start+ (sizeof(inodeTable)*LugarInododo),SEEK_SET);
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
                           for(iblock2 = 0; iblock2 < 4; iblock2++){
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
                       char auxbituno = '2';
                       int bitLibre = 0;
                       int bitlibre2 = 0;
                       int a =0;
                       //bitLibre = grp.buscarBloque(DiscoEnUso,inicio_particion,1,fit);
                       if (fit == 'F'){
                           int bitlibe2 = dirm.firsFitInodo(DiscoEnUso,inicio_particion);
                           if (bitlibe2 == -1) cout << " no se puede crear1 " <<endl;
                           else bitLibre = dirm.firsFitInodo(DiscoEnUso,inicio_particion);
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
                       for(a = 0;a<15;a++){
                           OTROBA.i_block[a] = -1;
                       }
                       OTROBA.i_type = '1';
                       OTROBA.i_perm = 664;
                       fseek(DiscoEnUso,sb.s_inode_start +(sizeof(inodeTable)*bitLibre),SEEK_SET);
                       fwrite(&OTROBA,sizeof(inodeTable),1,DiscoEnUso);
                       fseek(DiscoEnUso,sb.s_bm_inode_start + (bitLibre*sizeof(char)),SEEK_SET);
                       fwrite(&bituno,sizeof(char),1,DiscoEnUso);
                       int verArchivo = dirm.BuscarCoA(DiscoEnUso,copiaPath3,inicio_particion,&LugarInododo);
                       if(verArchivo == 1){
                           memset(Unionpath,0,sizeof(Unionpath));
                           fseek(DiscoEnUso,sb.s_inode_start+(sizeof(inodeTable)*LugarInododo),SEEK_SET);
                           fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                           double aux = 0;
                           int cantidadBloques = 1;
                           int index = 0;
                           int conIndex = 0;
                           int charNum = 0;
                           int caracteres = size;
                           if(size != 0){
                               int x = 0;
                               aux = (double)size/63;
                               cantidadBloques = (int)ceil(aux);
                               bloqueArchivo nBA;

                               for(x = 0; x < cantidadBloques; x++){
                                   char auxbit = '2';
                                   //int bloqueActual = grp.buscarBloque(DiscoEnUso,inicio_particion,2,fit);
                                   int bloqueActual = 0;
                                   if (fit == 'F'){
                                       int bitlibe2 = dirm.firsFit(DiscoEnUso,inicio_particion);
                                       if (bitlibe2 == -1) cout << " no se puede crear2 " <<endl;
                                       else bloqueActual = dirm.firsFit(DiscoEnUso,inicio_particion);
                                   }else if (fit == 'W'){

                                   }else if (fit == 'B'){

                                   }
                                   fseek(DiscoEnUso,sb.s_bm_block_start+(sizeof(char)*bloqueActual),SEEK_SET);
                                   fwrite(&auxbit,sizeof(char),1,DiscoEnUso);
                                   memset(nBA.b_content,0,sizeof(nBA.b_content));
                                   if(caracteres > 63){
                                       for(index = 0; index < 63; index++){
                                           if(contenido.length() == 0){
                                               nBA.b_content[index] = contenido[charNum];
                                               charNum++;
                                               if(charNum == 10){
                                                   charNum = 0;
                                               }
                                           }else {
                                               nBA.b_content[index] = contentSize[conIndex];
                                               conIndex ++;
                                           }
                                       }

                                       fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                       fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                       InodoUso2.i_block[x] = bloqueActual;
                                       InodoUso2.i_size = size;
                                       fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                       fwrite(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                       fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueArchivo)*bloqueActual),SEEK_SET);
                                       fwrite(&nBA,sizeof(bloqueArchivo),1,DiscoEnUso);
                                       caracteres = caracteres - 64;
                                   }else{

                                       for(index = 0; index < caracteres; index++){
                                           if(contenido.length() == 0){
                                               nBA.b_content[index] = contenido[charNum];
                                               charNum++;
                                               if(charNum == 10){
                                                   charNum = 0;
                                               }
                                           }else {
                                               nBA.b_content[index] = contentSize[conIndex];
                                               conIndex ++;
                                           }
                                       }
                                       fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                       fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                       InodoUso2.i_block[x] = bloqueActual;
                                       InodoUso2.i_size = size;
                                       fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                       fwrite(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                       fseek(DiscoEnUso,sb.s_block_start+(bloqueActual * sizeof(bloqueArchivo)),SEEK_SET);
                                       fwrite(&nBA,sizeof(bloqueArchivo),1,DiscoEnUso);

                                   }

                               }

                               fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                               fread(&sb,sizeof(superBloque),1,DiscoEnUso);
                               sb.s_free_block_count = sb.s_free_block_count - cantidadBloques;
                               sb.s_free_inodes_count = sb.s_free_inodes_count - 1;
                               sb.s_first_ino = sb.s_first_ino +1;
                               sb.s_first_blo = sb.s_first_blo +cantidadBloques;
                               fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                               fwrite(&sb,sizeof(superBloque),1,DiscoEnUso);

                           }
                       }
                       strcpy(Unionpath,copiaPath2);
                       return 6;
               }else if (libre == 0){
                   char auxbituno = '2';
                   int bitLibre = 0;
                   int bitlibre2 = 0;
                   int segui = 0;
                   int a = 0;
                   //bitlibre2 = grp.buscarBloque(DiscoEnUso,inicio_particion,2,fit);
                   if (fit == 'F'){
                       int bitlibe2 = dirm.firsFit(DiscoEnUso,inicio_particion);
                       if (bitlibe2 == -1) cout << " no se puede crear2 " <<endl;
                       else bitlibre2 = dirm.firsFit(DiscoEnUso,inicio_particion);
                   }else if (fit == 'W'){

                   }else if (fit == 'B'){

                   }
                   fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                   fread(&InodoUso,sizeof(inodeTable),1,DiscoEnUso);
                       for(segui = 0; segui < 12; segui++){
                           if(InodoUso.i_block[segui] == -1){
                               iblock2 = segui;
                               break;
                           }
                       }
                       InodoUso.i_block[iblock2]=bitlibre2;
                       fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                       fwrite(&InodoUso,sizeof(inodeTable),1,DiscoEnUso);
                       fseek(DiscoEnUso,sb.s_bm_block_start + (bitlibre2*sizeof(char)),SEEK_SET);
                       fwrite(&bituno,sizeof(char),1,DiscoEnUso);
                       //bitLibre = grp.buscarBloque(DiscoEnUso,inicio_particion,1,fit);
                       if (fit == 'F'){
                           int bitlibe2 = dirm.firsFitInodo(DiscoEnUso,inicio_particion);
                           if (bitlibe2 == -1) cout << " no se puede crear1 " <<endl;
                           else bitLibre = dirm.firsFitInodo(DiscoEnUso,inicio_particion);
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
                       fwrite(&bituno,sizeof(char),1,DiscoEnUso);
                       OTROBA.i_uid = 0;
                       OTROBA.i_gid = 0;
                       OTROBA.i_size= 0;
                       time_t t = time(nullptr);
                       tm *now = localtime(&t);
                       string dateC = to_string(now->tm_mday) + "/" + to_string((now->tm_mon+1)) + "/" + to_string((now->tm_year + 1900)) + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min);
                       strcpy(OTROBA.i_atime, dateC.c_str());
                       strcpy(OTROBA.i_ctime, dateC.c_str());
                       strcpy(OTROBA.i_mtime, dateC.c_str());
                       for(int a = 0; a < 15; a++){
                           OTROBA.i_block[a] = -1;
                       }
                       OTROBA.i_type = '1';
                       OTROBA.i_perm = 664;
                       fseek(DiscoEnUso,sb.s_inode_start +(sizeof(inodeTable)*bitLibre),SEEK_SET);
                       fwrite(&OTROBA,sizeof(inodeTable),1,DiscoEnUso);
                       fseek(DiscoEnUso,sb.s_bm_inode_start + (bitLibre*sizeof(char)),SEEK_SET);
                       fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                       int verArchivo = dirm.BuscarCoA(DiscoEnUso,copiaPath3,inicio_particion,&LugarInododo);
                       if(verArchivo == 1){
                           memset(Unionpath,0,sizeof(Unionpath));
                           fseek(DiscoEnUso,sb.s_inode_start+(sizeof(inodeTable)*LugarInododo),SEEK_SET);
                           fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                           double aux = 0;
                           int cantidadBloques = 1;
                           int index = 0;
                           int conIndex = 0;
                           int charNum = 0;
                           int caracteres = size;
                           if(size != 0){
                               int x = 0;
                               aux = (double)size/63;
                               cantidadBloques = (int)ceil(aux);
                               bloqueArchivo nBA;

                               for(x = 0; x < cantidadBloques; x++){
                                   char auxbit = '2';
                                   //int bloqueActual = grp.buscarBloque(DiscoEnUso,inicio_particion,2,fit);
                                   int bloqueActual = 0;
                                   if (fit == 'F'){
                                       int bitlibe2 = dirm.firsFit(DiscoEnUso,inicio_particion);
                                       if (bitlibe2 == -1) cout << " no se puede crear2 " <<endl;
                                       else bloqueActual = dirm.firsFit(DiscoEnUso,inicio_particion);
                                   }else if (fit == 'W'){

                                   }else if (fit == 'B'){

                                   }
                                   fseek(DiscoEnUso,sb.s_bm_block_start+(sizeof(char)*bloqueActual),SEEK_SET);
                                   fwrite(&auxbit,sizeof(char),1,DiscoEnUso);
                                   memset(nBA.b_content,0,sizeof(nBA.b_content));
                                   if(caracteres > 63){
                                       for(index = 0; index < 63; index++){
                                           if(contenido.length() == 0){
                                               nBA.b_content[index] = contenido[charNum];
                                               charNum++;
                                               if(charNum == 10){
                                                   charNum = 0;
                                               }
                                           }else {
                                               nBA.b_content[index] = contentSize[conIndex];
                                               conIndex ++;
                                           }
                                       }

                                       fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                       fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                       InodoUso2.i_block[x] = bloqueActual;
                                       InodoUso2.i_size = size;
                                       fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                       fwrite(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                       fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueArchivo)*bloqueActual),SEEK_SET);
                                       fwrite(&nBA,sizeof(bloqueArchivo),1,DiscoEnUso);
                                       caracteres = caracteres - 63;
                                   }else{
                                       for(index = 0; index < caracteres; index++){
                                           if(contenido.length() == 0){
                                               nBA.b_content[index] = contenido[charNum];
                                               charNum++;
                                               if(charNum == 10){
                                                   charNum = 0;
                                               }
                                           }else {
                                               nBA.b_content[index] = contentSize[conIndex];
                                               conIndex ++;
                                           }
                                       }
                                       fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                       fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                       InodoUso2.i_block[x] = bloqueActual;
                                       InodoUso2.i_size = size;
                                       fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                       fwrite(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                       fseek(DiscoEnUso,sb.s_block_start+(bloqueActual * sizeof(bloqueArchivo)),SEEK_SET);
                                       fwrite(&nBA,sizeof(bloqueArchivo),1,DiscoEnUso);

                                   }

                               }
                           }

                       fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                       fread(&sb,sizeof(superBloque),1,DiscoEnUso);
                       sb.s_free_block_count = sb.s_free_block_count - cantidadBloques;
                       sb.s_free_inodes_count = sb.s_free_inodes_count - 1;
                       sb.s_first_blo = sb.s_first_blo +cantidadBloques;
                       sb.s_first_ino = sb.s_first_ino +1;
                       fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                       fwrite(&sb,sizeof(superBloque),1,DiscoEnUso);
                    }
               }
                   strcpy(Unionpath,copiaPath2);
                   return 6;

           }
       }
}



int mkfile::CREARA(FILE *DiscoEnUso,char fit,int indicadorp,int inicio_particion,char*pampa,int size,QString cont){
    inodeTable InodoUso,OTROBA,InodoUso2;
        bloqueCarpetas BC,BC2,NueC;
        superBloque sb;
        char LarutaDelaCarpeta[500];
        char copiaPath[500];
        char copiaPath2[500];
        char copiaPath3[500];
        char *listapath2[500];
        char NombreCarpeta[84];
        char Unionpath[500];
        char COPIAUnionpath[500];
        char bitcero='0';
        char bituno='1';
        memset(LarutaDelaCarpeta,0,sizeof(LarutaDelaCarpeta));
        memset(copiaPath,0,sizeof(copiaPath));
        memset(copiaPath2,0,sizeof(copiaPath2));
        memset(copiaPath3,0,sizeof(copiaPath3));
        memset(listapath2,0,sizeof(listapath2));
        strcpy(copiaPath,pampa);
        strcpy(copiaPath2,pampa);
        strcpy(copiaPath3,pampa);
        strcpy(LarutaDelaCarpeta,dirname(copiaPath));
        strcpy(NombreCarpeta,basename(copiaPath2));
        const char s[2] = "\/";
        char *token = strtok(pampa,"/");;
        /*token= (char*)malloc(sizeof(char));
        token = strtok(pampa, s);*/
        int contador=0;
        int contador2=0;
        int iblock =0;
        int iblock2 =0;
        int Indicadorcont = 0;
        int LugarInododo = 0;
        QList<string> lista = QList<string>();
       /*while(token != nullptr)
        {
            listapath2[contador]=(char*)malloc(200);
            strcpy(listapath2[contador],token);
            //printf("LISTA POSICION %d: %s\n", contador,listapath2[contador]);
            contador=contador+1;
            token = strtok(nullptr, s);
        }*/
        while(token != nullptr){
            contador = contador +1;
            lista.append(token);
            token = strtok(nullptr,"/");

        }
        char *llenador = nullptr;
        if(cont != ""){
            FILE *ArchivoEnUso = fopen(cont.toStdString().c_str(), "r");
            if(ArchivoEnUso == nullptr){
                return 7;
            }
            size = 0;
            fseek(ArchivoEnUso, 0, SEEK_END);
            size = ftell(ArchivoEnUso);
            rewind(ArchivoEnUso);
            llenador = (char*) malloc((size + 1) * sizeof(*llenador));
            fread(llenador, size, 1, ArchivoEnUso);
            llenador[size] = '\0';
            printf("%sfofo\n", llenador);
            Indicadorcont = 1;
        }else cout << "cont esta vacio o no viene " <<endl;

        fseek(DiscoEnUso,inicio_particion,SEEK_SET);
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
                    char auxbituno = '2';
                    int bitLibre = 0;
                    int bitlibre2 = 0;
                    int a =0;
                    if (fit == 'F'){
                        int bitlibe2 = dirm.firsFitInodo(DiscoEnUso,inicio_particion);
                        if (bitlibe2 == -1) cout << " no se puede crear1 " <<endl;
                        else bitLibre = dirm.firsFitInodo(DiscoEnUso,inicio_particion);
                    }else if (fit == 'W'){

                    }else if (fit == 'B'){

                     }
                    BC.b_content[mover].b_inodo = bitLibre;
                    strcpy(BC.b_content[mover].b_name,NombreCarpeta);
                    fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueCarpetas)*InodoUso.i_block[mover2]),SEEK_SET);
                    fwrite(&BC,sizeof(bloqueCarpetas),1,DiscoEnUso);
                    OTROBA.i_uid = 1;
                    OTROBA.i_gid = 1;
                    OTROBA.i_size= 0;
                    time_t t = time(nullptr);
                    tm *now = localtime(&t);
                    string dateC = to_string(now->tm_mday) + "/" + to_string((now->tm_mon+1)) + "/" + to_string((now->tm_year + 1900)) + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min);
                    strcpy(OTROBA.i_atime, dateC.c_str());
                    strcpy(OTROBA.i_ctime, dateC.c_str());
                    strcpy(OTROBA.i_mtime, dateC.c_str());
                    for(a = 0;a<15;a++){
                        OTROBA.i_block[a] = -1;
                    }
                    OTROBA.i_type = '1';
                    OTROBA.i_perm = 664;
                    fseek(DiscoEnUso,sb.s_inode_start +(sizeof(inodeTable)*bitLibre),SEEK_SET);
                    fwrite(&OTROBA,sizeof(inodeTable),1,DiscoEnUso);
                    fseek(DiscoEnUso,sb.s_bm_inode_start + (bitLibre*sizeof(char)),SEEK_SET);
                    fwrite(&bituno,sizeof(char),1,DiscoEnUso);
                    int verArchivo = dirm.BuscarCoA(DiscoEnUso,copiaPath3,inicio_particion,&LugarInododo);
                    if(verArchivo == 1){
                        memset(Unionpath,0,sizeof(Unionpath));
                        fseek(DiscoEnUso,sb.s_inode_start+(sizeof(inodeTable)*LugarInododo),SEEK_SET);
                        fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                        double aux = 0;
                        int cantidadBloques = 1;
                        int index = 0;
                        int conIndex = 0;
                        int charNum = 0;
                        int caracteres = size;
                        if(size != 0){
                            int x = 0;
                            aux = (double)size/64;
                            cantidadBloques = (int)ceil(aux);
                            bloqueArchivo nBA;
                            for(x = 0; x < cantidadBloques; x++){
                                char auxbit = '2';
                                int bloqueActual;
                                if (fit == 'F'){
                                    int bitlibe2 = dirm.firsFit(DiscoEnUso,inicio_particion);
                                    if (bitlibe2 == -1) cout << " no se puede crear2 " <<endl;
                                    else bloqueActual = dirm.firsFit(DiscoEnUso,inicio_particion);
                                }else if (fit == 'W'){

                                }else if (fit == 'B'){

                                }
                                fseek(DiscoEnUso,sb.s_bm_block_start+(sizeof(char)*bloqueActual),SEEK_SET);
                                fwrite(&auxbit,sizeof(char),1,DiscoEnUso);
                                memset(nBA.b_content,0,sizeof(nBA.b_content));

                                if(caracteres > 63){
                                    Caracteres(caracteres,index,Indicadorcont,nBA,charNum,llenador,conIndex);
                                    fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                    fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                    InodoUso2.i_block[x] = bloqueActual;
                                    InodoUso2.i_size = size;
                                    fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                    fwrite(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                    fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueArchivo)*bloqueActual),SEEK_SET);
                                    fwrite(&nBA,sizeof(bloqueArchivo),1,DiscoEnUso);
                                    caracteres = caracteres - 63;
                                }else{
                                Caracteres(caracteres,index,Indicadorcont,nBA,charNum,llenador,conIndex);
                                    fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                    fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                    InodoUso2.i_block[x] = bloqueActual;
                                    InodoUso2.i_size = size;
                                    fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                    fwrite(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                    fseek(DiscoEnUso,sb.s_block_start+(bloqueActual * sizeof(bloqueArchivo)),SEEK_SET);
                                    fwrite(&nBA,sizeof(bloqueArchivo),1,DiscoEnUso);
                                }
                            }
                            fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                            fread(&sb,sizeof(superBloque),1,DiscoEnUso);
                            sb.s_free_block_count = sb.s_free_block_count - cantidadBloques;
                            sb.s_free_inodes_count = sb.s_free_inodes_count - 1;
                            sb.s_first_ino = sb.s_first_ino +1;
                            sb.s_first_blo = sb.s_first_blo +cantidadBloques;
                            fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                            fwrite(&sb,sizeof(superBloque),1,DiscoEnUso);

                        }
                    }
                    strcpy(Unionpath,copiaPath2);
                    return 6;

            }
            else if (libre == 0){
                char auxbituno = '2';
                int bitLibre = 0;
                int bitlibre2 = 0;
                int segui = 0;
                int asuper = 0;
                if (fit == 'F'){
                    int bitlibe2 = dirm.firsFit(DiscoEnUso,inicio_particion);
                    if (bitlibe2 == -1) cout << " no se puede crear2 " <<endl;
                    else bitlibre2 = dirm.firsFit(DiscoEnUso,inicio_particion);
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
                    fseek(DiscoEnUso,sb.s_inode_start,SEEK_SET);
                    fwrite(&InodoUso,sizeof(inodeTable),1,DiscoEnUso);
                    fseek(DiscoEnUso,sb.s_bm_block_start + (bitlibre2*sizeof(char)),SEEK_SET);
                    fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                    if (fit == 'F'){
                        int bitlibe2 = dirm.firsFitInodo(DiscoEnUso,inicio_particion);
                        if (bitlibe2 == -1) cout << " no se puede crear1 " <<endl;
                        else bitLibre = dirm.firsFitInodo(DiscoEnUso,inicio_particion);
                    }else if (fit == 'W'){

                    }else if (fit == 'B'){

                     }
                    NueC.b_content[0].b_inodo =bitLibre;
                    strcpy(NueC.b_content[0].b_name,NombreCarpeta);
                    NueC.b_content[1].b_inodo =-1;
                    strcpy(NueC.b_content[1].b_name,"");
                    NueC.b_content[2].b_inodo =-1;
                    strcpy(NueC.b_content[2].b_name,"");
                    NueC.b_content[3].b_inodo =-1;
                    strcpy(NueC.b_content[3].b_name,"");
                    fseek(DiscoEnUso,sb.s_block_start +(sizeof(bloqueCarpetas)*bitlibre2),SEEK_SET);
                    fwrite(&NueC,sizeof(bloqueCarpetas),1,DiscoEnUso);
                    fseek(DiscoEnUso,sb.s_bm_inode_start + (bitLibre*sizeof(char)),SEEK_SET);
                    fwrite(&bituno,sizeof(char),1,DiscoEnUso);
                    OTROBA.i_uid = 1;
                    OTROBA.i_gid = 1;
                    OTROBA.i_size= 0;
                    time_t t = time(nullptr);
                    tm *now = localtime(&t);
                    string dateC = to_string(now->tm_mday) + "/" + to_string((now->tm_mon+1)) + "/" + to_string((now->tm_year + 1900)) + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min);
                    strcpy(OTROBA.i_atime, dateC.c_str());
                    strcpy(OTROBA.i_ctime, dateC.c_str());
                    strcpy(OTROBA.i_mtime, dateC.c_str());
                    for(int a = 0;a<15;a++){
                        OTROBA.i_block[a] = -1;
                    }
                    fseek(DiscoEnUso,sb.s_inode_start +(sizeof(inodeTable)*bitLibre),SEEK_SET);
                    fwrite(&OTROBA,sizeof(inodeTable),1,DiscoEnUso);
                    fseek(DiscoEnUso,sb.s_bm_inode_start + (bitLibre*sizeof(char)),SEEK_SET);
                    fwrite(&bituno,sizeof(char),1,DiscoEnUso);
                    int verArchivo = dirm.BuscarCoA(DiscoEnUso,copiaPath3,inicio_particion,&LugarInododo);
                    if(verArchivo == 1){
                        memset(Unionpath,0,sizeof(Unionpath));
                        fseek(DiscoEnUso,sb.s_inode_start+(sizeof(inodeTable)*LugarInododo),SEEK_SET);
                        fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                        double aux = 0;
                        int cantidadBloques = 1;
                        int index = 0;
                        int conIndex = 0;
                        int charNum = 0;
                        int caracteres = size;
                        if(size != 0){
                            int x = 0;
                            aux = (double)size/64;
                            cantidadBloques = (int)ceil(aux);
                            bloqueArchivo nBA;
                            for(x = 0; x < cantidadBloques; x++){
                                char auxbit = '2';
                                int bloqueActual;
                                if (fit == 'F'){
                                    int bitlibe2 = dirm.firsFit(DiscoEnUso,inicio_particion);
                                    if (bitlibe2 == -1) cout << " no se puede crear2 " <<endl;
                                    else bloqueActual = dirm.firsFit(DiscoEnUso,inicio_particion);
                                }else if (fit == 'W'){

                                }else if (fit == 'B'){

                                }
                                fseek(DiscoEnUso,sb.s_bm_block_start+(sizeof(char)*bloqueActual),SEEK_SET);
                                fwrite(&auxbit,sizeof(char),1,DiscoEnUso);
                                memset(nBA.b_content,0,sizeof(nBA.b_content));
                                if(caracteres > 63){
                                    Caracteres(caracteres,index,Indicadorcont,nBA,charNum,llenador,conIndex);

                                    fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                    fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                    InodoUso2.i_block[x] = bloqueActual;
                                    InodoUso2.i_size = size;
                                    fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                    fwrite(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                    fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueArchivo)*bloqueActual),SEEK_SET);
                                    fwrite(&nBA,sizeof(bloqueArchivo),1,DiscoEnUso);
                                    caracteres = caracteres - 63;
                                }else{
                                    Caracteres(caracteres,index,Indicadorcont,nBA,charNum,llenador,conIndex);
                                    fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                    fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                    InodoUso2.i_block[x] = bloqueActual;
                                    InodoUso2.i_size = size;
                                    fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                    fwrite(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                    fseek(DiscoEnUso,sb.s_block_start+(bloqueActual * sizeof(bloqueArchivo)),SEEK_SET);
                                    fwrite(&nBA,sizeof(bloqueArchivo),1,DiscoEnUso);

                                }

                            }
                        }

                    fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                    fread(&sb,sizeof(superBloque),1,DiscoEnUso);
                    sb.s_free_block_count = sb.s_free_block_count - cantidadBloques;
                    sb.s_free_inodes_count = sb.s_free_inodes_count - 1;
                    sb.s_first_blo = sb.s_first_blo +cantidadBloques;
                    sb.s_first_ino = sb.s_first_ino +1;
                    fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                    fwrite(&sb,sizeof(superBloque),1,DiscoEnUso);
                    }

            }
                strcpy(Unionpath,copiaPath2);
                return 6;
        }
        else if (contador >1){
            int SaberExiste = dirm.BuscarCoA(DiscoEnUso,LarutaDelaCarpeta,inicio_particion,&LugarInododo);
            if(SaberExiste == 0){
                if(indicadorp == 0){
                    return 4;
                }else{
                    memset(Unionpath,0,sizeof(Unionpath));
                    for(contador2 = 0;contador2<contador-1;contador2++){
                        strcat(Unionpath, "\/");
                        //strcat(Unionpath,listapath2[contador2]);
                        strcat(Unionpath,lista.at(contador2).c_str());
                        strcpy(COPIAUnionpath,Unionpath);
                        int lovio = dirm.BuscarCoA(DiscoEnUso,Unionpath,inicio_particion,&LugarInododo);
                        if(lovio == 0){
                            dirm.CrearCarpeta(DiscoEnUso,fit,0,inicio_particion,COPIAUnionpath,0,copiaPath);
                        }
                    }
                }
            }else if(SaberExiste == 1){
                int mover = 0;
                int mover2 = 0;
                int libre = 0;
                fseek(DiscoEnUso,sb.s_inode_start+ (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                fread(&InodoUso,sizeof(inodeTable),1,DiscoEnUso);
                for(iblock=0;iblock<12;iblock++){
                    if(InodoUso.i_block[iblock] != -1){
                        fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueCarpetas)*InodoUso.i_block[iblock]),SEEK_SET);
                        fread(&BC,sizeof(bloqueCarpetas),1,DiscoEnUso);
                        if(iblock==0){
                            for(iblock2=2;iblock2<4;iblock2++){
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
                        char auxbituno = '2';
                        int bitLibre = 0;
                        int bitlibre2 = 0;
                        int a =0;
                        if (fit == 'F'){
                            int bitlibe2 = dirm.firsFitInodo(DiscoEnUso,inicio_particion);
                            if (bitlibe2 == -1) cout << " no se puede crear1 " <<endl;
                            else bitLibre = dirm.firsFitInodo(DiscoEnUso,inicio_particion);
                        }else if (fit == 'W'){

                        }else if (fit == 'B'){

                         }
                        BC.b_content[mover].b_inodo = bitLibre;
                        strcpy(BC.b_content[mover].b_name,NombreCarpeta);
                        fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueCarpetas)*InodoUso.i_block[mover2]),SEEK_SET);
                        fwrite(&BC,sizeof(bloqueCarpetas),1,DiscoEnUso);
                        OTROBA.i_uid = 1;
                        OTROBA.i_gid = 1;
                        OTROBA.i_size= 0;
                        time_t t = time(nullptr);
                        tm *now = localtime(&t);
                        string dateC = to_string(now->tm_mday) + "/" + to_string((now->tm_mon+1)) + "/" + to_string((now->tm_year + 1900)) + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min);
                        strcpy(OTROBA.i_atime, dateC.c_str());
                        strcpy(OTROBA.i_ctime, dateC.c_str());
                        strcpy(OTROBA.i_mtime, dateC.c_str());
                        for(a = 0;a<15;a++){
                            OTROBA.i_block[a] = -1;
                        }
                        OTROBA.i_type = '1';
                        OTROBA.i_perm = 664;
                        fseek(DiscoEnUso,sb.s_inode_start +(sizeof(inodeTable)*bitLibre),SEEK_SET);
                        fwrite(&OTROBA,sizeof(inodeTable),1,DiscoEnUso);
                        fseek(DiscoEnUso,sb.s_bm_inode_start + (bitLibre*sizeof(char)),SEEK_SET);
                        fwrite(&bituno,sizeof(char),1,DiscoEnUso);
                        int verArchivo = dirm.BuscarCoA(DiscoEnUso,copiaPath3,inicio_particion,&LugarInododo);
                        if(verArchivo == 1){
                            memset(Unionpath,0,sizeof(Unionpath));
                            fseek(DiscoEnUso,sb.s_inode_start+(sizeof(inodeTable)*LugarInododo),SEEK_SET);
                            fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                            double aux = 0;
                            int cantidadBloques = 1;
                            int index = 0;
                            int conIndex = 0;
                            int charNum = 0;
                            int caracteres = size;
                            if(size != 0){
                                int x = 0;
                                aux = (double)size/64;
                                cantidadBloques = (int)ceil(aux);
                                bloqueArchivo nBA;

                                for(x = 0; x < cantidadBloques; x++){
                                    char auxbit = '2';
                                    int bloqueActual;
                                    if (fit == 'F'){
                                        int bitlibe2 = dirm.firsFit(DiscoEnUso,inicio_particion);
                                        if (bitlibe2 == -1) cout << " no se puede crear2 " <<endl;
                                        else bloqueActual = dirm.firsFit(DiscoEnUso,inicio_particion);
                                    }else if (fit == 'W'){

                                    }else if (fit == 'B'){

                                    }
                                    fseek(DiscoEnUso,sb.s_bm_block_start+(sizeof(char)*bloqueActual),SEEK_SET);
                                    fwrite(&auxbit,sizeof(char),1,DiscoEnUso);
                                    memset(nBA.b_content,0,sizeof(nBA.b_content));
                                    if(caracteres > 63){
                                        Caracteres(caracteres,index,Indicadorcont,nBA,charNum,llenador,conIndex);
                                        fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                        fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                        InodoUso2.i_block[x] = bloqueActual;
                                        InodoUso2.i_size = size;
                                        fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                        fwrite(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                        fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueArchivo)*bloqueActual),SEEK_SET);
                                        fwrite(&nBA,sizeof(bloqueArchivo),1,DiscoEnUso);
                                        caracteres = caracteres - 63;
                                    }else{
                                        Caracteres(caracteres,index,Indicadorcont,nBA,charNum,llenador,conIndex);
                                        fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                        fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                        InodoUso2.i_block[x] = bloqueActual;
                                        InodoUso2.i_size = size;
                                        fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                        fwrite(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                        fseek(DiscoEnUso,sb.s_block_start+(bloqueActual * sizeof(bloqueArchivo)),SEEK_SET);
                                        fwrite(&nBA,sizeof(bloqueArchivo),1,DiscoEnUso);

                                    }

                                }

                                fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                                fread(&sb,sizeof(superBloque),1,DiscoEnUso);
                                sb.s_free_block_count = sb.s_free_block_count - cantidadBloques;
                                sb.s_free_inodes_count = sb.s_free_inodes_count - 1;
                                sb.s_first_ino = sb.s_first_ino +1;
                                sb.s_first_blo = sb.s_first_blo +cantidadBloques;
                                fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                                fwrite(&sb,sizeof(superBloque),1,DiscoEnUso);

                            }
                        }
                        strcpy(Unionpath,copiaPath2);
                        return 6;
                }else if (libre == 0){
                    char auxbituno = '2';
                    int bitLibre = 0;
                    int bitlibre2 = 0;
                    int segui = 0;
                    int a = 0;
                    if (fit == 'F'){
                        int bitlibe2 = dirm.firsFit(DiscoEnUso,inicio_particion);
                        if (bitlibe2 == -1) cout << " no se puede crear2 " <<endl;
                        else bitlibre2 = dirm.firsFit(DiscoEnUso,inicio_particion);
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
                        fwrite(&bituno,sizeof(char),1,DiscoEnUso);
                        if (fit == 'F'){
                            int bitlibe2 = dirm.firsFitInodo(DiscoEnUso,inicio_particion);
                            if (bitlibe2 == -1) cout << " no se puede crear1 " <<endl;
                            else bitLibre = dirm.firsFitInodo(DiscoEnUso,inicio_particion);
                        }else if (fit == 'W'){

                        }else if (fit == 'B'){

                         }
                        NueC.b_content[0].b_inodo =bitLibre;
                        strcpy(NueC.b_content[0].b_name,NombreCarpeta);
                        NueC.b_content[1].b_inodo =-1;
                        strcpy(NueC.b_content[1].b_name,"");
                        NueC.b_content[2].b_inodo =-1;
                        strcpy(NueC.b_content[2].b_name,"");
                        NueC.b_content[3].b_inodo =-1;
                        strcpy(NueC.b_content[3].b_name,"");
                        fseek(DiscoEnUso,sb.s_block_start +(sizeof(bloqueCarpetas)*bitlibre2),SEEK_SET);
                        fwrite(&NueC,sizeof(bloqueCarpetas),1,DiscoEnUso);
                        fseek(DiscoEnUso,sb.s_bm_inode_start + (bitLibre*sizeof(char)),SEEK_SET);
                        fwrite(&bituno,sizeof(char),1,DiscoEnUso);
                        OTROBA.i_uid = 1;
                        OTROBA.i_gid = 1;
                        OTROBA.i_size= 0;
                        time_t t = time(nullptr);
                        tm *now = localtime(&t);
                        string dateC = to_string(now->tm_mday) + "/" + to_string((now->tm_mon+1)) + "/" + to_string((now->tm_year + 1900)) + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min);
                        strcpy(OTROBA.i_atime, dateC.c_str());
                        strcpy(OTROBA.i_ctime, dateC.c_str());
                        strcpy(OTROBA.i_mtime, dateC.c_str());
                        for(a = 0;a<15;a++){
                            OTROBA.i_block[a] = -1;
                        }
                        OTROBA.i_type = '1';
                        OTROBA.i_perm = 664;
                        fseek(DiscoEnUso,sb.s_inode_start +(sizeof(inodeTable)*bitLibre),SEEK_SET);
                        fwrite(&OTROBA,sizeof(inodeTable),1,DiscoEnUso);
                        fseek(DiscoEnUso,sb.s_bm_inode_start + (bitLibre*sizeof(char)),SEEK_SET);
                        fwrite(&auxbituno,sizeof(char),1,DiscoEnUso);
                        int verArchivo = dirm.BuscarCoA(DiscoEnUso,copiaPath3,inicio_particion,&LugarInododo);
                        if(verArchivo == 1){
                            memset(Unionpath,0,sizeof(Unionpath));
                            fseek(DiscoEnUso,sb.s_inode_start+(sizeof(inodeTable)*LugarInododo),SEEK_SET);
                            fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                            double aux = 0;
                            int cantidadBloques = 1;
                            int index = 0;
                            int conIndex = 0;
                            int charNum = 0;
                            int caracteres = size;
                            if(size != 0){
                                int x = 0;
                                aux = (double)size/64;
                                cantidadBloques = (int)ceil(aux);
                                bloqueArchivo nBA;

                                for(x = 0; x < cantidadBloques; x++){
                                    char auxbit = '2';
                                    int bloqueActual;
                                    if (fit == 'F'){
                                        int bitlibe2 = dirm.firsFit(DiscoEnUso,inicio_particion);
                                        if (bitlibe2 == -1) cout << " no se puede crear2 " <<endl;
                                        else bloqueActual = dirm.firsFit(DiscoEnUso,inicio_particion);
                                    }else if (fit == 'W'){

                                    }else if (fit == 'B'){

                                    }
                                    fseek(DiscoEnUso,sb.s_bm_block_start+(sizeof(char)*bloqueActual),SEEK_SET);
                                    fwrite(&auxbit,sizeof(char),1,DiscoEnUso);
                                    memset(nBA.b_content,0,sizeof(nBA.b_content));
                                    if(caracteres > 63){
                                        Caracteres(caracteres,index,Indicadorcont,nBA,charNum,llenador,conIndex);
                                        fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                        fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                        InodoUso2.i_block[x] = bloqueActual;
                                        InodoUso2.i_size = size;
                                        fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                        fwrite(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                        fseek(DiscoEnUso,sb.s_block_start+(sizeof(bloqueArchivo)*bloqueActual),SEEK_SET);
                                        fwrite(&nBA,sizeof(bloqueArchivo),1,DiscoEnUso);
                                        caracteres = caracteres - 63;
                                    }else{
                                        Caracteres(caracteres,index,Indicadorcont,nBA,charNum,llenador,conIndex);
                                        fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                        fread(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                        InodoUso2.i_block[x] = bloqueActual;
                                        InodoUso2.i_size = size;
                                        fseek(DiscoEnUso,sb.s_inode_start + (sizeof(inodeTable)*LugarInododo),SEEK_SET);
                                        fwrite(&InodoUso2,sizeof(inodeTable),1,DiscoEnUso);
                                        fseek(DiscoEnUso,sb.s_block_start+(bloqueActual * sizeof(bloqueArchivo)),SEEK_SET);
                                        fwrite(&nBA,sizeof(bloqueArchivo),1,DiscoEnUso);
                                    }

                                }
                            }

                        fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                        fread(&sb,sizeof(superBloque),1,DiscoEnUso);
                        sb.s_free_block_count = sb.s_free_block_count - cantidadBloques;
                        sb.s_free_inodes_count = sb.s_free_inodes_count - 1;
                        sb.s_first_blo = sb.s_first_blo +cantidadBloques;
                        sb.s_first_ino = sb.s_first_ino +1;
                        fseek(DiscoEnUso,inicio_particion,SEEK_SET);
                        fwrite(&sb,sizeof(superBloque),1,DiscoEnUso);
                        }
                }
                    strcpy(Unionpath,copiaPath2);
                    return 6;

            }
        }
}
void mkfile::Caracteres(int caracteres, int index, int Indicadorcont,bloqueArchivo nBA,int charNum,char*llenador,int conIndex){
        for(index = 0; index < 63; index++){
            if(Indicadorcont == 0){
                nBA.b_content[index] = charNum + '0';
                charNum++;
                if(charNum == 10){
                    charNum = 0;
                }
            }else if(Indicadorcont == 1){
                nBA.b_content[index] = llenador[conIndex];
                conIndex ++;
            }
        }

}
