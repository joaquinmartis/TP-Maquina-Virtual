#ifndef TDATDDS_H_INCLUDED
#define TDATDDS_H_INCLUDED

typedef struct {
    unsigned int direccionFisica;
    unsigned int tamano }CeldaTDDS; //Es una celda del TDDS que tiene la Base y el tamanio del segmento (2 bytes cada una)
typedef struct{
    CeldaTDDS TDDS[8];
    unsigned short int tamTDDS; //Es el tipo de dato Tabla de Descriptores de Segmentos y tiene
}T_TDDS;                          //una variable que indica el ultimo segmento

void inicializaTDDS(T_TDDS*);
void cargaDireccionFisica(unsigned int, unsigned int direccionFisica,T_TDDS*); //Dado una posicion de segmento [0..7] Carga la Base
void cargaTamano(unsigned int, unsigned int tamano,T_TDDS*); //Dado una posicion de segmento [0..7] Carga el Tamanio
unsigned int getSegmento(unsigned int); // dado una posicion de Segmento devuelve todo el segmento (2 bytes)



#endif // TDATDDS_H_INCLUDED
