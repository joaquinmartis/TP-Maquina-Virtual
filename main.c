#include <stdio.h>
#include <stdlib.h>
#include "TDATDDS.h"
#include "TDAOPERACIONES.h"
#include "TDAREGISTROS.h"
#include "TDAFUNCIONES.h"




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
int main(int argc,char *argv[])
{
    T_VecMemoriaPriPal MemoriaPrincipal;
    T_TDDS TDDS;
    cargaCodigo(MemoriaPrincipal,&TDDS,&IP);//Ver descripcion en la definicion en la funcion.



    while (CODIGOOPERACION!=STOP o IP>tamano del CS){
        //TOMA INSTRUCCION
        //AVANZA IP
        //ANALIZA INSTRUCCION
        //OBTIENE CODIGO DE OPERACION Y TIPOS DE OPERANDOS
        //LEVANTA DE LA MEMORIA DEL CODIGO VALORES DE LOS OPERANDOS, TAMBIEN SI ESTA EN DATA SEGMENT LEVANTA EL VALOR DESDE AHI
        //EJECUTA OPERACION (si se ejecuta con -d debe mostrar tambien por pantalla lo que hace.
        //(SI FUERA NECESARIO) REASIGNA VALORES OBTENIDOS A LOS REGISTROS O A LOS ESPACIOS EN MEMORIA INDICADOS
        //SIGUIENTE OPERACION
    }
    printf("Programa finalizado");
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
