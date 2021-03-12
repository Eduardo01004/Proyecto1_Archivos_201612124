#include "mkgrp.h"

Mkgrp::Mkgrp()
{

}

void Mkgrp::BuscarGrupo(QString name,int iniciosuper){


}

int  Mkgrp::llenar1(FILE *disco,superBloque super,bloqueArchivo archivo,inodeTable inodouser,int posicion, char cadena[400],int tam_actual){
    int res = 1;
    strcat(archivo.b_content,cadena);
    fseek(disco,super.s_block_start+ (static_cast<int>(sizeof(bloqueArchivo)*posicion)),SEEK_SET);
    fwrite(&archivo,sizeof(bloqueArchivo),1,disco);
    fseek(disco,super.s_inode_start + static_cast<int>(sizeof(inodeTable)),SEEK_SET);
    fread(&inodouser,sizeof(inodeTable),1,disco);
    inodouser.i_size = inodouser.i_size + tam_actual;
    time_t t = time(nullptr);
    tm *now = localtime(&t);
    string dateC = to_string(now->tm_mday) + "/" + to_string((now->tm_mon+1)) + "/" + to_string((now->tm_year + 1900)) + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min);
    strcpy(inodouser.i_atime, dateC.c_str());
    fseek(disco,super.s_inode_start + static_cast<int>(sizeof(inodeTable)),SEEK_SET);
    fwrite(&inodouser,sizeof(inodeTable),1,disco);
    return res;
}

int Mkgrp::llenar2(FILE *disco,superBloque super,bloqueArchivo archivo,inodeTable inodouser,int posicion, char fase2[400],int tam_actual,int iniciosuper,char fit){
    int superblock = super.s_block_start;
    int superinodo = super.s_inode_start;
    int bitlibe = 0;
   fseek(disco,super.s_block_start + (sizeof(bloqueArchivo) * posicion),SEEK_SET);
   fwrite(&archivo,sizeof(bloqueArchivo),1,disco);
   bloqueArchivo archAux;
   strcpy(archAux.b_content,fase2);
   /*if (fit == 'F'){
       int bitlibe2 = firsFit(disco,iniciosuper);
       if (bitlibe2 == -1) cout << " no se puede crear " <<endl;
       else bitlibe = firsFit(disco,iniciosuper);

   }else if (fit == 'W'){

   }else if (fit == 'B'){

   }*/
   cout << "inicio " << iniciosuper <<endl;
   //bitlibe = buscarBloque(disco,iniciosuper,'2','F');
   cout << "bitlibe " << bitlibe << endl;
   fseek(disco,super.s_block_start+ (sizeof(bloqueArchivo)*bitlibe),SEEK_SET);
   fwrite(&archAux,sizeof(bloqueArchivo),1,disco);

   fseek(disco,super.s_bm_block_start + bitlibe,SEEK_SET);
   fputc('2',disco);

   fseek(disco,super.s_inode_start + sizeof(inodeTable),SEEK_SET);
   fread(&inodouser,sizeof(inodeTable),1,disco);

   inodouser.i_size = inodouser.i_size + tam_actual;
   time_t t = time(nullptr);
   tm *now = localtime(&t);
   string dateC = to_string(now->tm_mday) + "/" + to_string((now->tm_mon+1)) + "/" + to_string((now->tm_year + 1900)) + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min);
   strcpy(inodouser.i_mtime, dateC.c_str());
   inodouser.i_block[posicion] = bitlibe;

   fseek(disco,super.s_inode_start+ sizeof(inodeTable),SEEK_SET);
   fwrite(&inodouser,sizeof(inodeTable),1,disco);

   int superfi = super.s_first_blo;
   int superfree = super.s_free_block_count;

   super.s_first_blo = super.s_first_blo + 1;
   super.s_free_block_count = super.s_free_block_count- 1;
   fseek(disco,iniciosuper,SEEK_SET);
   fwrite(&super,sizeof(superBloque),1,disco);
   return 1;
}

int Mkgrp::firsFit(FILE *disco,int inicio){
    superBloque super;
    int retorno = -1;
    bool bandera = false;
    fseek(disco,inicio,SEEK_SET);
    fread(&super,sizeof(superBloque),1,disco);
    char bitTemporal;
    for (int i = 0; i < super.s_blocks_count; i++){
        fseek(disco,super.s_bm_block_start + i,SEEK_SET);
        char byte;
        bitTemporal = static_cast<int>(fgetc(disco));
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

int Mkgrp::firsFitInodo(FILE *disco,int inicio){
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
        bitTemporal = static_cast<int>(fgetc(disco));
        if (bitTemporal == '0'){
            bandera = true;
            retorno = i;
        }
    }
    if (bandera == true){
        return retorno;
    }else {
        return -1;
    }

}
