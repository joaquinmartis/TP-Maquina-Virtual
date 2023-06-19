#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MaquinaVirtual.h"


int main(int argc, char *argv[])
{
    int i,cantDiscos;
    char* argumento[3];
    unsigned int auxMem;
    char* nombres_discos[8];

    argumento[0]=NULL;
    argumento[1]=NULL;
    argumento[2]=NULL;
    cantDiscos=0;

    for (i=1;i<argc;i++){
        if (strcmp(argv[i],"-d")==0)
            argumento[2]=argv[i];
        if (strlen(argv[i])>4 && (strcmp(argv[i]+strlen(argv[i])-4,".vmi")==0))
            argumento[0]=argv[i];
        if (argv[i][0]=='m' && argv[i][1]=='=')
            argumento[1]=argv[i];
        if (strlen(argv[i])>4 && (strcmp(argv[i]+strlen(argv[i])-4,".vdd")==0)){
            //Sandra me mata
            while (i<argc){
                nombres_discos[cantDiscos]=argv[i];
                cantDiscos++;
                i++;
            }
        }
    }
    if (argumento[1]==NULL)
        auxMem=0;
    else
        auxMem=atoi(argumento[1]+2);

    run(argv[1],argumento[0],auxMem,nombres_discos,cantDiscos,argumento[2]);
    return 0;

}

