#ifndef TDAFUNCIONES_H_INCLUDED
#define TDAFUNCIONES_H_INCLUDED

typedef void t_fun2op(int*,int*);
typedef void t_fun1op(int *,T_Registros*,T_VecMemoriaPriPal);
typedef void t_fun0op();// no se si esto sirve.

void mov(int*,int*);
void add(int*,int*);
void sub(int*,int*);
void swapx(int*,int*);
void mul(int*,int*);
void div(int*,int*);
void cmp(int*,int*);
void shl(int*,int*);
void shr(int*,int*);
void and(int*,int*);
void or(int*,int*);
void xor(int*,int*);


//1 operando:
//TODOS TRABAJAN CON UN OPERANDO, LOS REGISTROS y MemoriaPrincipal, ALGUNOS LOS USAN ALGUNOS NO
void sys(int *,T_Registros*,MemoriaPrincipal);
void jmp(int *,T_Registros*,T_VecMemoriaPriPal);
void jz(int *,T_Registros*,T_VecMemoriaPriPal);
void jp(int *,T_Registros*,T_VecMemoriaPriPal);
void jn(int *,T_Registros*,T_VecMemoriaPriPal);
void jnz(int *,T_Registros*,T_VecMemoriaPriPal);
void jnp(int *,T_Registros*,T_VecMemoriaPriPal);
void jnn(int *,T_Registros*,T_VecMemoriaPriPal);
void ldl(int *,T_Registros*,T_VecMemoriaPriPal);
void ldh(int *,T_Registros*,T_VecMemoriaPriPal);
void rnd(int *,T_Registros*,T_VecMemoriaPriPal);
void not(int *,T_Registros*,T_VecMemoriaPriPal);


#endif // TDAFUNCIONES_H_INCLUDED
