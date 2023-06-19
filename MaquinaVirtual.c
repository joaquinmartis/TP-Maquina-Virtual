#include "MaquinaVirtual.h"


typedef void t_fun(TOperando *,TOperando *, EMV *);

long int bigYlittleindian(long int lectura,int cantbytes){
    long int valor=0;
    int i;
    for(i=0;i<cantbytes;i++){
        valor=valor<<8;
        valor=valor|(lectura&0xFF);
        lectura=lectura>>8;
    }
    return valor;
}

long int BytesMasSignificativos(long int entrada){
    entrada= (entrada>>16)&0xFFFF;
    return entrada;
}

unsigned short int getOffset(long int registro){
    unsigned short int rta=registro&0xFFFF;
    return rta;
}
void setOffset(long int *registro,unsigned short int offset){ //entra en little endian (el byte menos sig al principio)
long int aux;
    aux=(*registro&0xFFFF0000)|offset;
    *registro=aux;
}

void seteaByte(long int *registro,char letra,int valor){

switch (letra){
    case 'H':
        *registro=(*registro&0xFFFF00FF)|(valor<<8)&0xFF00;
        break;
    case 'L':
        *registro=((*registro&0xFFFFFF00)|(valor)&0xFF);
        break;
}
}

long int getByte(long int registro,char letra){
long int res;

switch (letra){
    case 'H':
        res=((registro&0xFF00)<<16)>>24;
        break;
    case 'L':
        res=((registro&0xFF)<<24)>>24;
        break;
}
return res;
}

void iniciaMV(FILE *programa,EMV *maquinavirtual, int *cond,unsigned int tamanio){ //en caso de no poder inicializar la MV se devuelve 0 y corta ejecucion
    unsigned short int tamanioSegmento,i,j,cantSeg=0;
    unsigned int acumTamReg=0;
    long int aux;
    char version, identificador[5];
    *cond=0;
    fread(identificador,sizeof(identificador),1,programa);
    fread(&version,sizeof(version),1,programa);
    if(tamanio<=0)
        maquinavirtual->tamaniomem=MEM;
    else
        maquinavirtual->tamaniomem=tamanio*1024;//la memoria entra n kib
    if(strcmp(identificador,"VMX23")){
       if(maquinavirtual->tamaniomem<=MEM){ //si la cabecera es correcta
        if(version==1){
            fread(&tamanioSegmento,sizeof(tamanioSegmento),1,programa);//Lee tamanio en bigendian (guarda el binario al reves)
            tamanioSegmento=bigYlittleindian(tamanioSegmento,2); //tamanio a little endian
            if(tamanioSegmento<=maquinavirtual->tamaniomem){
                *cond=1;
                maquinavirtual->TDDS[1].tamanio= maquinavirtual->tamaniomem-tamanioSegmento;//trabaja con los nros en littlendian y desp guarda en bigendian
                maquinavirtual->TDDS[0].direccionFisica=0;
                maquinavirtual->TDDS[0].tamanio=tamanioSegmento;
                maquinavirtual->TDDS[1].direccionFisica=tamanioSegmento;
                maquinavirtual->Registros[CS]=0;
                maquinavirtual->Registros[DS]=0x00010000;
                maquinavirtual->Registros[IP]=0;
                //XXXXXXXmaquinavirtual->Registros[CC]=0;
                for(i=0;i<tamanioSegmento;i++)
                    fread(&maquinavirtual->MemoriaPrincipal[i],sizeof(char),1,programa); //carga las instrucciones en el Code Segment de la memoria
            }
        }
        else
            if(version==2 || version==3){
                for(i=0;i<5;i++){
                    if(i==1) // para acceder al registro 2 al cargar el KS
                        j=i+1;
                    else
                        if(i==2) // para acceder al registro 1 al cargar el DS
                            j=i-1;
                        else
                            j=i;
                    fread(&tamanioSegmento,sizeof(tamanioSegmento),1,programa);//Lee tamanio en bigendian (guarda el binario al reves)
                    tamanioSegmento=bigYlittleindian(tamanioSegmento,2); //tamanio a little endian
                    if(tamanioSegmento!=0){
                        maquinavirtual->TDDS[cantSeg].direccionFisica=acumTamReg;
                        acumTamReg+=tamanioSegmento;
                        maquinavirtual->TDDS[cantSeg].tamanio=tamanioSegmento;
                        aux=cantSeg;
                        aux=aux<<16;
                        maquinavirtual->Registros[j]=aux; //inicializa el registro asociado con el segmento
                        if(i==4){
                          maquinavirtual->Registros[SP]=aux|tamanioSegmento; //inicializa sp=ptero a sp+offset(tamanio del segmento SS)
                          //BP no requiere de inicializacion
                        }
                        cantSeg+=1;
                    }
                    else{
                        maquinavirtual->Registros[j]=-1;
                    }
                }
                while(cantSeg<8){
                    maquinavirtual->TDDS[cantSeg].direccionFisica=-1;
                    cantSeg++;
                }
                if(acumTamReg<=maquinavirtual->tamaniomem){
                    *cond=1;
                    maquinavirtual->Registros[IP]=0;
                    if(maquinavirtual->Registros[0]!=-1){
                        aux=BytesMasSignificativos(maquinavirtual->Registros[0]);
                        maquinavirtual->Registros[IP]=maquinavirtual->TDDS[aux].direccionFisica;//permite localizar el IP a la direccion del CS
                        for(i=maquinavirtual->TDDS[aux].direccionFisica;i<maquinavirtual->TDDS[aux].tamanio;i++)
                            fread(&maquinavirtual->MemoriaPrincipal[i],sizeof(char),1,programa); //carga las instrucciones en el Code Segment de la memoria
                    }
                    if(maquinavirtual->Registros[2]!=-1){
                        aux=BytesMasSignificativos(maquinavirtual->Registros[2]);
                        for(i=maquinavirtual->TDDS[aux].direccionFisica;i<maquinavirtual->TDDS[aux].direccionFisica+maquinavirtual->TDDS[aux].tamanio;i++)
                                fread(&maquinavirtual->MemoriaPrincipal[i],sizeof(char),1,programa); //carga las instrucciones en el Code Segment de la memoria
                    }
                }
                else
                    printf("ERROR: Memoria insuficiente, la memoria principal no cuenta con espacio suficiente para alojar todos los segmentos del proceso");
            }
            else
                printf("ERROR: version ingresada desconocida");
       }
       else
         printf("Error: la cantidad de memoria que se intenta asignar supera la capacidad fisica de la RAM");
    }
    else
        printf("Error en el identificador");
}

