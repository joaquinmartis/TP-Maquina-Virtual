#include "Disassembler.h"


void inicializaDisassembler(TElementoD *Dis){
    Dis->reg[CS]="CS";
    Dis->reg[DS]="DS";
    Dis->reg[KS]="KS";
    Dis->reg[ES]="ES";
    Dis->reg[SS]="SS";
    Dis->reg[IP]="IP";
    Dis->reg[SP]="SP";
    Dis->reg[BP]="BP";
    Dis->reg[CC]="CC";
    Dis->reg[AC]="AC";
    Dis->reg[EAX]="EAX";
    Dis->reg[EBX]="EBX";
    Dis->reg[ECX]="ECX";
    Dis->reg[EDX]="EDX";
    Dis->reg[EEX]="EEX";
    Dis->reg[EFX]="EFX";
}
void reseteaDisassembler(TElementoD *D){

strcpy((*D).codoperacion,"");
(*D).Op1.codOp=0;
(*D).Op1.nro=0;
(*D).Op1.registro=0;
(*D).Op1.tipo=3;
(*D).Op1.tamaniocelda=0;

(*D).Op2.codOp=0;
(*D).Op2.nro=0;
(*D).Op2.registro=0;
(*D).Op2.tipo=3;
(*D).Op2.tamaniocelda=0;
(*D).instr=0;
(*D).posinstr=0;
}

void cargaD(TElementoD *Dis,unsigned int posinstr,char instr,char codoperacion[]){
    (*Dis).posinstr=posinstr;
    (*Dis).instr=instr;
    strcpy((*Dis).codoperacion,codoperacion);
}

void cargaOp(TElementoD *Dis,int nrodeop,char tipo,long int codOp, long int nro,long int registro,char tamaniocelda){
    TOp operando;

    operando.tamaniocelda=tamaniocelda;
    operando.codOp=codOp;
    operando.nro=nro;
    operando.registro=registro;
    operando.tipo=tipo;
    if(nrodeop==1)
        (*Dis).Op1=operando;
    else
        (*Dis).Op2=operando;
}

void imprimeReg(long int registro, long int parteReg,char *registros[],char *muestra){
    if(parteReg==0){
        strcat(muestra,registros[registro]);
    }
    else{
        sprintf(muestra, "%1X", registro); //copia a muestra un string formateado de esta forma ("XXX")
        switch(parteReg){
            case 1: strcat(muestra,"L");
                    break;
            case 2: strcat(muestra,"H");
                    break;
            case 3: strcat(muestra,"X");
                    break;
        }
    }
}

void muestraop( int nroOp,TOp operando,char *registros[]){
    char muestra[15];
    char stnro[6];
    int espacio,i;
    strcpy(muestra,"");
    strcpy(stnro,"");
    switch(operando.tipo){
        case 0: if(operando.tamaniocelda==4)
                    strcpy(muestra,"l");
                else
                    if(operando.tamaniocelda==2)
                       strcpy(muestra,"w");
                    else
                        strcpy(muestra,"b");

                strcat(muestra,"[");
                imprimeReg(operando.registro,0,registros,muestra);
                if(operando.nro!=0){
                    if(operando.nro>0){
                        strcat(muestra,"+");
                    }
                    sprintf(stnro,"%d",operando.nro);
                    strcat(muestra,stnro);
                }
                strcat(muestra,"]");
                break;
        case 1: sprintf(muestra,"%ld",operando.nro);
                break;
        case 2: imprimeReg(operando.registro,operando.nro,registros,muestra);
                break;

    }
    //crea string luego lo muestra dependiendo el operador que debe ir
    if(nroOp==1){
        espacio=10-strlen(muestra);
        for(i=0;i<=espacio;i++)
            printf(" ");
        printf("%s, ",muestra);
    }
    else
        printf("%s",muestra);
}

void muestra(TElementoD Dis){ //Funcion llamada desde MV - run()
    int tamanio,i,espacio=0;

    printf("[%04X] %02X ",Dis.posinstr,((unsigned int)Dis.instr)&0xFF); // CORRECTO casteo y pongo 0 en byte mas significativo para que se muestre bien,sino pone todo FFFFFF

    tamanio= (~Dis.Op1.tipo)&0x03; //el complemento del tipo de operando resulta en el tamanio del mismo (ocupa los 1ros 3 bits)
    for(i=0;i<tamanio;i++){
        printf("%02X ",Dis.Op1.codOp&0xFF);
        Dis.Op1.codOp=Dis.Op1.codOp>>8;
    }

    espacio+=3-tamanio;
    tamanio= (~Dis.Op2.tipo)&0x03;

    for(i=0;i<tamanio;i++){
        printf("%02X ",Dis.Op2.codOp&0xFF);
        Dis.Op2.codOp=Dis.Op2.codOp>>8;
    }
    espacio+=3-tamanio;

    for(i=0;i<=espacio;i++)
        printf("   ");
    printf(" | %s ",Dis.codoperacion);
    if(Dis.Op1.tipo!=0x03){
        if(Dis.Op2.tipo!=0x03){ //2 operandos
            muestraop(1,Dis.Op1,Dis.reg);
            muestraop(2,Dis.Op2,Dis.reg);
        }
        else{ //1 operando
            muestraop(2,Dis.Op1,Dis.reg);
        }
    }
    printf("\n");
}


