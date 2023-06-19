#ifndef Operando_H_INCLUDED
#define Operando_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>

typedef struct TOperando{
    char tipo,tamaniocelda;
    unsigned int posicion;
    char partereg; //la posicion se usa solo para registros y memoria y partereg solo para registros
    long int valor;
}TOperando;

void reiniciaOperando(TOperando *op);

void cargaOperando(TOperando *op, char Itipo,unsigned int Iposicion,char Ipartereg,long int Ivalor, char tamaniocelda);

char getTipo(TOperando op);

unsigned int getPosicion(TOperando op);

char getPartereg(TOperando op);

long int  getValor(TOperando op);

void setValor(TOperando *op,long int valor);

void Valor1por2(TOperando *op1, TOperando *op2);
void Suma(TOperando *op1,TOperando *op2);
void Resta(TOperando *op1, TOperando *op2);
void intercambiovalores(TOperando *op1,TOperando *op2);
void multiplica(TOperando *op1,TOperando *op2);
void divide(TOperando *op1,TOperando *op2,long int *resto);
void corrimientoIzq(TOperando *op1,TOperando *op2);
void corrimientoDer(TOperando *op1,TOperando *op2);
void Ylogico(TOperando *op1,TOperando *op2);
void Ologico(TOperando *op1,TOperando *op2);
void XORlogico(TOperando *op1,TOperando *op2);
void negacion(TOperando *op);

#endif
