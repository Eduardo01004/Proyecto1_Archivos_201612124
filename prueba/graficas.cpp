#include "graficas.h"
#include <string>
#include <sstream>
Graficas::Graficas()
{

}
void Graficas:: GraficarMBR_EBR(QString direccion , QString destino, QString extension){
    int index_extendida = -1;
    FILE *disco;
    int index_ebr = 0;
    FILE *graph;
    int sub_total = 0;
    string body = "";
    disco=fopen(direccion.toStdString().c_str(),"r");
    if(disco != nullptr){
        graph = fopen("grafica.dot","w");
        body += "digraph G{ \nsubgraph cluster{\n label=\"MBR\"";
        body += "\nNodo[shape=box,label=<<TABLE BORDER=\"1\" CELLBORDER=\"1\" WIDTH=\"300\"  HEIGHT=\"200\" >\n";
        body += "<TR ><TD WIDTH=\"150\" ><b>Nombre</b></TD><TD WIDTH=\"150\"><b>Valor</b></TD></TR>\n";
        MBR auxmbr;
        fseek(disco,0,SEEK_SET);
        fread(&auxmbr,sizeof(MBR),1,disco);
        int total = auxmbr.mbr_tamano;
        body += "<TR><TD><b>mbr_tamaño</b></TD><TD>" + std::to_string(total) + "</TD></TR>\n";
        string s5;
        stringstream ss5;
        ss5 << auxmbr.mbr_fecha_creacion;
        ss5 >> s5;
        body += "<TR><TD><b>mbr_fecha_creacion</b></TD><TD>"+ s5 +"</TD></TR>\n";
        body += "<TR><TD><b>mbr_disk_signature</b></TD><TD>"+ std::to_string(auxmbr.mbr_disksignature)+"</TD></TR>\n";
        string s;
        stringstream ss;
        ss << auxmbr.disk_fit;
        ss >> s;
        body += "<tr><td><b>Disk_fit</b></td> <td>"+ s +"</td></tr>\n";

        for(int i = 0; i < 4; i++){
            int parcial = auxmbr.mbr_partition[i].part_size;
            sub_total = parcial+auxmbr.mbr_partition[i].part_start;
            if(auxmbr.mbr_partition[i].part_start == -1){
                break;
            }else{
                double porcent=(double)(parcial*100)/total;
                double porcent2=(double)(porcent*450)/100;
                if(auxmbr.mbr_partition[i].part_status != '1'){
                    if(auxmbr.mbr_partition[i].part_type == 'E'){
                        index_extendida=i;
                    }
                    char status[3];
                    sprintf(status,"%c",auxmbr.mbr_partition[i].part_status);
                    if(strcmp(status,"") == 0) strcpy(status,"0");
                    string s;
                    stringstream ss;
                    ss << status;
                    ss >> s;
                    body += "<TR><TD><b>part_status_"+ std::to_string(i + 1)+ "</b></TD><TD>"+ s +"</TD></TR>\n";
                    body += "<TR><TD><b>part_type_"+std::to_string(i + 1) +"</b></TD><TD><b>"+auxmbr.mbr_partition[i].part_type+"</b></TD></TR>\n";
                    body += "<TR><TD><b>part_fit_"+std::to_string(i + 1) +"</b></TD><TD><b>"+auxmbr.mbr_partition[i].part_fit+"</b></TD></TR>\n";
                    body += "<TR><TD><b>part_start_"+std::to_string(i + 1)+"</b></TD><TD>"+std::to_string(auxmbr.mbr_partition[i].part_start)+"</TD></TR>\n";
                    body += "<TR><TD><b>part_size_"+std::to_string(i + 1)+"</b></TD><TD>"+std::to_string(auxmbr.mbr_partition[i].part_size)+"</TD></TR>\n";
                    body += "<TR><TD><b>part_name_"+std::to_string(i + 1)+"</b></TD><TD>"+auxmbr.mbr_partition[i].part_name+"</TD></TR>\n";


                }
            }
        }
        //------------------------------EBR-------------------------------
        body += "</TABLE>>];\n}";
        if(index_extendida != -1){
            fseek(disco,auxmbr.mbr_partition[index_extendida].part_start,SEEK_SET);
            EBR auxebr;
            while((fread(&auxebr,sizeof(EBR),1,disco)) !=0 && ftell(disco)<(auxmbr.mbr_partition[index_extendida].part_size+auxmbr.mbr_partition[index_extendida].part_start)){
                if(auxebr.part_status != '1'){
                    body += "subgraph ebr_"+std::to_string(index_ebr)+"{\n label=\"EBR_"+std::to_string(index_ebr)+"\"\n";
                    body += "\nNodo_"+std::to_string(index_ebr)+"[shape=box,label=<<TABLE BORDER=\"0\" CELLBORDER=\"1\" WIDTH=\"200\"  HEIGHT=\"150\" >\n";
                    body += "<TR ><TD WIDTH=\"150\" ><b>Nombre</b></TD><TD WIDTH=\"150\"><b>Valor</b></TD></TR>\n";
                    char status[3];
                    sprintf(status,"%c",auxebr.part_status);
                     if(strcmp(status,"")==0) strcpy(status,"0");
                     string s;
                     stringstream ss;
                     ss << status;
                     ss >> s;
                    body += "<TR><TD><b>part_status_1</b></TD><TD>"+s+"</TD></TR>\n";
                    string s1;
                    stringstream ss1;
                    cout << "este es fit: " << auxebr.part_fit << endl;
                    ss1 << auxebr.part_fit;
                    ss1 >> s1;
                    body += "<TR><TD><b>part_fit_1</b></TD><TD><b>"+s1+"</b></TD></TR>\n";
                    body += "<TR><TD><b>part_start_1</b></TD><TD>"+std::to_string(auxebr.part_start)+"</TD></TR>\n";
                    body += "<TR><TD><b>part_size_1</b></TD><TD>"+std::to_string(auxebr.part_size)+"</TD></TR>\n";
                    string s2;
                    stringstream ss2;
                    ss2 << auxebr.part_name;
                    ss2 >> s2;
                    body += "<TR><TD><b>part_name_1</b></TD><TD>"+s2+"</TD></TR>\n";
                    body +="</TABLE>>];\n}\n";
                    index_ebr++;
                }

                if(auxebr.part_next != -1){

                    fseek(disco,auxebr.part_next,SEEK_SET);
                }
                  else {
                    break;
                }
            }
       }

    fclose(disco);
    body +="}\n";
    fclose(graph);
    ofstream file;
        string pathTxt = direccion.toStdString() + ".txt";
        string pathJpg = direccion.toStdString() + "."+extension.toStdString();
        file.open(pathTxt);
        if(file.fail()){
            std::cout << "Error al abrir el txt\n";
            return;
        }
        file << body << std::endl;
        file.close();
        std::string pathUnion = "dot " + pathTxt + " -o " + pathJpg + " -T" +extension.toStdString();
            system(pathUnion.c_str());
    string abrir = "eog " + pathJpg;
    system(abrir.c_str());
    cout << "Grafica del MBR REalizada con exito" << endl;

    }   else cout << "No se encuentra el disco" << endl;

}