long int consultamemoria(unsigned int posicion,char cantceldas,EMV *maquinavirtual){
    unsigned int i;
    long int valor=0;
    for(i=0;i<cantceldas;i++){
       valor=(valor<<8) | (maquinavirtual->MemoriaPrincipal[posicion+i]&0xFF);
    }
    valor=(valor<<((4-cantceldas)*8))>>((4-cantceldas)*8);//Expande signo
    return valor; //retorna el valor en little endian
}

void leeOperando(char tipo,TOperando *op,EMV *maquinavirtual,long int *codop,int *nro,unsigned int *registro,char *tamaniocelda, int disassembler){
    int i,offset;
    unsigned int indiceTDDS;
    char celda, partereg=0,tamanio;
    unsigned char codReg;
    short unsigned int posicion=0;
    long int valor=0,lectura=0;
    tamanio= (~tipo)&0x03;//el complemento del tipo de operando resulta en el tamanio del mismo (ocupa los 1ros 3 bits)
    for(i=0;i<tamanio;i++){
        celda= maquinavirtual->MemoriaPrincipal[getOffset((*maquinavirtual).Registros[IP])];
        lectura= (lectura<<8) | (celda&0xFF);
        setOffset(&(maquinavirtual->Registros[IP]),getOffset((*maquinavirtual).Registros[IP])+1);
    }//LECTURA ESTA EN LITTLE ENDIAN
    *codop=bigYlittleindian(lectura,tamanio);

    switch(tipo){

        case 0: codReg=(lectura>>16)&0x0F;
                *registro=codReg;
                *tamaniocelda=4-((lectura>>22)&0x0F);//se utiliza el parteReg para almacenar el tamanio de bytes con el que se opera (CAMBIAR EL NOMBRE)
                if(*tamaniocelda<=1&&*tamaniocelda>4){
                        printf("ERROR: codigo de instruccion inexistente ");
                            exit(1);
                }
                else{
                    if(*tamaniocelda==3)
                    *tamaniocelda=4;
                    offset=((lectura&0xFFFF)<<16)>>16;
                    *nro=offset;
                    if(maquinavirtual->Registros[codReg]!=-1){
                        indiceTDDS=BytesMasSignificativos(maquinavirtual->Registros[codReg]);
                        if (disassembler!=NULL){
                            indiceTDDS=0;
                            codReg=0;
                        }
                         if( (indiceTDDS<8 && maquinavirtual->TDDS[indiceTDDS].direccionFisica!=-1) || disassembler){ //verifica que el registro sea un puntero y apunte a una zona valida de memoria (data segment)
                            posicion=maquinavirtual->TDDS[indiceTDDS].direccionFisica+(maquinavirtual->Registros[codReg]&0xFFFF)+offset;
                            if(((posicion>=maquinavirtual->TDDS[indiceTDDS].direccionFisica)  && ((posicion+*tamaniocelda)<= (maquinavirtual->TDDS[indiceTDDS].direccionFisica+maquinavirtual->TDDS[indiceTDDS].tamanio)) )|| disassembler){
                                    if (disassembler!=NULL)
                                        posicion=0;
                                    valor=consultamemoria(posicion,*tamaniocelda,maquinavirtual);
                            }
                            else{
                                printf("ERROR: registro seleccionado para acceder a memoria no es valido");
                                exit(1);
                            }
                         }
                        else{
                            printf("ERROR: el registro recibido como opernado no es un puntero a segmento valido");
                            exit(1);
                        }
                    }
                    else{
                            printf("ERROR: intento de operacion en un segmento sin memoria");
                            exit(1);
                    }
                }
                break;
        case 1: valor= lectura;
                valor=(valor<<16)>>16;
                *nro=valor;
                break;
        case 2: partereg=(lectura>>4)& 0x03;// como lectura ya esta en bigendian puede trabajarse en operaciones con constantes
                posicion=lectura & 0x0F;
                *registro=posicion;
                *nro=partereg;
                valor=maquinavirtual->Registros[posicion];
                switch(partereg){
                    case 1: valor=valor & 0xFF;
                            valor=(valor<<24)>>24;
                            break;
                    case 2: valor= (valor>>8)& 0xFF;
                            valor=(valor<<24)>>24;
                            break;
                    case 3: valor= valor & 0xFFFF;
                            valor=(valor<<16)>>16;
                            break;
                }
                if(posicion<10 && partereg!=0){ //no permite acceder x ej a los bits mas significativos de DS x ej
                    printf("ERROR: se intenta acceder a una parte de registro inaccesible");
                }
                break;
        default: printf("ERROR: cod de operando incorrecto");
                 exit(1);
                 break;
    }

    cargaOperando(op, tipo, posicion,partereg,valor,*tamaniocelda);

}

