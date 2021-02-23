#include "exec.h"

Exec::Exec()
{

}

void Exec::Ejectuar(char lexema[400], char token[400], int flag,char adress[400]){
    switch (estadoexec) {
    case 0:
       estadoexec = 1;
        break;
    case 1:
        if (strcmp(lexema,"path") == 0){
            estadoexec = 2;
        }else {
            estadoexec = 0;
            flag = 0;
            cout << "Parametro path obligatorio" << endl;
        }
        break;

    case 2:
        estadoexec = 0;
        flag = 0;
        if (strcmp(lexema,"finInstruccion") == 0){
            FILE *archivo;
            char str[1500];
            if(archivo=fopen(adress,"rb+")){
                char* line=NULL;
                size_t len=0;
                ssize_t read;
                while(fgets(str, 1500, archivo) != NULL){
                        estadoexec=0;
                        flag = 0;
                        memset(token, 0, sizeof(token));
                        memset(lexema, 0, sizeof(lexema));
                        memset(adress, 0, sizeof(adress));
                        //memset(fitt, 0, sizeof(fitt));
                    char *apun = str;
                    if(*apun != 10 && *apun != '\0' && str[0] != ' '){
                       lexi->Concatenar(apun,"Tokens2.txt");
                        //sin2->Analisis("Tokens2.txt",0);
                    }

                }
            }else{
                printf("Error no existe la direccion\n");
            }
        }else cout << "NO se reconoce el comando: " <<token<< endl;
    }
}
