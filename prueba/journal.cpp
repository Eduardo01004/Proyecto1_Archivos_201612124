#include "journal.h"

Journal::Journal()
{

}

void Journal::GuardarJournaling(int accion,int tipo,int permisos,QString nombre,QString contenido,QString usuario, QString direccion,int inicio_journal){
    JOURNAL journal;
    journal.Journal_tipo_operacion = accion;
    journal.Journal_tipo = tipo;
    strcpy(journal.Journal_nombre,nombre.toStdString().c_str());
    strcpy(journal.Journal_contenido,contenido.toStdString().c_str());
    journal.Journal_fecha = time(NULL);
    strcpy(journal.Journal_propietario,usuario.toStdString().c_str());
    journal.Journal_Permisos = permisos;

    FILE* disco;
    disco=fopen(direccion.toStdString().c_str(),"rb+");
    if (disco != nullptr){
        fseek(disco,inicio_journal,SEEK_SET);
        fwrite(&journal,sizeof(JOURNAL),1,disco);
        inicio_journal += sizeof(JOURNAL);
        cout << "guarda en journaling " << endl;
    }else cout << "disco no existe "<< endl;
    fclose(disco);
}
