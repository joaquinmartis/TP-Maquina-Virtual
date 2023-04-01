#include <stdio.h>
#include <stdlib.h>
#include "TDATDDS.h"
#include "TDAOPERACIONES.h"
#include "TDAREGISTROS.h"
#include "TDAFUNCIONES.h"
#include "TDAMEMORIAPRINCIPAL.h"

// CONSTANTES

//---------

//TIPOS DE DATOS

//--------------------------------------

// FUNCIONES


//--------------------------------------

//DATOS DE COLOR
/*
CHAR 1 BYTE 1 // INSTRUCCION, VALOR REGISTROS EN CS

SHORT INT 2 BYTES //VALOR INMEDIATO EN CS, TAMANO EN TDDS , BASE EN TDDS
INT 2 BYTES

LONG INT 4 BYTES // REGISTROS, VALOR MEMORIA FISICA X 4, 1 SEGMENTO EN TDDS
*/
//---------------------------

// ACTUALIZACIONES
/*HAY QUE VERIFICAR LOS TIPOS DE DATOS  Y LOS TAMANOS DE LOS TIPOS DE DATOS
HAY UN APARTADO DE ERRORES QUE LA MAQUINA VIRTUAL DEBE PODER DETECTAR:
INSTRUCCION INVALIDA listo
DIVISION POR 0 listo
FALLO DEL SEGMENTO (Hay que verificar que no se salga del segmento tanto en el DS como en el CS)
LA MV DEBE CORTAR LA EJECUCION AL DETECTAR ESTOS ERRORES.
//
LA MV DEBE SER CAPAZ DE MOSTRAR POR PANTALLA LA OPERACION QUE SE VA A EJECUTAR(NO IMPLEMENTADO)
*/
//---------------------------


//MAIN
int main(int argc,char *argv[])
{
    T_VecMemoriaPriPal MemoriaPrincipal;
    T_TDDS TDDS;
    T_Registros Registros
    int op1,op2; //en op1 y en op2 va el tamanio que le corresponde (y en consecuencia el tipo de dato que corresponde)
    auxIP T_Registro;
    int valorOp1,valorOp2,operacion;
    int error=0;
    cargaCodigo(MemoriaPrincipal,&TDDS,&IP);//Ver descripcion en la definicion en la funcion.

    while (operacion!=STOP && getReg(IP,Registros)<getDS(TDDS) && error==0){ //Esto es correcto? Puede haber varios DS? TDDS tiene una variable segmento para el segmento actual
        op1=getOp1(MemoriaPrincipal[getReg(IP,Registros)]);      //Vienen de la funcion con el tamaño de bytes
        op2=getOp2(MemoriaPrincipal[getReg(IP,Registros)]);
        operacion=getCodOperacion(MemoriaPrincipal[auxIP]);
        auxIP=getReg(IP,Registros);                              //en auxIP queda la instruccion que se esta ejecutando
        setReg(IP,getReg(IP,Registros)+1,&Registros);           // Avanza el IP
        error+=instruccionInvalida(operacion)+;
        if (error==0){
            if (op1!=0 && op2!=0){                                   //Instruccion con 2 operandos
                valorOp1=getValor(op1,MemoriaPrincipal,Registros);
                setReg(IP,getReg(IP,Registros)+op1,&Registros);
                valorOp2=getValor(op2,MemoriaPrincipal,Registros);
                setReg(IP,getReg(IP,Registros)+op2,&Registros);

                error+=divPorCero(operacion,valorOp2);
                if (error==0){
                    funciones2op[operacion](&valorOp1,&valorOp2);

                    almacenaValor(op1,valorOp1,MemoriaPrincipal,&Registros);
                }
            }
            else{
                if (op2==0 && op1!=0){// no tiene sentido verificar op2, debe ser 0 mientras op1 sea 0, pero lo dejo para claridad //Instruccion con 2 operandos
                    valorOp1=getValor(op1,MemoriaPrincipal,Registros);
                    setReg(IP,getReg(IP,Registros)+op1,&Registros);

                    funciones1op[operacion](&valorOp1,MemoriaPrincipal,&Registros);

                    almacenaValor(op1,valorOp1,MemoriaPrincipal,&Registros);//puede que no cambie el valor pero se lo asigno nuevamente porq paja. Deberia almacenarlo en la funcion
                }
                else
                    if (parametro=='d')
                        printf("STOP");
            }
        }
        error+=falloSegmento();







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