void Graficas::GraficarDisk(QString direccion , QString destino, QString extension){
    double cant_libre = 0;
    FILE *disco;
    int flag_libres = 1;
    FILE *graph;
    int sub_total = 0;
    if ((disco = fopen(direccion.toStdString().c_str(),"r"))){
        graph=fopen("grafica.dot","w");
        fprintf(graph,"digraph G{\nNodo[shape=box,label=<<TABLE BORDER=\"0\" CELLBORDER=\"1\" WIDTH=\"500\"  HEIGHT=\"200\" >\n<TR>\n<TD HEIGHT=\"200\" WIDTH=\"50\">MBR</TD>\n");
        MBR auxmbr;
        fseek(disco,0,SEEK_SET);
        fread(&auxmbr,sizeof(MBR),1,disco);
        int total = auxmbr.mbr_tamano;
        for(int i = 0; i < 4; i++){
            int parcial = auxmbr.mbr_partition[i].part_size;
            sub_total = parcial + sub_total;
            if(auxmbr.mbr_partition[i].part_start == -1){
                flag_libres = 0;
                parcial = total - (auxmbr.mbr_partition[i-1].part_size + auxmbr.mbr_partition[i-1].part_start);
                double porcent = (double)(parcial*100)/total;
                cant_libre = cant_libre + porcent;
                double porcent2 = (double)(porcent*450)/100;
                fprintf(graph, "<TD WIDTH=\"%f\" HEIGHT=\"200\">LIBRE<br/>Ocupado: %f%c</TD>\n",porcent2,porcent,'%');
                break;
            }else{
                double porcent = (parcial*100)/total;
                cant_libre=cant_libre + porcent;
                double porcent2 = (porcent * 450) /100;
                if(auxmbr.mbr_partition[i].part_status != '1'){
                    if(auxmbr.mbr_partition[i].part_type == 'E'){
                        fprintf(graph, "<TD HEIGHT=\"200\" WIDTH=\"%f\">\n<TABLE BORDER=\"0\"  HEIGHT=\"200\" WIDTH=\"%f\" CELLBORDER=\"1\">\n<TR>\n<TD HEIGHT=\"60\" WIDTH=\"%f\">Extendida</TD>\n</TR>\n<TR>\n",porcent,porcent,porcent);
                        fseek(disco,auxmbr.mbr_partition[i].part_start,SEEK_SET);
                        EBR auxebr;
                        while((fread(&auxebr,sizeof(EBR),1,disco)) != 0 && ftell(disco) < (auxmbr.mbr_partition[i].part_size + auxmbr.mbr_partition[i].part_start)){
                                parcial = auxebr.part_size;
                                 porcent = (double)(parcial*100)/total;
                                 if(porcent != 0){
                                    if(auxebr.part_status != '1'){
                                        fprintf(graph, "<TD HEIGHT=\"150\">EBR </TD>\n");
                                        fprintf(graph, "<TD HEIGHT=\"150\" >LOGICA <br/> Ocupado: %f%c</TD>\n",porcent,'%');
                                    }else{
                                        fprintf(graph, "<TD HEIGHT=\"150\" >LIBRE <br/> Ocupado: %f%c</TD>\n",porcent,'%');
                                    }

                                    fseek(disco,auxebr.part_next,SEEK_SET);
                                    if(auxebr.part_next != -1){
                                    }else{
                                        parcial = (auxmbr.mbr_partition[i].part_size + auxmbr.mbr_partition[i].part_start) - (auxebr.part_size +auxebr.part_start);
                                         porcent = (double)(parcial * 100) /total;
                                        fprintf(graph, "<TD HEIGHT=\"150\" >LIBRE <br/> Ocupado: %f%c</TD>\n",porcent,'%');
                                        break;
                                    }
                                 }
                            }
                            fprintf(graph, "<TD>1\n</TD></TR>\n</TABLE>\n</TD>\n");

                    }else{
                        fprintf(graph, "<TD HEIGHT=\"150\" WIDTH=\"%f\">PRIMARIA <br/> Ocupado: %f%c</TD>\n",porcent2,porcent,'%');
                    }
                }else{
                    fprintf(graph, "<TD HEIGHT=\"150\" WIDTH=\"%f\">LIBRE <br/> Ocupado: %f%c</TD>\n",porcent2,porcent,'%');
                }
            }
        }

        fprintf(graph, "<TD HEIGHT=\"200\">LIBRE <br/> Ocupado: %f%c</TD>\n",100 - cant_libre,'%');
        fclose(disco);
        fprintf(graph,"</TR>\n</TABLE>>];\n}");
        fclose(graph);
        char comando[400];
        strcpy(comando,"dot -T");
        strcat(comando,extension.toStdString().c_str());
        strcat(comando," grafica.dot -o ");
        strcat(comando,destino.toStdString().c_str());
        system(comando);
        QString abrir = "eog " + destino;
        system(abrir.toStdString().c_str());
        cout << "Grafica DISK Realizada con exito " <<endl;
    }else cout << "el disco no existe " << endl;



}


