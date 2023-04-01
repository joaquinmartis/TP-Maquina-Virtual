#ifndef TDAOPERACIONES_H_INCLUDED
#define TDAOPERACIONES_H_INCLUDED

char getPrimerByte(long int);
char getSegundoByte(long int);
char getTercerByte(long int);
char getCuartoByte(long int);
unsigned int get2BytesMasSignificativos(long int);
unsigned int get2BytesMenosSignificativos(long int);
unsigned int getCodOperacion2op(long int); //Podria ser una sola funcion que dentro de ella determine el operando dependiendo de los operadores.
unsigned int getCodOperacion1op(long int);
unsigned int getCodOperacion0op(long int);
//unsigned int getCodOperacion(long int);
unsigned int getOp1(long int);
unsigned int getOp2(long int);


unsigned short int esNegativo(int*);


#endif // TDAOPERACIONES_H_INCLUDED
