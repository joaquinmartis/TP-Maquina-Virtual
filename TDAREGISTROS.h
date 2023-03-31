#ifndef TDAREGISTROS_H_INCLUDED
#define TDAREGISTROS_H_INCLUDED
#define TAM_REGISTROS 16
#define CS 0
#define DS 1

#define IP 5

#define CC 8
#define AC 9
#define EAX 10
#define EBX 11
#define ECX 12
#define EDX 13
#define EEX 14
#define EFX 15

typedef long int T_Registro;
typedef TRegistro[TAM_REGISTROS] T_Registros;

void setGPReg(char XX,T_Registro,long int valor);
T_Registro getReg(char XX,T_Registro); //XX se refiere a si es EFX, FX, FL o FH. Las posibilidades son L,H,X y E



#endif // TDAREGISTROS_H_INCLUDED
