#include "TDADISCO.h"
#include "MaquinaVirtual.h"
#include <time.h>

void iniciaDiscos(EMV* maquinavirtual,char** nombres_discos,int cantDiscos){//Recibe MV, Vector de String con nombres de los Discos y la cantidad de discos
FILE* discoActual;
int i,iGUID;
long int GUID[4];
long int auxlint;
unsigned char auxbyte;
char fecha[4];
char hora[4];
time_t t;
struct tm tm;
short int auxano;

(*maquinavirtual).cantDiscos=cantDiscos;

for(i=0;i<cantDiscos;i++){//no olvidar cerrar arch
    if (discoActual=fopen(nombres_discos[i],"rb+")){
        strcpy((*maquinavirtual).Discos[i].nombre,nombres_discos[i]);//Valores por default al disco;

        fseek(discoActual,33,SEEK_SET);
        fread(&auxbyte,1,1,discoActual);

        (*maquinavirtual).Discos[i].cilindros=((int)auxbyte)&0xFF;
        fread(&auxbyte,1,1,discoActual);

        (*maquinavirtual).Discos[i].cabezas=((int)auxbyte)&0xFF;
        fread(&auxbyte,1,1,discoActual);

        (*maquinavirtual).Discos[i].sectores=((int)auxbyte)&0xFF;
        fread(&(*maquinavirtual).Discos[i].tamano_sector,4,1,discoActual);
        (*maquinavirtual).Discos[i].tamano_sector=bigYlittleindian((*maquinavirtual).Discos[i].tamano_sector,4);
        fclose(discoActual);
    }
    else{ //capaz no existe, creo el disco
        if (discoActual=fopen(nombres_discos[i],"wb+")){//siempre se crea con los valores por defecto
            for(iGUID=0;iGUID<4;iGUID++)//crea un GUID random
                GUID[iGUID]=rand();
            t = time(NULL);
            tm = *localtime(&t);
            auxano=tm.tm_year + 1900;
            fecha[0]=((auxano&0xFF00)>>8);
            fecha[1]=auxano&0xFF;
            fecha[2]= tm.tm_mon + 1;
            fecha[3]= tm.tm_mday;
            hora[0]=tm.tm_hour;
            hora[1]=tm.tm_min;
            hora[2]=tm.tm_sec;
            hora[3]=rand()%100;//q no rompa las bolas, es imposible sacar las decimas, las inventamos y listo
            //Escribe header
            fwrite("VDD0",4,1,discoActual);

            auxlint= bigYlittleindian(1,4);
            fwrite(&auxlint,4,1,discoActual);

            fwrite(GUID,16,1,discoActual);

            fwrite(fecha,4,1,discoActual);
            fwrite(hora,4,1,discoActual);

            auxbyte=1;
            fwrite(&auxbyte,1,1,discoActual);

            auxbyte=128;
            fwrite(&auxbyte,1,1,discoActual);// Escribe 3 veces 128
            fwrite(&auxbyte,1,1,discoActual);
            fwrite(&auxbyte,1,1,discoActual);

            auxlint=bigYlittleindian(512,4);
            fwrite(&auxlint,4,1,discoActual);
            auxbyte=0;
            fwrite(&auxbyte,1,472,discoActual);

            strcpy((*maquinavirtual).Discos[i].nombre,nombres_discos[i]);//Valores por default del disco;
            (*maquinavirtual).Discos[i].cilindros=128;
            (*maquinavirtual).Discos[i].cabezas=128;
            (*maquinavirtual).Discos[i].sectores=128;
            (*maquinavirtual).Discos[i].tamano_sector=512;

            fclose(discoActual);
        }
    }
}
}

int posicionFisicaMemoria(EMV maquinavirtual,long int puntero){
short int segmento=puntero>>16;

if (segmento<8)
    return ((maquinavirtual.TDDS[segmento].direccionFisica) + (puntero&0xFF));
else
    return -1;
}