void Graficas:: Inode(QString direccion, QString destino, QString extension,int biS,int iS,int biE,int caca){
    int numeroInode=0;
        char  buff;
        FILE *disco;
        FILE *discoaux;
        FILE *graph;
        if(disco = fopen(direccion.toStdString().c_str(),"r")){
            graph = fopen("grafica.dot","w");
            fprintf(graph,"digraph G{\n");
            discoaux = fopen(direccion.toStdString().c_str(),"r");

            inodeTable inode;
           fseek(discoaux,biS,SEEK_SET);
            fseek(disco,iS,SEEK_SET);
            while(ftell(discoaux) < biE){
                buff = fgetc(discoaux);
                fread(&inode,sizeof(inodeTable),1,disco);
                if(buff == '1'){
                    fprintf(graph,"subgraph inode_%d{\n",numeroInode);
                    fprintf(graph, "nodo_%d [ shape=none,",numeroInode);
                    fprintf(graph, "label=< <TABLE> <TR> <TD COLSPAN=\"2\"> Inodo %d </TD></TR>\n",numeroInode);
                    fprintf(graph, "<TR> <TD> i_uid </TD> <TD> %d </TD></TR>\n",inode.i_uid);
                    fprintf(graph, "<TR> <TD> i_gid </TD> <TD> %d </TD></TR>\n",inode.i_gid);
                    fprintf(graph, "<TR> <TD> i_size </TD> <TD> %d </TD></TR>\n",inode.i_size);
                    fprintf(graph, "<TR> <TD> i_gid </TD> <TD> %d </TD></TR>\n",inode.i_gid);
                    string s5;
                    stringstream ss5;
                    ss5 << inode.i_atime;
                    ss5 >> s5;
                    fprintf(graph, "<TR> <TD> i_atime </TD> <TD> %s </TD></TR>\n",inode.i_atime);
                    string s1;
                    stringstream ss1;
                    ss1 << inode.i_ctime;
                    ss1 >> s1;
                    fprintf(graph, "<TR> <TD> i_ctime </TD> <TD> %s </TD></TR>\n",inode.i_ctime);
                    //tm = localtime(&inode.i_mtime);
                    string s2;
                    stringstream ss2;
                    ss2 << inode.i_mtime;
                    ss2 >> s2;
                    fprintf(graph, "<TR> <TD> i_mtime </TD> <TD> %s </TD></TR>\n",inode.i_mtime);
                    for(int i = 0; i < 15;i++){
                        fprintf(graph, "<TR> <TD> i_block_%d </TD> <TD> %d </TD></TR>\n",i,inode.i_block[i]);
                    }
                    fprintf(graph, "<TR> <TD> i_type </TD> <TD> %c </TD></TR>\n",inode.i_type);
                    fprintf(graph, "<TR> <TD> i_perm </TD> <TD> %d </TD></TR>\n",inode.i_perm);
                    fprintf(graph, "</TABLE>>]\n");
                    fprintf(graph,"}\n");
                }

                numeroInode++;
            }

            fclose(disco);
            fprintf(graph,"\n}");
            fclose(graph);
            char comando[400];
            strcpy(comando,"dot -T");
            strcat(comando,extension.toStdString().c_str());
            strcat(comando," grafica.dot -o ");
            strcat(comando,destino.toStdString().c_str());
            system(comando);
            fclose(discoaux);
            cout << "Grafica de Inodos Realizada con exito" << endl;
            cout << endl;

            }else{
                printf("Error el disco no existe\n");
            }
}


