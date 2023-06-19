#include "Operando.h"

void reiniciaOperando(TOperando *op){
    op->partereg=0;
    op->posicion=0;
    op->tipo=0;
    op->valor=0;
    op->tamaniocelda=0;
}

void cargaOperando(TOperando *op,char Itipo,unsigned  int Iposicion,char Ipartereg,long int Ivalor,char Itamaniocelda)
{
    op->partereg=Ipartereg;
    op->posicion=Iposicion;
    op->tipo=Itipo;
    op->valor=Ivalor;
    op->tamaniocelda=Itamaniocelda;
}

char getTipo(TOperando op){
    return op.tipo;
}

unsigned int getPosicion(TOperando op){
    return op.posicion;
}

char getPartereg(TOperando op){
    return op.partereg;
}

long int  getValor(TOperando op){
    return op.valor;
}

void setValor(TOperando *op,long int valor){
    op->valor=valor;
}

void Valor1por2(TOperando *op1, TOperando *op2){
    op1->valor=op2->valor; //despues revisar si el valor que se tiene en op2 extiende los 1 si es negativo(esta en longint pero igual no c si estaria bien)
}

void Suma(TOperando *op1, TOperando *op2){
    op1->valor+=op2->valor;
}

void Resta(TOperando *op1, TOperando *op2){
    op1->valor-=op2->valor;
}

void intercambiovalores(TOperando *op1,TOperando *op2){
    long int auxval1;
    auxval1=op1->valor;  //se guarda el valor que tiene el operando 1 en aux
    op1->valor=op2->valor; //hace el intercambio de op2 a op1
    op2->valor=auxval1;  //se hace el intercambio de op1 a op2
}

void multiplica(TOperando *op1,TOperando *op2){
    op1->valor*=op2->valor;
}

void divide(TOperando *op1,TOperando *op2, long int *resto){
    *resto=op1->valor%op2->valor;
    op1->valor/=op2->valor;
}

void corrimientoIzq(TOperando *op1,TOperando *op2){
    op1->valor=op1->valor<<op2->valor;
}

void corrimientoDer(TOperando *op1,TOperando *op2){
    op1->valor=op1->valor>>op2->valor;
}
void Ylogico(TOperando *op1,TOperando *op2){
    op1->valor= op1->valor&op2->valor;
}
void Ologico(TOperando *op1,TOperando *op2){
    op1->valor= op1->valor|op2->valor;
}
void XORlogico(TOperando *op1,TOperando *op2){
    op1->valor= op1->valor^op2->valor;
}

void negacion(TOperando *op){
    op->valor= ~op->valor;
}