int estaEnSegmento(EMV maquinavirtual, long int puntero,int offset){
short int segmento=puntero>>16;
int posFisica,posFisicaMax;
if (segmento<8){
    posFisicaMax=maquinavirtual.TDDS[segmento].direccionFisica+maquinavirtual.TDDS[segmento].tamanio;
    posFisica=maquinavirtual.TDDS[segmento].direccionFisica;
    posFisica+=puntero&0xFFFF;
    posFisica+=offset;
    return (posFisica<=posFisicaMax);
}else
    return 0;
}

int posicionFisicaDisco(T_Disco disco,int cilindro, int cabeza, int sector){
int cantcilindros,cantcabezas,cantsectores;
long int tamanoSector;
cantcilindros=disco.cilindros;//no sirve
cantcabezas=disco.cabezas;
cantsectores=disco.sectores;
tamanoSector=disco.tamano_sector;

return (512+tamanoSector*(cantsectores*(cilindro*cantcabezas + cabeza)+sector));//es una version simplificada de la forma para calcular el sector
}



int discoVerificaParametrosEntrada(EMV maquinavirtual){
int cilindro,cabeza,sector,nroDisco,funcion;

nroDisco=getByte(maquinavirtual.Registros[EDX],'L');
cilindro=getByte(maquinavirtual.Registros[ECX],'H');
cabeza=getByte(maquinavirtual.Registros[ECX],'L');
sector=getByte(maquinavirtual.Registros[EDX],'H');
funcion=getByte(maquinavirtual.Registros[EAX],'H');

if (funcion!=0x0 && funcion!=0x2 && funcion!=0x3 && funcion!=0x8)
    return 0x1;
if (nroDisco>=maquinavirtual.cantDiscos)
    return 0x31;
if  (cilindro>maquinavirtual.Discos[nroDisco].cilindros)//  Cilindro B
    return 0xB;
if  (cabeza>maquinavirtual.Discos[nroDisco].cabezas)//  cabeza C
    return 0xC;
if  (sector>maquinavirtual.Discos[nroDisco].sectores)//  sector D
    return 0xD;

return 0;
}

int topedelSegmento(EMV maquinavirtual,long int registro){
int puntero=registro>>16;
if (puntero<8){
    return maquinavirtual.TDDS[puntero].direccionFisica+maquinavirtual.TDDS[puntero].tamanio;
}
else
    return -1;
}

int posMaximaDisco(T_Disco disco){

return posicionFisicaDisco(disco,disco.cilindros-1,disco.cabezas-1,disco.sectores-1);
}


void escribeDisco(EMV *maquinavirtual){

int cilindro,cabeza,sector;
long int tamanoBytes;
int cantSectores;
int nroDisco;
int primeraCelda;
int posFisicaDisco,posFisicaMemoria,topeSegmento;
int i,aux;
FILE *disco;

cantSectores=getByte((*maquinavirtual).Registros[EAX],'L');//vienen verificados
cilindro=getByte((*maquinavirtual).Registros[ECX],'H');
cabeza=getByte((*maquinavirtual).Registros[ECX],'L');
sector=getByte((*maquinavirtual).Registros[EDX],'H');
nroDisco=getByte((*maquinavirtual).Registros[EDX],'L');

tamanoBytes=(cantSectores*((*maquinavirtual).Discos[nroDisco].tamano_sector));
primeraCelda=(*maquinavirtual).Registros[EBX];
if (estaEnSegmento(*maquinavirtual,primeraCelda,tamanoBytes)){//esto no anda

    if (disco=fopen((*maquinavirtual).Discos[nroDisco].nombre,"rb+")){//lo abro asi de esta forma para que no borre todo si uso wb
        posFisicaDisco=posicionFisicaDisco((*maquinavirtual).Discos[nroDisco],cilindro,cabeza,sector);
        posFisicaMemoria=posicionFisicaMemoria(*maquinavirtual,(*maquinavirtual).Registros[EBX]);
        if (posFisicaMemoria==-1){
            printf("ERROR: el registro recibido no se encuentra inicializado como puntero a un segmento existente");
            exit(1);
        }
        aux=posFisicaDisco+tamanoBytes-posMaximaDisco((*maquinavirtual).Discos[nroDisco]);
        if (aux>0){                                           //Si se supera la posicion maxima se ajusta para que lea lo maximo posible y se devuelve en AL la cantidad de sectores leidos cantSectores a leer - aux/512
            tamanoBytes-=aux;              //lo maximo posible para leer
            seteaByte(&((*maquinavirtual).Registros[EAX]),'H',0xCC);
        }
        else{
            topeSegmento=topedelSegmento(*maquinavirtual,primeraCelda);
            fseek(disco,posFisicaDisco,SEEK_SET);
            for (i=posFisicaMemoria;i<topeSegmento;i++){
                fwrite(&((*maquinavirtual).MemoriaPrincipal[i]),1,1,disco);
            }
            seteaByte(&((*maquinavirtual).Registros[EAX]),'H',0);
        }
    fclose(disco);
    }
    else{
        seteaByte(&((*maquinavirtual).Registros[EAX]),'H',0xFF);
    }
}
else
    seteaByte(&((*maquinavirtual).Registros[EAX]),'H',0xCC);
}