void descifrainstrc(TOperando *op1,TOperando *op2,unsigned char *codoperacion, char instr, EMV *maquinavirtual,TElementoD *D){
    char tipo1,tipo2,tamaniocelda=0;
    long int lectura;
    unsigned int registro;
    int nro;
    tipo1=(instr >>6)& 0x03;
    tipo2=(instr>>4)& 0x03;
    if(tipo1!=0x03){
        if(tipo2!=0x03){ //2 operandos
            *codoperacion=(instr & 0x0F);
            if(D==NULL){
                leeOperando(tipo1,op1,maquinavirtual,&lectura,&nro,&registro,&tamaniocelda,0);
                leeOperando(tipo2,op2,maquinavirtual,&lectura,&nro,&registro,&tamaniocelda,0);
            }
            else{
                leeOperando(tipo1,op1,maquinavirtual,&lectura,&nro,&registro,&tamaniocelda,1);
                cargaOp(D,1,tipo1,lectura, nro,registro,tamaniocelda);
                leeOperando(tipo2,op2,maquinavirtual,&lectura,&nro,&registro,&tamaniocelda,1);
                cargaOp(D,2,tipo2,lectura, nro,registro,tamaniocelda);
            }

        }
        else{ //1 operando
            *codoperacion=(instr & 0x3F);
            if(D==NULL)
                leeOperando(tipo1,op1,maquinavirtual,&lectura,&nro,&registro,&tamaniocelda,0);
            else{
                leeOperando(tipo1,op1,maquinavirtual,&lectura,&nro,&registro,&tamaniocelda,1);
                cargaOp(D,1,tipo1,lectura, nro,registro,tamaniocelda);
            }
        }
    }
    else{   // 0 operandos (el caso de que el operando 1 sea 11 y el operando 2 no lo sea es un error no contemplado en esta instancia, pero luego al verificar el nro de instruccion este error se captura)
        *codoperacion=instr;
    }
}

void actualizaMemoria(unsigned int pos,long int valor,int cantceldas,EMV *maquinavirtual){
   unsigned char celda;
   int i;
   for(i=1;i<=cantceldas;i++){ //no necesita verificar una pos de memoria valida, puesto que ya se verifico al cargar el operando
        celda= valor & 0xFF;
        valor= valor>>8;
        maquinavirtual->MemoriaPrincipal[pos+cantceldas-i]=celda;

    }
}


void actualizaEstructura(TOperando op,EMV  *maquinavirtual ){//despues de operar con el tda operando, se debe actualizar las estructuras de la VM
    unsigned int pos;
    long int valor,aux;
    char partereg;
    pos=getPosicion(op);
    valor=getValor(op);
    switch(getTipo(op)){
        case 0: actualizaMemoria(pos,valor,op.tamaniocelda,maquinavirtual);//la escritura en memoria es de 4 bytes, excepto si es de teclado(sujeta a registro)
                break;
        case 1: printf("ERROR de instruccion: intenta guardar informacion en un inmediato");
                exit(1);
                break;
        case 2: partereg=getPartereg(op);
                switch(partereg){
                    case 0: maquinavirtual->Registros[pos]=valor;
                            break;
                    case 1: aux=(maquinavirtual->Registros[pos] & 0xFFFFFF00)|(valor&0xFF);//trabaja en little endian
                            maquinavirtual->Registros[pos]=aux;
                            break;
                    case 2: aux= (maquinavirtual->Registros[pos]& 0xFFFF00FF)|((valor<<8)&0xFF00);//trabaja en little endian
                            maquinavirtual->Registros[pos]=aux;
                            break;
                    case 3: aux= (maquinavirtual->Registros[pos] &0xFFFF0000)|(valor& 0xFFFF);
                            maquinavirtual->Registros[pos]=aux;
                            break;
                }
                break;
    }
}

void modificaCC(long int valor, EMV *maquinavirtual){
maquinavirtual->Registros[CC]=maquinavirtual->Registros[CC]&0x3FFFFFFF; //se pasa la cte a bigendian y se ponen los bits mas significativos en 0

if(valor==0){
    maquinavirtual->Registros[CC]=maquinavirtual->Registros[CC]|0X40000000; //se pone el 2do bit mas significativo en 0
}
else
    if(valor<0){
        maquinavirtual->Registros[CC]=maquinavirtual->Registros[CC]|0x80000000; //se pone el 1er bit mas significativo en 0
    }
}


