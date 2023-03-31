#include <stdio.h>
#include <stdlib.h>
#include "TDATDDS.h"


// CONSTANTES
#define MEM 16384 //Tamanio de Memoria
//---------

//TIPOS DE DATOS
typedef char T_VecMemoriaPriPal[MEM];
//--------------------------------------

// FUNCIONES
void cargaCodigo(T_VecMemoriaPriPal,T_TDDS*,int*);
//--------------------------------------

//MAIN
int main(int argc)//FALTAN ARGUMENTOS
{
    T_VecMemoriaPriPal MemoriaPrincipal;
    unsigned int IP=0; //Instruction Pointer
    T_TDDS TDDS;
    TDDS.
    cargaCodigo(MemoriaPrincipal,&TDDS,&IP);//Ver descripcion en la definicion en la funcion.

    return 0;
}

//-----------------------------------------
//DEFINICION DE FUNCIONES

void cargaCodigo(T_VecMemoriaPriPal MemoriaPrincipal,T_TDDS* TDDS,int* IP){
/*Carga a la MemoriaPrincipal el codigo del programa,
a TDDS(Tabla de descriptores de segmentos) los indicadores de inicio
y fin del codigo, a IP le asigna la posicion en memoria de la primera instruccion a ejecutar siguiente*/
FILE* arch;
unsigned int tamSeg=0;


fopen()
fseek()
fread()
for
    fread()
fclose

//Actualiza TDDS


}