void Graficas:: Bloques(QString direccion, QString destino, QString extension,int biS,int iS,int biE,int start_super){
    int numerobloque = 0;
    char  buff;
    FILE *disco;
    FILE *discoaux;
    FILE *graph;
    if(disco = fopen(direccion.toStdString().c_str(),"r")){
        graph = fopen("grafica.dot","w");
        fprintf(graph,"digraph G{\n");
        discoaux = fopen(direccion.toStdString().c_str(),"r");
        superBloque super;
        bloqueArchivo archivo;
        bloqueCarpetas carpeta;
        fseek(disco,start_super,SEEK_SET);
        fread(&super,sizeof(superBloque),1,disco);

        fseek(discoaux,super.s_bm_block_start,SEEK_SET);
        fseek(disco,super.s_block_start,SEEK_SET);
        while(ftell(discoaux) < biE){
            fread(&buff,sizeof (char),1,discoaux);
            fseek(disco,super.s_block_start+(sizeof(bloqueArchivo)*numerobloque),SEEK_SET);
            if(buff == '1'){
                fread(&carpeta,sizeof(bloqueCarpetas),1,disco);
                fprintf(graph,"subgraph inode_%d{\n",numerobloque);
                fprintf(graph, "nodo_%d [ shape=none,",numerobloque);
                fprintf(graph, "label=< <TABLE> <TR> <TD COLSPAN=\"2\"> Bloque de Carpeta %d </TD></TR>\n",numerobloque);
                fprintf(graph, "<TR> <TD> b_name </TD> <TD> b_inode </TD></TR>\n");
                for(int i = 0; i < 4; i++){
                    fprintf(graph, "<TR> <TD> %s </TD> <TD> %d </TD></TR>\n",carpeta.b_content[i].b_name,carpeta.b_content[i].b_inodo);
                }
                fprintf(graph, "</TABLE>>]\n");
                fprintf(graph,"}\n");
            }else if(buff == '2'){
                fread(&archivo,sizeof(bloqueCarpetas),1,disco);
                fprintf(graph,"subgraph inode_%d{\n",numerobloque);
                fprintf(graph, "nodo_%d [ shape=none,",numerobloque);
                fprintf(graph, "label=< <TABLE> <TR> <TD COLSPAN=\"2\"> Bloque de Archivo %d </TD></TR>\n",numerobloque);
                fprintf(graph, "<TR> <TD COLSPAN=\"2\"> %s </TD></TR>\n",archivo.b_content);
                fprintf(graph, "</TABLE>>]\n");
                fprintf(graph,"}\n");
            }

            numerobloque++;
        }

        fclose(disco);
        fprintf(graph,"\n}");
        fclose(graph);
        char comando[400];
        strcpy(comando,"dot -T");
        strcat(comando,extension.toStdString().c_str());
        strcat(comando," grafica.dot -o ");
        strcat(comando,destino.toStdString().c_str());
        system(comando);
        cout << "Grafica de Bloques realizada con Exito" << endl;
        cout << endl;

    }else{
        printf("Error el disco no existe\n");
    }

}