void  MOV(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
      Valor1por2(op1,op2);
      actualizaEstructura(*op1,maquinavirtual);
}

void  ADD(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
      Suma(op1,op2);
      modificaCC(getValor(*op1),maquinavirtual);
      actualizaEstructura(*op1,maquinavirtual);
}

void  SUB(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    Resta(op1,op2);
    modificaCC(getValor(*op1),maquinavirtual);
    actualizaEstructura(*op1,maquinavirtual);
}

void  SWAP(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    intercambiovalores(op1,op2);
    actualizaEstructura(*op1,maquinavirtual);
    actualizaEstructura(*op2,maquinavirtual);
}

void  MUL(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    multiplica(op1,op2);
    modificaCC(getValor(*op1),maquinavirtual);
    actualizaEstructura(*op1,maquinavirtual);
}

void  DIV(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    long int resto;
    if(getValor(*op2)!=0){
        divide(op1,op2,&resto);
        modificaCC(getValor(*op1),maquinavirtual);
        maquinavirtual->Registros[AC]=resto;
        actualizaEstructura(*op1,maquinavirtual);
    }
    else{
        printf("ERROR: division por 0");
        exit(2);
    }
}

void  CMP(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    Resta(op1,op2);
    modificaCC(getValor(*op1),maquinavirtual);
}

void  SHL(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    corrimientoIzq(op1,op2);
    modificaCC(getValor(*op1),maquinavirtual);
    actualizaEstructura(*op1,maquinavirtual);
}

void  SHR(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    corrimientoDer(op1,op2);
    modificaCC(getValor(*op1),maquinavirtual);
    actualizaEstructura(*op1,maquinavirtual);
}

void  AND(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    Ylogico(op1,op2);
    modificaCC(getValor(*op1),maquinavirtual);
    actualizaEstructura(*op1,maquinavirtual);
}

void  OR(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    Ologico(op1,op2);
    modificaCC(getValor(*op1),maquinavirtual);
    actualizaEstructura(*op1,maquinavirtual);
}

void  XOR(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    XORlogico(op1,op2);
    modificaCC(getValor(*op1),maquinavirtual);
    actualizaEstructura(*op1,maquinavirtual);
}

void MuestraSegunTipo(char tipo,long int lectura,int celdas){
    char* VecSt[]={"#%d ","%c"," @%o "," %%%X "};// DECIMAL CARACTER OCTAL HEXA
    int ichar;
    long int auxlectura;
    char auxchar;

    lectura=lectura<<8*(4-celdas);
    lectura=lectura>>8*(4-celdas);

    if  (tipo&0x2){//char
        auxlectura=lectura;
        for (ichar=0;ichar<celdas;ichar++){
            auxchar=(auxlectura&0xFF000000)>>24;
            if (isprint(auxchar)==0){
                auxchar='.';
            }
            printf(VecSt[1],auxchar);
            auxlectura=auxlectura<<8;
        }
    }
    if  (tipo&0x8){ //hexa
        printf(VecSt[3],lectura);
    }
    if  (tipo&0x4){ //octal
        printf(VecSt[2],lectura);
    }
    if  (tipo&0x1){ //dec
        printf(VecSt[0],lectura);
    }
}




