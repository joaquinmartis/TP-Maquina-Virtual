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



void inicializaRegistros(T_Registros*);//Puede que no sea necesaria esta funcion
//GPReg = General Purpose Register
void setGPReg(char XX,T_Registro,long int valor,T_Registros*);
T_Registro getGPReg(char XX,T_Registro,T_Registros); //XX se refiere a si es EFX, FX, FL o FH. Las posibilidades son L,H,X y E o como esta en la documentacion 00 01 10 11

void setReg(T_Registro,long int valor,T_Registros*); //la diferencia entre setReg y setGPReg es que en GPReg se le debe pasar por parametro a que byte o bytes se quiere escribir el dato
                                                     // en cambio con setReg se copia todo el condtenido de valor al registro
T_Registro getReg(T_Registro,T_Registros);

#endif // TDAREGISTROS_H_INCLUDED
