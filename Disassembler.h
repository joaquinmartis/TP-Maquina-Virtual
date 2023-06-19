#ifndef Disassembler_H_INCLUDED
#define Disassembler_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define CS 0
#define DS 1
#define KS 2
#define ES 3
#define SS 4
#define IP 5
#define SP 6
#define BP 7
#define CC 8
#define AC 9
#define EAX 10
#define EBX 11
#define ECX 12
#define EDX 13
#define EEX 14
#define EFX 15

#define TAMINSTR 5 //4 letras max + \0

typedef struct {
    char tipo,tamaniocelda;
    long int nro;
    long int registro;
    long int codOp;
}TOp;

typedef struct {
    char* reg[16];
    unsigned int posinstr; //Para mostrar al principio
    char instr;
    TOp Op1,Op2;
    char codoperacion[TAMINSTR];
}TElementoD;

void inicializaDisassembler(TElementoD *Dis);
void reseteaDisassembler(TElementoD *D);
void cargaD(TElementoD *Dis,unsigned int iposinstr,char instr,char icodoperacion[]);
void cargaOp(TElementoD *Dis,int nrodeop,char tipo,long int codOp, long int nro,long int registro,char tamaniocelda);
void muestra(TElementoD Dis);

#endif