void  SYS(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    int cant,celdas,i,systipo;
    char lecturaString[256],aux;
    long int lectura,posTdds,dirSeg;
    unsigned int posicion,tipo;
    short int operacion,rta;
    int AH,estado,nroDisco;


    systipo=getValor(*op1);
    posTdds=0;
    if(systipo>=1 && systipo<=4){ //carga el tipo cantidadceldas y la postdds para trabajar con el sys 1 y 2
        if(systipo==1||systipo==2){
            tipo=maquinavirtual->Registros[EAX]&0xFF;
            cant=maquinavirtual->Registros[ECX]&0xFF;
            celdas=((maquinavirtual->Registros[ECX])>>8)&0xFF;
            if(celdas>4 && celdas!=3){ //la cantidad de celdas debe ser menor a 4
                printf("ERROR: cantidad de celdas incorrectas para sys");
                exit(1);
            }
        }
        else{
            if(systipo==3||systipo==4){ //carga la cantidad y la pos tdds para trabajar con el sys 1 y 2
                cant=((maquinavirtual->Registros[ECX]&0xFFFF)<<16)>>16;
            }
        }
        posTdds=BytesMasSignificativos(maquinavirtual->Registros[EDX]);
        if((posTdds<8 && maquinavirtual->TDDS[posTdds].direccionFisica!=-1)){
            posicion=maquinavirtual->TDDS[posTdds].direccionFisica+(maquinavirtual->Registros[EDX]&0xFFFF);
        }
        else{
            printf("ERROR: el registro recibido no se encuentra inicializado como puntero a un segmento existente");
            exit(1);
        }
    }

       switch(systipo){
            case 1: if((posicion>=maquinavirtual->TDDS[posTdds].direccionFisica)&&(posicion+cant*celdas<=maquinavirtual->TDDS[posTdds].tamanio+maquinavirtual->TDDS[posTdds].direccionFisica)){
                        for(i=0;i<cant;i++){
                            printf("[%04X]",posicion-maquinavirtual->TDDS[BytesMasSignificativos(maquinavirtual->Registros[EDX])].direccionFisica);
                            switch(tipo){
                                case 1: scanf("%ld",&lectura);
                                        break;
                                case 2:scanf("%c",&lectura);
                                    break;
                                case 4:scanf("%lo",&lectura);
                                    break;
                                case 8:scanf("%lx",&lectura);
                                    break;
                                default:{   printf("ERROR: tipo de lectura incorrecta para sys");
                                    exit(1);
                                }
                            }
                            actualizaMemoria(posicion,lectura,celdas,maquinavirtual);//en vez de lectura seria valor
                            posicion+=celdas;
                        }
                    }
                    else{
                        printf("ERROR: acceso fuera de segmento");
                        exit(1);
                    }
                    break;
            case 2: if((posicion>=maquinavirtual->TDDS[posTdds].direccionFisica)&& (posicion+cant*celdas<=maquinavirtual->TDDS[posTdds].tamanio+maquinavirtual->TDDS[posTdds].direccionFisica)){
                        for(i=0;i<cant;i++){
                            lectura=consultamemoria(posicion,celdas,maquinavirtual);
                            printf("[%04X]",posicion-maquinavirtual->TDDS[BytesMasSignificativos(maquinavirtual->Registros[EDX])].direccionFisica);
                            posicion+=celdas;
                            MuestraSegunTipo(tipo,lectura,celdas);
                            printf("\n");
                        }
                    }
                    else{
                        printf("ERROR: acceso fuera de segmento");
                        exit(1);
                    }
                    break;
            case 3: gets(lecturaString);//agregar el if del sys 1 y 2
                    i=0;
                    while((cant==-1 || i<=cant )&& lecturaString[i]!='\0'){
                        if((posicion+i>=maquinavirtual->TDDS[posTdds].direccionFisica)  && (posicion+i<=maquinavirtual->TDDS[posTdds].tamanio+maquinavirtual->TDDS[posTdds].direccionFisica)){ //verifica que la lectura se mantenga dentro del segmento
                            actualizaMemoria(posicion+i,lecturaString[i],1,maquinavirtual);//en vez de lectura seria valor
                            i++;
                        }
                        else{
                            printf("ERROR: acceso fuera de segmento");
                            exit(1);
                        }
                    }
                    break;
            case 4: i=0; //agregar el if del sys 1 y 2
                    aux=consultamemoria(posicion,1,maquinavirtual);
                    while((i<=cant || cant==-1)&& aux!='\0'){
                        if((posicion+i>=maquinavirtual->TDDS[posTdds].direccionFisica)  && (posicion+i<=maquinavirtual->TDDS[posTdds].tamanio+maquinavirtual->TDDS[posTdds].direccionFisica)){ //verifica que la lectura se mantenga dentro del segmentolecturaString[i]=aux;
                            lecturaString[i]=aux;
                            i++;
                            aux=consultamemoria(posicion+i,1,maquinavirtual);
                        }
                        else{
                            printf("ERROR: acesso fuera de segmento");
                            exit(1);
                        }
                    }
                    lecturaString[i]='\0';
                    printf("%s",lecturaString);
                    break;
            case 7: system("cls");
                    break;

            case 0xD://disco
                    AH=getByte((*maquinavirtual).Registros[EAX],'H');
                    nroDisco=getByte((*maquinavirtual).Registros[EDX],'L');

                    if (AH==0x08){//Obtener parametros
                        if (nroDisco<(*maquinavirtual).cantDiscos){
                            T_Disco disco=(*maquinavirtual).Discos[nroDisco];

                            seteaByte(&((*maquinavirtual).Registros[ECX]),'H',disco.cilindros);
                            seteaByte(&((*maquinavirtual).Registros[ECX]),'L',disco.cabezas);
                            seteaByte(&((*maquinavirtual).Registros[EDX]),'H',disco.sectores);
                            seteaByte(&((*maquinavirtual).Registros[EAX]),'H',0x0);
                        }
                        else{//Disco inexistente
                            seteaByte(&((*maquinavirtual).Registros[EAX]),'H',0x31);
                        }
                    }
                    else{
                        estado=discoVerificaParametrosEntrada(*maquinavirtual);

                        if (estado!=0){

                            seteaByte(&((*maquinavirtual).Registros[EAX]),'H',estado);
                        }
                        else{
                            switch (AH){

                                case 0:
                                    seteaByte(&(*maquinavirtual).Registros[EAX],'H',(*maquinavirtual).Discos[nroDisco].ultimoEstado);// Esto aca esta bien. Leer especificacion p.5
                                    break;
                                case 2:
                                    leeDisco(maquinavirtual);
                                    break;
                                case 3:
                                    escribeDisco(maquinavirtual);
                                    break;
                                }
                        }
                    }
                    break;

            case 0xE:   operacion=maquinavirtual->Registros[EAX]&0xFFFF;
                        switch (operacion){
                            case 0: posTdds=BytesMasSignificativos(maquinavirtual->Registros[EBX]);
                                    if((posTdds<8 && maquinavirtual->TDDS[posTdds].direccionFisica!=-1)){
                                       maquinavirtual->Registros[ECX]= maquinavirtual->Registros[ECX]& 0xFFFF0000;
                                       maquinavirtual->Registros[ECX]=maquinavirtual->Registros[ECX] | maquinavirtual->TDDS[posTdds].tamanio;
                                       rta=0;
                                    }
                                    else{
                                        maquinavirtual->Registros[ECX]= maquinavirtual->Registros[ECX]& 0xFFFF0000;
                                        rta=0x31;
                                    }
                            break;
                            case 1: cant=maquinavirtual->Registros[ECX]&0xFFFF; //no se propaga el signo, el tama�o en del segmento es extrictamente positivo
                                    i=0;
                                    while(i<8 && maquinavirtual->TDDS[i].direccionFisica!=-1){
                                        i++;
                                    }
                                    if(i==8){// no hay espacio en tdds
                                        maquinavirtual->Registros[EBX]=-1;
                                        rta=0xFFFF;
                                    }
                                    else{ //si hay espacio en tdds
                                        posicion=maquinavirtual->TDDS[i-1].tamanio+maquinavirtual->TDDS[i-1].direccionFisica;
                                        if(posicion+cant<=MEM){// comprueba si la memoria es capaz de contener el segmento
                                            rta=0;
                                            maquinavirtual->TDDS[i].direccionFisica=posicion;
                                            maquinavirtual->TDDS[i].tamanio=cant;
                                            maquinavirtual->Registros[EBX]=i;
                                            maquinavirtual->Registros[EBX]=maquinavirtual->Registros[EBX]<<16;
                                        }
                                        else{ //no hay memoria suficiente
                                            maquinavirtual->Registros[EBX]=-1;
                                            rta=0x00CC;
                                        }
                                    }
                                    break;
                            default:    rta=0x0001;
                                        break;
                        }
                        maquinavirtual->Registros[EAX]= maquinavirtual->Registros[EAX]& 0xFFFF0000;
                        maquinavirtual->Registros[EAX]=maquinavirtual->Registros[EAX] | rta;
                        break;
            case 0xF: if (maquinavirtual->imagen==1)//Si se ingreso un archivo imagen
                         maquinavirtual->breakpoint=1; //Se activa la flag de la MV de breakpoints
                        break;

            default: printf("ERROR: operando no valido para instruccion SYS");
                     break;
        }
}


