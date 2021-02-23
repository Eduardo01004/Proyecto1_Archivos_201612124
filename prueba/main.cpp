#include <QCoreApplication>
#include "lexico.h"
#include "sintactico.h"
#include "main.h"
#include "mkdisk.h"
#include "rmdisk.h"
#include "fdisk.h"
#include <math.h>
#include "ejecutar.h"
int global = 1;
Lexico  *lex = new Lexico();
Sintactico *s = new Sintactico();
Mkdisk *o = new Mkdisk();
Rmdisk *r = new Rmdisk();
Fdisk *f = new Fdisk();
Ejecutar *ej = new Ejecutar();
using namespace std;

int main()
{
    while(global){
        char entrada[400];
        char *p=entrada;
        //estados = 0;
        o->estadomkdisk = 0;
        r->estadormdisk = 0;
        f->estadoFdisk = 0;
        ej->estadoexec = 0;
        cout << " Escriba un comando: " << endl;
        scanf(" %400[^\n]", entrada);
        lex->Concatenar(p,"Tokens.txt");
        s->Analisis("Tokens.txt",global);
    }
        return 0;
}