void Graficas::CrearReporte(int inicio,int final,QString direccion,QString path){
    FILE *disco;
    FILE *disco2;
    char byte;
    char sat = '\n';
    int salto = 0;
    disco2 = fopen(direccion.toStdString().c_str(),"w+");
    fseek(disco2,0,SEEK_SET);
    disco = fopen(path.toStdString().c_str(),"rb+");
    for(int i = 0; i < final; i++){
        fseek(disco,inicio+(sizeof(char)*i),SEEK_SET);
        fread(&byte,sizeof (char),1,disco);
        if(byte != '0') byte = '1';
        fputc(byte,disco2);
        if(salto == 19){
            salto=0;
            fputc(sat,disco2);
        }else salto++;
    }
    fclose(disco);
    fclose(disco2);
    cout << "Grafica Realizada con exito" << endl;
    cout <<endl;

}


void Graficas::graficarSuper(QString direccion, QString destino, QString extension, int start_super){
    superBloque super;
        FILE* disco;
        FILE* graph;
        disco = fopen(direccion.toStdString().c_str(),"r");
        if (disco != nullptr){
            fseek(disco,start_super,SEEK_SET);
            fread(&super,sizeof(superBloque),1,disco);
            graph = fopen("grafica.dot","w");
                fprintf(graph,"digraph G{\n");
                        fprintf(graph,"subgraph super{\n");
                        fprintf(graph, "nodo [ shape=none,");
                        fprintf(graph, "label=< <TABLE> <TR> <TD COLSPAN=\"2\"> SUPERBLOQUE </TD></TR>\n");
                        fprintf(graph, "<TR> <TD> Nombre </TD> <TD> Valor </TD></TR>\n");
                        fprintf(graph, "<TR> <TD> s_inodes_count </TD> <TD> %d </TD></TR>\n",super.s_inodes_count);
                        fprintf(graph, "<TR> <TD> s_blocks_count </TD> <TD> %d </TD></TR>\n",super.s_blocks_count);
                        fprintf(graph, "<TR> <TD> s_free_block_count </TD> <TD> %d </TD></TR>\n",super.s_free_block_count);
                        fprintf(graph, "<TR> <TD> s_free_inodes_count </TD> <TD> %d </TD></TR>\n",super.s_free_inodes_count);

                        struct tm *tm;
                        char fecha[100];
                        tm=localtime(&super.s_mtime);
                        strftime(fecha,100,"%d/%m/%y %H:%S",tm);
                        fprintf(graph, "<TR> <TD> s_mtime </TD> <TD> %s </TD></TR>\n",fecha);


                        tm=localtime(&super.s_umtime);
                        strftime(fecha,100,"%d/%m/%y %H:%S",tm);
                        fprintf(graph, "<TR> <TD> s_umtime </TD> <TD> %s </TD></TR>\n",fecha);

                        fprintf(graph, "<TR> <TD> s_mnt_count </TD> <TD> %d </TD></TR>\n",super.s_mnt_count);
                        fprintf(graph, "<TR> <TD> s_magic </TD> <TD> %d </TD></TR>\n",super.s_magic);
                        fprintf(graph, "<TR> <TD> s_inode_size </TD> <TD> %d </TD></TR>\n",super.s_inode_size);
                        fprintf(graph, "<TR> <TD> s_block_size </TD> <TD> %d </TD></TR>\n",super.s_block_size);
                        fprintf(graph, "<TR> <TD> s_first_ino </TD> <TD> %d </TD></TR>\n",super.s_first_ino);
                        fprintf(graph, "<TR> <TD> s_first_blo </TD> <TD> %d </TD></TR>\n",super.s_first_blo);
                        fprintf(graph, "<TR> <TD> s_bm_inode_start </TD> <TD> %d </TD></TR>\n",super.s_bm_inode_start);
                        fprintf(graph, "<TR> <TD> s_block_start </TD> <TD> %d </TD></TR>\n",super.s_block_start);

                        fprintf(graph, "</TABLE>>]\n");
                        fprintf(graph,"}\n");

            fprintf(graph,"\n}");
        }else cout << "el disco no existe" <<endl;
        fclose(graph);
        fclose(disco);

        char comando[400];
        strcpy(comando,"dot -T");
        strcat(comando,extension.toStdString().c_str());
        strcat(comando," grafica.dot -o ");
        strcat(comando,destino.toStdString().c_str());
        system(comando);
    cout << "Reporte SuperBloque generado con exito " << endl;
}