void  JMP(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    setOffset(&(maquinavirtual->Registros[IP]),getValor(*op1));
}


void  JZ(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    if((maquinavirtual->Registros[CC]>>30)&0x1) //si el 2do bit mas significativo es 1 es Verdadero
        setOffset(&(maquinavirtual->Registros[IP]),getValor(*op1));
}

void  JP(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    if (!((maquinavirtual->Registros[CC]>>30)&0x3)) //si los 2 bits mas significativos son 0 es Verdadero
        setOffset(&(maquinavirtual->Registros[IP]),getValor(*op1));
}

void  JN(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    if(((maquinavirtual->Registros[CC]>>30)&0x3)==2) //si el bit mas significativo es 1 es Verdadero
        setOffset(&(maquinavirtual->Registros[IP]),getValor(*op1));
}

void  JNZ(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    if  ((!((maquinavirtual->Registros[CC]>>30)&0x3)) || ((((maquinavirtual->Registros[CC]>>30)&0x3)==2) )) // verdadero si los 2 bits mas significativos son 0 o bit mas significativo es 1
        setOffset(&(maquinavirtual->Registros[IP]),getValor(*op1));
}

void  JNP(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    if ( ((maquinavirtual->Registros[CC]>>30)&0x1) || ((((maquinavirtual->Registros[CC]>>30)&0x3)==2) )) //verdadero si el 1er o el 2do bit significativo son 1
        setOffset(&(maquinavirtual->Registros[IP]),getValor(*op1));
}

void  JNN(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    if ( ((maquinavirtual->Registros[CC]>>30)&0x1) || (!((maquinavirtual->Registros[CC]>>30)&0x3)) ) //verdadero si el 2do bit mas significativo es 1 o los 2 primeros bits son 0
        setOffset(&(maquinavirtual->Registros[IP]),getValor(*op1));
}

void  LDH(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    maquinavirtual->Registros[AC]=maquinavirtual->Registros[AC]& 0xFFFF; //pone en 0 los 2 bytes mas significativos del reg AC
    maquinavirtual->Registros[AC]=maquinavirtual->Registros[AC]|(getValor(*op1)<<16);//carga los 2 bytes menos significativos del operando en los 2 mas significativos de AC
}


void  LDL(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    maquinavirtual->Registros[AC]=maquinavirtual->Registros[AC]& 0xFFFF0000; //pone en 0 los 2 bytes menos significativos del reg AC
    maquinavirtual->Registros[AC]=maquinavirtual->Registros[AC]| (getValor(*op1)&0xFFFF); //carga los 2 bytes menos significativos del operando en los 2 menos significativos de AC
}

void  RND(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    long int aux;
    aux= rand() % (getValor(*op1)+1);
    maquinavirtual->Registros[AC]=aux;
}

void  NOT(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    negacion(op1);
    modificaCC(getValor(*op1),maquinavirtual);
    actualizaEstructura(*op1,maquinavirtual);
}

