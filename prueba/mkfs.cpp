#include "mkfs.h"

Mkfs::Mkfs()
{

}


void Mkfs::AutomataMkfs(QString lexema,QString token,int flag){
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
            if (flag_idmkfs){
                if (flag_tipomkfs == 0){
                    tipomkfs = "full";
                }
                if (flag_msmkfs == 0){
                    msmkfs = "3fs";
                }
                nodolist *nodo = los->BMount(idmkfs);
                if (nodo != nullptr){


                }else cout << "No existe la particion para mkfs 45" << endl;


            }else {
                estadomkfs = 0;
                flag = 0;
                cout << "Error se esperaba un id" << endl;
            }


        }else {
            estadomkfs = 0;
            flag = 0;
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





void Mkfs::FormatearEXT2(int inicio,int fin,int tamano,QString direccion){

    FILE *disco = fopen(direccion.toStdString().c_str(), "r+b");
        if(disco == NULL){
            cout << "No existe el disco" << endl;
            return;
        }
        char buffer = '0';
          char buffer2 = '1';
          char buffer3 = '2';

    int numero_estructuras = 0;
    superBloque super;
    inodeTable inodo;
    bloqueCarpetas bloque;
    numero_estructuras = Calcular_estructuras( tamano,"ext2");
    int bitmap_inodo = inicio + sizeof(superBloque);
    int bitmap_bloques = inicio + sizeof(superBloque) + numero_estructuras;

    super.s_filesystem_type = 2;
    super.s_inodes_count = numero_estructuras;
    super.s_blocks_count = 3 * numero_estructuras;
    super.s_free_inodes_count = numero_estructuras -2;
    super.s_free_block_count = (3 * numero_estructuras) - 2;
    cout << "count " << super.s_free_block_count << endl;
    time_t t = time(nullptr);
    tm *now = localtime(&t);
    string dateC = to_string(now->tm_mday) + "/" + to_string((now->tm_mon+1)) + "/" + to_string((now->tm_year + 1900)) + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min);
    super.s_mtime = '0';
    super.s_umtime = 0;
    super.s_mnt_count = 0;
    super.s_magic = 0xEF53;
    super.s_inode_size = sizeof(inodeTable);
    super.s_block_size = sizeof(bloqueArchivo);
    super.s_first_ino = 2;
    super.s_first_blo = 2;
    super.s_bm_inode_start = inicio + sizeof(superBloque);
    super.s_bm_block_start = bitmap_bloques;
    super.s_inode_start = inicio + sizeof(superBloque) + numero_estructuras + (3* numero_estructuras);
    super.s_block_start = inicio + sizeof(superBloque) + numero_estructuras + (3* numero_estructuras) + (sizeof(inodeTable) * numero_estructuras);
    cout << "super 2" << super.s_block_start << endl;



    fseek(disco,inicio,SEEK_SET);
    fwrite(&super,sizeof(superBloque),1,disco);

    for(int i = 0; i < numero_estructuras; i++){
        fseek(disco,super.s_bm_inode_start + (i*sizeof(char)),SEEK_SET);
        fwrite(&buffer,(sizeof(char)),1,disco);
    }
    /*for(int i = 0; i < 2; i++){
        fseek(disco,super.s_bm_inode_start + (i*sizeof(char)),SEEK_SET);
        fwrite(&buffer2,(sizeof(char)),1,disco);
    }*/
    fseek(disco,super.s_bm_inode_start,SEEK_SET);
    fwrite(&buffer2,sizeof(char),1,disco);
    fwrite(&buffer2,sizeof(char),1,disco);

    for(int i = 0; i < (numero_estructuras*3); i++){
        fseek(disco,super.s_bm_block_start+(i*sizeof(char)),SEEK_SET);
        fwrite(&buffer,sizeof(char),1,disco);
    }
    fseek(disco,super.s_bm_block_start,SEEK_SET);
    fwrite(&buffer2,sizeof(char),1,disco);
    fwrite(&buffer3,sizeof(char),1,disco);
    //------------------------------------------------------
    inodo.i_uid = 1;
    inodo.i_gid = 1;
    inodo.i_size = 0;
    strcpy(inodo.i_atime, dateC.c_str());
    strcpy(inodo.i_ctime, dateC.c_str());
    strcpy(inodo.i_mtime, dateC.c_str());
    inodo.i_block[0] = 0;
    for(int i = 1 ; i < 15; i++){
        inodo.i_block[i] = -1;
    }
    inodo.i_type = '0';
    inodo.i_perm = 664;
    fseek(disco,super.s_inode_start,SEEK_SET);
    fwrite(&inodo,sizeof(inodeTable),1,disco);

    strcpy(bloque.b_content[0].b_name,".");
    bloque.b_content[0].b_inodo=0;

    strcpy(bloque.b_content[1].b_name,"..");
    bloque.b_content[1].b_inodo=0;

    strcpy(bloque.b_content[2].b_name,"user.txt");
    bloque.b_content[2].b_inodo=1;

    strcpy(bloque.b_content[3].b_name,".");
    bloque.b_content[3].b_inodo=-1;

    fseek(disco,super.s_block_start,SEEK_SET);
    fwrite(&bloque,sizeof(bloqueCarpetas),1,disco);

    inodo.i_uid = 1;
    inodo.i_gid = 1;
    inodo.i_size = 27;//
    strcpy(inodo.i_atime, dateC.c_str());
    strcpy(inodo.i_ctime, dateC.c_str());
    strcpy(inodo.i_mtime, dateC.c_str());
    inodo.i_block[0] = 1;
    for(int i = 1; i < 15; i++){
        inodo.i_block[i] = -1;
    }

    inodo.i_type = '1';
    inodo.i_perm = 755;

    fseek(disco,super.s_inode_start + sizeof(inodeTable),SEEK_SET);
    fwrite(&inodo,sizeof(inodeTable),1,disco);

    bloqueArchivo archivo;
    int pos=0;
    char caracter[65]="";
    char frase[64]="1,G,root\n1,U,root,root,123\n";
    int tamanio = strlen(frase);

    for(int i = 0; i < 64; ++i){
        if(pos < tamanio){
            caracter[i] = frase[pos];
            pos++;
        }else{
            char aux[10]="";
            caracter[i]=aux[0];
        }
    }
     memset(archivo.b_content,0,sizeof(archivo.b_content));
    strcpy(archivo.b_content,caracter);
    fseek(disco,super.s_block_start+sizeof(bloqueCarpetas),SEEK_SET);
    fwrite(&archivo,sizeof(bloqueArchivo),1,disco);
    cout << "Disco Formateado en  EXT 2" << endl;
    fclose(disco);


}

