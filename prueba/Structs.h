#ifndef STRUCTS_H
#define STRUCTS_H
#include <ctime>
#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include "qdebug.h"
#include "qstring.h"
#include <stdlib.h>
#include <ctime>
#include <dirent.h>
#include <QStringList>
typedef struct {
   char part_status;
   char part_type;
   char part_fit;
   int part_start;
   int part_size;
   char part_name[16];
}Partition;


typedef struct {
    int mbr_tamano;
    char mbr_fecha_creacion[16];
    int mbr_disksignature;
    char disk_fit;
    Partition mbr_partition[4];
}MBR;

typedef struct{
    char part_status;
    char part_fit;
    int part_start;
    int part_size;
    int part_next;
    char part_name[16];

}EBR;

typedef struct{
    int s_filesystem_type;
    int s_inodes_count;
    int s_blocks_count;
    int s_free_block_count;
    int s_free_inodes_count;
    time_t s_mtime;
    time_t s_umtime;
    int s_mnt_count;
    int s_magic;
    int s_inode_size;
    int s_block_size;
    int s_first_ino;
    int s_first_blo;
    int s_bm_inode_start;
    int s_bm_block_start;
    int s_inode_start;
    int s_block_start;
}superBloque;

typedef struct{
    int i_uid;
    int i_gid;
    int i_size;
    char i_atime[16];
    char i_ctime[16];
    char i_mtime[16];
    int i_block[15];
    char i_type;
    int i_perm;
}inodeTable;
typedef struct
{
    char b_name[12];
    int b_inodo;
}content;

typedef struct{
    content b_content[4];
}bloqueCarpetas;

typedef struct{
    char b_content[64];
}bloqueArchivo;


typedef struct{
    int Journal_tipo_operacion;
    int Journal_tipo;
    char Journal_nombre[50];
    char Journal_contenido[100];
    time_t Journal_fecha;
    char Journal_propietario[16];
    int Journal_Permisos;
}JOURNAL;

typedef struct{
    int id_user;
    int id_grup;
    int iniciosuper;
    QString direccion;
    char fit;
}USERLOG;
#endif // STRUCTS_H
