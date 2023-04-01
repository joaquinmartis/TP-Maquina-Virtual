#ifndef TDAMEMORIAPRINCIPAL_H_INCLUDED
#define TDAMEMORIAPRINCIPAL_H_INCLUDED

#define MEM 16384 //Tamanio de Memoria


typedef char T_VecMemoriaPriPal[MEM];

void MuestraMemoriaCargada(T_VecMemoriaPriPal MemoriaPrincipal);
void cargaCodigo(T_VecMemoriaPriPal,T_TDDS*);
void almacenaValor(int,int,T_VecMemoriaPriPal,T_Registros*); //Almacena el valor en la memoria o en los registros dependiendo de 'op1'



#endif // TDAMEMORIAPRINCIPAL_H_INCLUDED
