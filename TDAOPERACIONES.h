#ifndef TDAOPERACIONES_H_INCLUDED
#define TDAOPERACIONES_H_INCLUDED

char getPrimerByte(long int);
char getSegundoByte(long int);
char getTercerByte(long int);
char getCuartoByte(long int);
unsigned int get2BytesMasSignificativos(long int);
unsigned int get2BytesMenosSignificativos(long int);
long int negarBits(long int);
unsigned int getCodOperacion(long int);
unsigned int getOp1(char); //Devuelve con el valor del tamaño de bytes
unsigned int getOp2(char);



unsigned short int esNegativo(int);


#endif // TDAOPERACIONES_H_INCLUDED