void leeDisco(EMV *maquinavirtual){

int cilindro,cabeza,sector;
long int tamanoBytes;
int cantSectores;
int nroDisco;
int primeraCelda;
int posFisicaDisco,posFisicaMemoria;
int i,aux;
FILE *disco;

cantSectores=getByte((*maquinavirtual).Registros[EAX],'L');//vienen verificados
cilindro=getByte((*maquinavirtual).Registros[ECX],'H');
cabeza=getByte((*maquinavirtual).Registros[ECX],'L');
sector=getByte((*maquinavirtual).Registros[EDX],'H');
nroDisco=getByte((*maquinavirtual).Registros[EDX],'L');

primeraCelda=(*maquinavirtual).Registros[EBX];
tamanoBytes=(cantSectores*((*maquinavirtual).Discos[nroDisco].tamano_sector));
if (estaEnSegmento(*maquinavirtual,primeraCelda,tamanoBytes)){//Verifica ,antes de leer, que lo que se vaya a leer sea menor al Segmento a donde apunta EBX
    if (disco=fopen((*maquinavirtual).Discos[nroDisco].nombre,"rb")){
        posFisicaDisco=posicionFisicaDisco((*maquinavirtual).Discos[nroDisco],cilindro,cabeza,sector);
        fseek(disco,posFisicaDisco,SEEK_SET);
        posFisicaMemoria=posicionFisicaMemoria(*maquinavirtual,(*maquinavirtual).Registros[EBX]);
        if (posFisicaMemoria==-1){
            printf("ERROR: el registro recibido no se encuentra inicializado como puntero a un segmento existente");
            exit(1);
        }
        fseek(disco,0,SEEK_END);
        if (ftell(disco)<posFisicaDisco){
            int cero=0;
            fseek(disco,posFisicaDisco,SEEK_SET);
            fwrite(&cero,1,1,disco);
            seteaByte(&((*maquinavirtual).Registros[EAX]),'L',0);
        }
        else{
            fseek(disco,posFisicaDisco,SEEK_SET);
            aux=posFisicaDisco+tamanoBytes-posMaximaDisco((*maquinavirtual).Discos[nroDisco]);
            if (aux>0){                                         //Si se supera la posicion maxima se ajusta para que lea lo maximo posible y se devuelve en AL la cantidad de sectores leidos cantSectores a leer - aux/512
                tamanoBytes-=aux;              //lo maximo posible para leer
                seteaByte(&((*maquinavirtual).Registros[EAX]),'L',cantSectores-(aux/512));
            }
            for(i=0;i<tamanoBytes;i++)
                fread(&((*maquinavirtual).MemoriaPrincipal[posFisicaMemoria+i]),1,1,disco);
        }
        fclose(disco);
        seteaByte(&((*maquinavirtual).Registros[EAX]),'H',0);
    }
    else{
        seteaByte(&((*maquinavirtual).Registros[EAX]),'H',0xFF);
    }
}
else{
    seteaByte(&((*maquinavirtual).Registros[EAX]),'H',0x4);
}
}