void Graficas:: Graficar_Tree(QString direccion, QString destino, QString ext,int start ){
    int numeroInode = 0;
    char  buff;
    char buff2;
    FILE *disco;
    FILE *discoaux;
    int flag_libres = 1;
    FILE *graph;
    int sub_total = 0;
    superBloque super;
    disco=fopen(direccion.toStdString().c_str(),"r");
    fseek(disco,start,SEEK_SET);
    fread(&super,sizeof(superBloque),1,disco);
    if(disco != nullptr){
        graph=fopen("grafica.dot","w");
        fprintf(graph,"digraph G{ rankdir=\"LR\"\n");
        discoaux=fopen(direccion.toStdString().c_str(),"r");
        inodeTable inode;
        bloqueCarpetas carpeta;
        bloqueArchivo archivo;
       fseek(discoaux,super.s_bm_inode_start,SEEK_SET);
        fseek(disco,super.s_inode_start,SEEK_SET);
        while(ftell(discoaux) < super.s_bm_block_start){
            buff = fgetc(discoaux);
            fseek(disco,super.s_inode_start+(sizeof(inodeTable)*numeroInode),SEEK_SET);
            fread(&inode,sizeof(inodeTable),1,disco);
            if(buff != '0'){
                fprintf(graph,"subgraph inode_%d{\n",numeroInode);
                fprintf(graph, "inodo_%d [ shape=none,",numeroInode);
                fprintf(graph, "label=< <TABLE> <TR> <TD COLSPAN=\"2\"> Inodo %d </TD></TR>\n",numeroInode);
                fprintf(graph, "<TR> <TD> i_uid </TD> <TD> %d </TD></TR>\n",inode.i_uid);
                fprintf(graph, "<TR> <TD> i_gid </TD> <TD> %d </TD></TR>\n",inode.i_gid);
                fprintf(graph, "<TR> <TD> i_size </TD> <TD> %d </TD></TR>\n",inode.i_size);
                fprintf(graph, "<TR> <TD> i_gid </TD> <TD> %d </TD></TR>\n",inode.i_gid);

                fprintf(graph, "<TR> <TD> i_atime </TD> <TD> %s </TD></TR>\n",inode.i_atime);
                fprintf(graph, "<TR> <TD> i_ctime </TD> <TD> %s </TD></TR>\n",inode.i_ctime);
                fprintf(graph, "<TR> <TD> i_mtime </TD> <TD> %s </TD></TR>\n",inode.i_mtime);
                for(int i = 0; i < 15; i++){
                    fprintf(graph, "<TR> <TD> i_block_%d </TD> <TD> %d </TD></TR>\n",i,inode.i_block[i]);
                }
                fprintf(graph, "<TR> <TD> i_type </TD> <TD> %c </TD></TR>\n",inode.i_type);
                fprintf(graph, "<TR> <TD> i_perm </TD> <TD> %d </TD></TR>\n",inode.i_perm);
                fprintf(graph, "</TABLE>>]\n");
                fprintf(graph,"}\n");
                for(int i = 0; i < 15; i++){
                    if(inode.i_block[i] != -1){
                        fprintf(graph, "inodo_%d->bloque_%d;\n",numeroInode,inode.i_block[i]);
                        fseek(disco,super.s_bm_block_start+(sizeof(char)*inode.i_block[i]),SEEK_SET);
                        buff2 = fgetc(disco);
                        if(buff2 == '1'){
                            fseek(disco,super.s_block_start+(sizeof(bloqueCarpetas)*inode.i_block[i]),SEEK_SET);
                            fread(&carpeta,sizeof(bloqueCarpetas),1,disco);
                            fprintf(graph, "bloque_%d [ shape=none,",inode.i_block[i]);
                            fprintf(graph, "label=< <TABLE> <TR> <TD COLSPAN=\"2\"> Bloque de Carpeta %d </TD></TR>\n",inode.i_block[i]);
                            fprintf(graph, "<TR> <TD> b_name </TD> <TD> b_inode </TD></TR>\n");
                            for(int j = 0; j < 4; j++){
                                fprintf(graph, "<TR> <TD> %s </TD> <TD> %d </TD></TR>\n",carpeta.b_content[j].b_name,carpeta.b_content[j].b_inodo);
                            }
                            fprintf(graph, "</TABLE>>];\n");
                            for(int j=0 ; j < 4; j++){
                                if((strcmp(carpeta.b_content[j].b_name,".")!=0 && strcmp(carpeta.b_content[j].b_name,"..")!=0 ) && carpeta.b_content[j].b_inodo!=-1){
                                    fprintf(graph, "bloque_%d->inodo_%d;\n",inode.i_block[i],carpeta.b_content[j].b_inodo);
                                }
                            }
                        }else if(buff2 == '2'){
                            fseek(disco,super.s_block_start+(sizeof(bloqueCarpetas)*inode.i_block[i]),SEEK_SET);
                            fread(&archivo,sizeof(bloqueCarpetas),1,disco);
                            fprintf(graph, "bloque_%d [ shape=none,",inode.i_block[i]);
                            fprintf(graph, "label=< <TABLE> <TR> <TD COLSPAN=\"2\"> Bloque de Archivo %d </TD></TR>\n",inode.i_block[i]);
                            fprintf(graph, "<TR> <TD COLSPAN=\"2\"> %s </TD></TR>\n",archivo.b_content);
                            fprintf(graph, "</TABLE>>]\n");
                        }
                    }
                }
            }

            numeroInode++;
        }

        fclose(disco);
        fprintf(graph,"\n}");
        fclose(graph);
        char comando[400];
        strcpy(comando,"dot -T");
        strcat(comando,ext.toStdString().c_str());
        strcat(comando," grafica.dot -o ");
        strcat(comando,destino.toStdString().c_str());
        system(comando);
        fclose(discoaux);
        cout << "GRAFICA TREE REALIZADA CON EXITO " << endl;
        cout << endl;
    }else{
        printf("Error el disco no existe\n");
    }

}