void Mkfs::FormatearEXT3(int inicio,int fin,int tamano,QString direccion){
    FILE *disco = fopen(direccion.toStdString().c_str(), "r+b");
        if(disco == NULL){
            cout << "No existe el disco" << endl;
            return;
        }
    superBloque super;
    inodeTable inodo;
    bloqueCarpetas bloque;
    int numero_estructuras = Calcular_estructuras(tamano,"ext3");
    int bitmap_inodo = inicio + sizeof(superBloque) + (sizeof(JOURNAL) * numero_estructuras);
    int bitmap_bloques = inicio + sizeof(superBloque) + numero_estructuras +(sizeof(JOURNAL) * numero_estructuras);

    super.s_filesystem_type = 3;
    super.s_inodes_count = numero_estructuras;
    super.s_blocks_count = (3 * numero_estructuras);
    super.s_free_inodes_count = numero_estructuras -2;
    super.s_free_block_count = (3 * numero_estructuras) - 2;
    time_t t = time(nullptr);
    tm *now = localtime(&t);
    string dateC = to_string(now->tm_mday) + "/" + to_string((now->tm_mon+1)) + "/" + to_string((now->tm_year + 1900)) + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min);
    super.s_mtime = '0';
    super.s_umtime = 0;
    super.s_mnt_count = 0;
    super.s_magic = 0xEF53;
    super.s_inode_size = sizeof(inodeTable);
    super.s_block_size = sizeof(bloqueArchivo);
    super.s_first_ino = 2;
    super.s_first_blo = 2;
    super.s_bm_inode_start = bitmap_inodo;
    super.s_bm_block_start = bitmap_bloques;
    super.s_inode_start = bitmap_bloques + (3* numero_estructuras);
    super.s_block_start = bitmap_bloques + (3* numero_estructuras) + (sizeof(inodeTable) * numero_estructuras);
    cout << "----------EXT3------" << endl;
    char buffer = '0';
      char buffer2 = '1';
      char buffer3 = '2';
    fseek(disco,inicio,SEEK_SET);
    fwrite(&super,sizeof(superBloque),1,disco);

    for(int i = 0; i < numero_estructuras; i++){
        fseek(disco,super.s_bm_inode_start + +(i*sizeof(char)) ,SEEK_SET);
        fwrite(&buffer,sizeof(char),1,disco);
    }
    /*for(int i = 0; i < 2; i++){
        fseek(disco,super.s_bm_inode_start + (i*sizeof(char)),SEEK_SET);
        fwrite(&buffer2,sizeof(char),1,disco);
    }*/
    fseek(disco,super.s_bm_inode_start,SEEK_SET);
    fwrite(&buffer2,sizeof(char),1,disco);
    fwrite(&buffer2,sizeof(char),1,disco);
    for(int i = 0; i < (numero_estructuras*3); i++){
        fseek(disco,super.s_bm_block_start+(i*sizeof(char)),SEEK_SET);
        fwrite(&buffer,sizeof(char),1,disco);
    }
    fseek(disco,super.s_bm_block_start,SEEK_SET);
    fwrite(&buffer2,sizeof(char),1,disco);
    fwrite(&buffer3,sizeof(char),1,disco);
    //------------------------------------------------------
    inodo.i_uid = 1;
    inodo.i_gid = 1;
    inodo.i_size = 0;
    strcpy(inodo.i_atime, dateC.c_str());
    strcpy(inodo.i_ctime, dateC.c_str());
    strcpy(inodo.i_mtime, dateC.c_str());
    inodo.i_block[0] = 0;
    //----------------------------------------
    for(int i = 1 ; i < 15; i++){
        inodo.i_block[i] = -1;
    }
    inodo.i_type = '0';
    inodo.i_perm = 664;

    fseek(disco,super.s_inode_start,SEEK_SET);
    fwrite(&inodo,sizeof(inodeTable),1,disco);

    strcpy(bloque.b_content[0].b_name,".");
    bloque.b_content[0].b_inodo=0;

    strcpy(bloque.b_content[1].b_name,"..");
    bloque.b_content[1].b_inodo=0;

    strcpy(bloque.b_content[2].b_name,"user.txt");
    bloque.b_content[2].b_inodo=1;

    strcpy(bloque.b_content[3].b_name,".");
    bloque.b_content[3].b_inodo=-1;

    fseek(disco,super.s_block_start,SEEK_SET);
    fwrite(&bloque,sizeof(bloqueCarpetas),1,disco);


    inodo.i_uid = 1;
    inodo.i_gid = 1;
    inodo.i_size = 27;//
    strcpy(inodo.i_atime, dateC.c_str());
    strcpy(inodo.i_ctime, dateC.c_str());
    strcpy(inodo.i_mtime, dateC.c_str());
    inodo.i_block[0] = 1;
    for(int i = 1; i < 15; i++){
        inodo.i_block[i] = -1;
    }

    inodo.i_type = '1';
    inodo.i_perm = 755;

    fseek(disco,super.s_inode_start + sizeof(inodeTable),SEEK_SET);
    fwrite(&inodo,sizeof(inodeTable),1,disco);

    bloqueArchivo archivo;
    int pos=0;
    char caracter[65]="";
    char frase[64]="1,G,root\n1,U,root,root,123\n";
    int tamanio = strlen(frase);

    for(int i = 0; i < 64; ++i){
        if(pos < tamanio){
            caracter[i] = frase[pos];
            pos++;
        }else{
            char aux[10]="";
            caracter[i]=aux[0];
        }
    }
    strcpy(archivo.b_content,caracter);
    fseek(disco,super.s_block_start+sizeof(bloqueCarpetas),SEEK_SET);
    fwrite(&archivo,sizeof(bloqueArchivo),1,disco);
    cout << "Disco Formateado en  EXT3 " << endl;
    fclose(disco);


}

int Mkfs::Calcular_estructuras(int tamano,QString tipo){
    if(tipo == "ext2"){
        int tam_superBlock = sizeof(superBloque);
        int tam_journaling = sizeof(JOURNAL);
        int tam_inodo = sizeof(inodeTable);
        int tam_bloque = sizeof (bloqueArchivo);

        int operacion = tamano - tam_superBlock;
        int operacion2 = 4 + tam_inodo + (3 * tam_bloque);
        return floor(operacion / operacion2);


    }else if (tipo == "ext3") {
        cout << "entra en ext 3 " <<endl;
        int tam_superBlock = sizeof(superBloque);
        int tam_journaling = sizeof(JOURNAL);
        int tam_inodo = sizeof(inodeTable);
        int tam_bloque = sizeof (bloqueArchivo);

        int operacion = tamano - tam_superBlock;
        int operacion2 = 4 + tam_inodo + (3 * tam_bloque) + tam_journaling;
        cout << operacion << " / " << operacion2 << endl;
        return floor(operacion / operacion2);
    }

}
