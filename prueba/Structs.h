#ifndef STRUCTS_H
#define STRUCTS_H
#include <ctime>
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
    time_t mbr_fecha_creacion;
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

#endif // STRUCTS_H