void Graficas::reporteFile(QString path,QString destino,char *pa_path,int inicio){
    FILE* disco;
    FILE* archivo;
    inodeTable inodo;
    superBloque super;
    disco = fopen(path.toStdString().c_str(),"rb+");
    archivo=fopen(destino.toStdString().c_str(),"w+");
    char cont[1000];
    memset(cont,0,sizeof(cont));
    strcpy(cont,"");
    int numeracion=0;
    int resu=0;
    fseek(disco,inicio,SEEK_SET);
    fread(&super,sizeof(superBloque),1,disco);
    resu = dirm.BuscarCoA(disco,pa_path,inicio,&numeracion);
    if(resu == 1){
        fseek(disco,super.s_inode_start+(sizeof(inodeTable)*numeracion),SEEK_SET);
        fread(&inodo,sizeof(inodeTable),1,disco);
        for(int i=0;i<15;i++){
            if(inodo.i_block[i]!=-1){
                bloqueArchivo barchivo;
                fseek(disco,super.s_block_start+(sizeof(bloqueCarpetas)*inodo.i_block[i]),SEEK_SET);
                fread(&barchivo,sizeof(bloqueArchivo),1,disco);
                strcat(cont,barchivo.b_content);
            }else break;
        }
        cout << "Reporte FILE " << endl;
        fprintf(archivo, "%s\n",cont);
        cout << "contenido " << cont <<  endl;
        cout << "reporte FIle realizado con exito " << endl;
    }else printf("No existe el archivo para hacer el reporte\n");
    fclose(archivo);
    fclose(disco);
}