void  PUSH(TOperando *op1,TOperando *op2, EMV *maquinavirtual){//Para almacenar un valor en la pila debe ser en bigendian
      int indiceTDDS,posicion;
      if(maquinavirtual->Registros[SS]!=-1){
            indiceTDDS=(maquinavirtual->Registros[SP]>>16)&0xFFFF;
            posicion=maquinavirtual->TDDS[indiceTDDS].direccionFisica+(maquinavirtual->Registros[SP]&0xFFFF)-4;
            if (posicion>=maquinavirtual->TDDS[indiceTDDS].direccionFisica){ //verifica que la posicion del Sp no se vaya del SS(inferior)
                actualizaMemoria(posicion,op1->valor,4,maquinavirtual);
                setOffset(&(maquinavirtual->Registros[SP]),getOffset(maquinavirtual->Registros[SP])-4);

            }
            else{
                printf("ERROR : STACK OVERFLOW");
                exit(1);
            }
      }
      else{
        printf("ERROR: intento de operacion en un segmento sin memoria");
        exit(1);
      }
}

void  POP(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    int indiceTDDS,posicion,posaux;
    long int lectura;
    if(maquinavirtual->Registros[SS]!=-1){
            indiceTDDS=(maquinavirtual->Registros[SP]>>16)&0xFFFF;
            posicion=maquinavirtual->TDDS[indiceTDDS].direccionFisica+(maquinavirtual->Registros[SP]&0xFFFF);
            posaux=posicion+4;//para comprobar que la posicion luego de quitar 4 bytes se mantenga dentro del SP o la deje vacia(pero quitando exactamente 4 bytes)
            if (posaux<=(maquinavirtual->TDDS[indiceTDDS].direccionFisica+maquinavirtual->TDDS[indiceTDDS].tamanio)){ //verifica que haya mas de 4 celdas para leer (pila vacia o sino pila con menos de 4 celdas)
                setOffset(&(maquinavirtual->Registros[SP]),getOffset(maquinavirtual->Registros[SP])+4); //aumenta efectivamente el SP
                lectura=consultamemoria(posicion,4,maquinavirtual);
                setValor(op1,lectura);//mantiene la estructura del operando 1 pero le cambia el valor por el de la lectura para asi guardarlo
                actualizaEstructura(*op1,maquinavirtual);
            }
            else{
                printf("ERROR : STACK UNDERFLOW");
                exit(1);
            }
    }
    else{
        printf("ERROR: intento de operacion en un segmento sin memoria");
        exit(1);
    }
}

void  CALL(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    TOperando opaux;
    reiniciaOperando(&opaux);
    cargaOperando(&opaux,2,IP,0,maquinavirtual->Registros[IP],0);
    PUSH(&opaux,op2,maquinavirtual);
    JMP(op1,op2,maquinavirtual);
}

void  STOP(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    int postdds=BytesMasSignificativos(maquinavirtual->Registros[0]);
    setOffset(&(maquinavirtual->Registros[IP]),maquinavirtual->TDDS[postdds].direccionFisica + maquinavirtual->TDDS[postdds].tamanio + 1);
}

void  RET(TOperando *op1,TOperando *op2, EMV *maquinavirtual){
    TOperando opaux;
    reiniciaOperando(&opaux);
    cargaOperando(&opaux,2,IP,0,maquinavirtual->Registros[IP],0);
    POP(&opaux,op2,maquinavirtual);
}

void MuestraDisassembler(EMV maquinavirtual){

    char *nomb[]={"MOV","ADD","SUB","SWAP","MUL","DIV","CMP","SHL","SHR","AND","OR","XOR","SYS","JMP","JZ","JP","JN","JNZ","JNP","JNN","LDL","LDH","RND","NOT","PUSH","POP","CALL","STOP","RET"};
    TElementoD D;
    char instr;
    unsigned char codoperacion;
    unsigned int posinstr;
    TOperando op1,op2;

    printf("Codigo Assembler ejecutado por la Maquina Virtual: \n");
    inicializaDisassembler(&D);

    while (getOffset(maquinavirtual.Registros[IP])< maquinavirtual.TDDS[BytesMasSignificativos(maquinavirtual.Registros[CS])].tamanio){ //la ejecucion termina cuando el IP sobrepasa el espacio de memoria asignado al code segment
        instr=maquinavirtual.MemoriaPrincipal[getOffset(maquinavirtual.Registros[IP])];
        posinstr=maquinavirtual.Registros[IP];
        setOffset(&(maquinavirtual.Registros[IP]),getOffset(maquinavirtual.Registros[IP])+1);
        reseteaDisassembler(&D);
        reiniciaOperando(&op1);
        reiniciaOperando(&op2);
        descifrainstrc(&op1,&op2,&codoperacion,instr,&maquinavirtual,&D);
        if(codoperacion<0xC){ //como las instrucciones no son secuenciales, debe verificarse que existan en los intervalos posibles
            codoperacion;
        }
        else{
            if(codoperacion>=0x30 && codoperacion<0x3F){ //entre 48 a 59
                codoperacion-=36;
            }
            else{
               if(codoperacion==0xF0 || codoperacion==0xF1){ //240
                  codoperacion-=213;
                }
               else{
                  printf("ERROR: codigo de instruccion inexistente ");
                  exit(1);
                }
            }
        }
        cargaD(&D,posinstr,instr,nomb[codoperacion]);
        muestra(D);
    }
}

