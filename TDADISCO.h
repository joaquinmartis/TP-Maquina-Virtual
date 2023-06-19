#ifndef TDADISCO_H_INCLUDED
#define TDADISCO_H_INCLUDED

#define SECTOR 512


typedef struct T_Disco{
    char nombre[50];
    int cilindros,cabezas,sectores;
    long int tamano_sector;
    int ultimoEstado;
}T_Disco;

struct EMV;

int posicionFisicaDisco(T_Disco disco,int cilindro, int cabeza, int sector);
void iniciaDiscos(struct EMV* , char** , int );
void leeDisco(struct EMV*);
int discoVerificaParametrosEntrada(struct EMV);
#endif