void Graficas::reporteLS(QString path,int inicio,QString extension,QString destino,int usuario,int grupo,char*pa_path){
    char byte='0';
        FILE* disco;
        FILE* graph;
        inodeTable inodo;
        superBloque super;
        disco=fopen(path.toStdString().c_str(),"rb+");
        graph=fopen("grafica.dot","w");
        char cont[1000];
        memset(cont,0,sizeof(cont));
        strcpy(cont,"");
        int numeracion=0;
        int resu=0;
        fseek(disco,inicio,SEEK_SET);
        fread(&super,sizeof(superBloque),1,disco);
        resu = dirm.BuscarCoA(disco,pa_path,inicio,&numeracion);
        cout << "inicio " << inicio << endl;
        cout << "usuario " << usuario << endl;
        cout << "grupo " << grupo << endl;
        if(resu == 1){
            fprintf(graph,"digraph G{\n");
            fprintf(graph, "nodo [ shape=none,");
            fprintf(graph, "label=< <TABLE>\n");
            fprintf(graph,"<TR> <TD>PERMISOS</TD><TD>OWNER</TD><TD>GRUPO</TD><TD>SIZE</TD><TD>FECHA</TD><TD>HORA</TD><TD>TIPO</TD><TD>NAME</TD></TR>\n");
            fseek(disco,super.s_inode_start+(sizeof(inodeTable)*numeracion),SEEK_SET);
            fread(&inodo,sizeof(inodeTable),1,disco);
            for(int i=0;i<15;i++){
                if(inodo.i_block[i]!=-1){
                    bloqueArchivo barchivo;
                    bloqueCarpetas carp;
                    inodeTable auxInodo;
                    fseek(disco,super.s_block_start+(sizeof(bloqueArchivo)*inodo.i_block[i]),SEEK_SET);
                    fread(&carp,sizeof(bloqueArchivo),1,disco);
                    for(int j = 2; j < 4; j++){
                        if(carp.b_content[j].b_inodo!=-1){
                            fseek(disco,super.s_inode_start+(sizeof(inodeTable)*carp.b_content[j].b_inodo),SEEK_SET);
                            fread(&auxInodo,sizeof(inodeTable),1,disco);
                            int milla=(auxInodo.i_perm/1000);
                            int usuari=(auxInodo.i_perm-(milla*1000))/100;
                            int gru=(auxInodo.i_perm-(milla*1000+usuari*100))/10;
                            int otros=auxInodo.i_perm-(milla*1000+usuari*100+gru*10);

                            char perm[50];
                            memset(perm,0,sizeof(perm));
                            if(usuari==0) strcpy(perm,"- - -");
                            else if(usuari==1) strcpy(perm, "- - x");
                            else if(usuari==2) strcpy(perm,"- w -");
                            else if(usuari==3) strcpy(perm,"- w x");
                            else if(usuari==4) strcpy(perm,"r - -");
                            else if(usuari==5) strcpy(perm,"r - x");
                            else if(usuari==6) strcpy(perm,"r w -");
                            else strcpy(perm,"r w x");

                            if(gru==0) strcat(perm,"- - -");
                            else if(gru==1) strcat(perm, "- - x");
                            else if(gru==2) strcat(perm,"- w -");
                            else if(gru==3) strcat(perm,"- w x");
                            else if(gru==4) strcat(perm,"r - -");
                            else if(gru==5) strcat(perm,"r - x");
                            else if(gru==6) strcat(perm,"r w -");
                            else strcat(perm,"r w x");

                            if(otros==0) strcat(perm,"- - -");
                            else if(otros==1) strcat(perm, "- - x");
                            else if(otros==2) strcat(perm,"- w -");
                            else if(otros==3) strcat(perm,"- w x");
                            else if(otros==4) strcat(perm,"r - -");
                            else if(otros==5) strcat(perm,"r - x");
                            else if(otros==6) strcat(perm,"r w -");
                            else strcat(perm,"r w x");
                            fprintf(graph,"<TR> <TD>%s</TD><TD>%d</TD><TD>%d</TD><TD>%d</TD>",perm,usuario,grupo,inodo.i_size);
                            fprintf(graph, "<TD>%s</TD><TD>%s</TD>",&auxInodo.i_atime,&auxInodo.i_atime );
                            fseek(disco,super.s_bm_block_start+(sizeof(char)*carp.b_content[j].b_inodo),SEEK_SET);
                            byte = fgetc(disco);
                            if(byte=='1'){
                                fprintf(graph, "<TD>Carpeta</TD><TD>%s</TD></TR>",carp.b_content[j].b_name);
                            }else if(byte=='2'){
                                fprintf(graph, "<TD>Archivo</TD><TD>%s</TD></TR>\n",carp.b_content[j].b_name);
                            }
                        }
                    }
                }else break;
            }
            fprintf(graph, "</TABLE>>];\n");
            fprintf(graph,"\n}");
        }else printf("No existe el archivo para hacer el reporte\n");

        fclose(graph);
        char comando[400];
        strcpy(comando,"dot -T");
        strcat(comando,extension.toStdString().c_str());
        strcat(comando," grafica.dot -o ");
        strcat(comando,destino.toStdString().c_str());
        system(comando);
        fclose(disco);
}


void Graficas::reportarJournaling(QString destino,QString direccion,int inicio,int inicio_journal){
    FILE* arch;
    arch = fopen(destino.toStdString().c_str(),"w");
    fprintf(arch, "OPERACION,TIPO,NOMBRE,CONTENIDO,FECHA,PROPIETARIO,PERMISOS\n" );
    JOURNAL journal;
    FILE* disco;
    superBloque super;
    disco=fopen(direccion.toStdString().c_str(),"rb+");
    fseek(disco,inicio,SEEK_SET);
    fread(&super,sizeof(superBloque),1,disco);

    fseek(disco,inicio + sizeof(superBloque),SEEK_SET);
    cout << "cac  " << super.s_bm_inode_start<< endl;
    cout << "sea " << ftell(disco) << endl;
    while(ftell(disco) < (inicio + sizeof(superBloque))){
        fread(&journal,sizeof(JOURNAL),1,disco);
            struct tm *tm;
            char fecha[100];
            tm=localtime(&journal.Journal_fecha);
            strftime(fecha,100,"%d/%m/%y %H:%S",tm);
            fprintf(arch, "%d,%d,%s,%s,%s,%s,%d\n",journal.Journal_tipo_operacion,journal.Journal_tipo,journal.Journal_nombre,journal.Journal_contenido,fecha,journal.Journal_propietario,journal.Journal_Permisos);
    }
    cout  << "Journaling graficado con exito " << endl;
    fclose(disco);
    fclose(arch);

}