void generarArchivoImagen(EMV maquinavirtual,char *imagenMV){

    FILE *arch;
    short int memoriabigendian=maquinavirtual.tamaniomem >>10;// divide por 1024 de una forma muy cool
    long int i,registrobigendian;
    char val=1;
    long int tddsbigendian,armatdds;
    arch=fopen(imagenMV,"wb+");
    if (arch!=NULL){
        fwrite("VMI23",5,1,arch);
        fwrite(&val,1,1,arch);//version el 1 no es caracter, es l valor 0x1, sino se almacena el valor en ascii
        memoriabigendian=bigYlittleindian(memoriabigendian,2);
        fwrite(&(memoriabigendian),2,1,arch);//tamanio de la memoria

        for (i=0;i<Reg;i++){
            registrobigendian=bigYlittleindian(maquinavirtual.Registros[i],4);
            fwrite(&registrobigendian,4,1,arch);
        }
        for (i=0;i<TabDescSeg;i++){
            armatdds=maquinavirtual.TDDS[i].direccionFisica<<16;
            armatdds=armatdds|maquinavirtual.TDDS[i].tamanio;
            tddsbigendian=bigYlittleindian(armatdds,4);
            fwrite(&(tddsbigendian),4,1,arch);
        }
        for (i=0;i<maquinavirtual.tamaniomem;i++){
            fwrite(&(maquinavirtual.MemoriaPrincipal[i]),1,1,arch);

        }
        fclose(arch);
    }
    else{
        printf("ERROR: no se pudo crear el archivo de imagen");
        exit(1);
    }
}

void run(char arch[],char *imagenMV,int tamanio,char* nombres_discos[],int cantDiscos,char disassembler[])
{
    EMV maquinavirtual;
    int inicia;
    long int postdds;
    FILE *programa;
    char instr,car;
    unsigned char codoperacion;
    TOperando op1,op2;
    t_fun *fun[]={MOV,ADD,SUB,SWAP,MUL,DIV,CMP,SHL,SHR,AND,OR,XOR,SYS,JMP,JZ,JP,JN,JNZ,JNP,JNN,LDL,LDH,RND,NOT,PUSH,POP,CALL,STOP,RET};
    srand(time(0));
    programa=fopen(arch,"rb");
    maquinavirtual.breakpoint=0;
    if (imagenMV!=NULL)
        maquinavirtual.imagen=1;
    else
        maquinavirtual.imagen=0;

    if(programa==NULL){
        printf("ERROR: no se encuentra archivo");
    }
    else{
        iniciaMV(programa,&maquinavirtual,&inicia,tamanio);
        iniciaDiscos(&maquinavirtual,nombres_discos,cantDiscos);
        fclose(programa);
        if(inicia){
            if(disassembler!=NULL){
                MuestraDisassembler(maquinavirtual);
            }
            postdds=BytesMasSignificativos(maquinavirtual.Registros[0]);
            while (getOffset(maquinavirtual.Registros[IP])< maquinavirtual.TDDS[postdds].tamanio){ //la ejecucion termina cuando el IP sobrepasa el espacio de memoria asignado al code segment
                instr=maquinavirtual.MemoriaPrincipal[(unsigned int)getOffset(maquinavirtual.Registros[IP])];
                setOffset(&(maquinavirtual.Registros[IP]),getOffset(maquinavirtual.Registros[IP])+1);
                reiniciaOperando(&op1);
                reiniciaOperando(&op2);
                descifrainstrc(&op1,&op2,&codoperacion,instr,&maquinavirtual,NULL);
                if(codoperacion<0xC){ //como las instrucciones no son secuenciales, debe verificarse que existan en los intervalos posibles
                    (*fun[codoperacion])(&op1,&op2,&maquinavirtual);
                }
                else{
                    if(codoperacion>=0x30 && codoperacion<0x3F){ //entre 48 a 59
                       (*fun[codoperacion-36])(&op1,&op2,&maquinavirtual); //se resta 36 al codigo de operacion,puesto que el vector es secuencial, es decir no contempla la posicion de los codop que se reciben
                    }
                    else
                        if(codoperacion==0xF0|| codoperacion==0xF1){ //240
                            (*fun[codoperacion-213])(&op1,&op2,&maquinavirtual); //se resta 216 x la misma razon
                        }
                        else{
                            printf("ERROR: codigo de instruccion inexistente ");
                            exit(1);
                        }
                }

                if (maquinavirtual.breakpoint==1){
                    generarArchivoImagen(maquinavirtual,imagenMV);
                    do{
                            printf("Breakpoint: 'q':quit | 'enter':continuar\n");
                            car=getchar();
                            fflush(stdin);
                    }while(car!=0xA && car!='q'); //0xa en enter

                    if(car=='q'){
                        maquinavirtual.breakpoint=0;
                    }


                }
                if (getOffset(maquinavirtual.Registros[IP])<0){
                    maquinavirtual.Registros[IP]=maquinavirtual.TDDS[postdds].tamanio+maquinavirtual.TDDS[postdds].direccionFisica; // AFUERA DEL CS para que salga del while
                    printf("ERROR: IP con valor NEGATIVO");
                }

            }
        }
    }
}
