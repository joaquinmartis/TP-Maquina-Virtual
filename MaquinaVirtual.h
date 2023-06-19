#ifndef MaquinaVirtual_H_INCLUDED
#define MaquinaVirtual_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include "Operando.h"
#include "Disassembler.h"
#include "TDADISCO.h"

#define MEM 16384 //Tamanio de Memoria (16 KiB)
#define Reg 16
#define TabDescSeg 8
#define DISCOS 255

typedef struct {
    short unsigned int direccionFisica; //int= 2 bytes
    short unsigned int tamanio;
}ElementoTDDS; //Es una celda del TDDS que tiene la Base y el tamanio del segmento (2 bytes cada una)


typedef struct EMV{
    unsigned short int tamaniomem;
    unsigned char MemoriaPrincipal[MEM]; //char= 1 byte
    long int Registros[Reg]; // long int= 4 bytes
    ElementoTDDS TDDS[TabDescSeg];
    short int breakpoint;
    short int imagen;
    T_Disco Discos[DISCOS];
    short int cantDiscos;
} EMV;


long int bigYlittleindian(long int lectura,int cantbytes);
void run(char arch[],char *imagenMV,int tamanio,char* nombres_discos[],int cantDiscos,char disassembler[]);
unsigned short int getOffset(long int registro);
void setOffset(long int* registro,unsigned short int offset);
void seteaByte(long int *registro,char letra,int valor);
long int getByte(long int registro,char letra);
#endif  // no borres esto salchicha
