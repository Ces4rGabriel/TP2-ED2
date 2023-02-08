#ifndef QUICKSORT
#define QUICKSORT
//QUICKSORT
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef TipoItem;
typedef TipoApontador;
typedef TipoItem TipoRegistro;
typedef TipoArea;

FILE *ArqLEs;
FILE *ArqEi;
FILE *ArqLi;
TipoItem R;

void quickSortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir);
void LeSup(FILE **ArqLEs, TipoRegistro *UltLido, int *Ls, short *OndeLer);
void LeInf(FILE **ArqLi, TipoRegistro *UltLido, int *Li, short *OndeLer);
void inserirArea(TipoArea *Area, TipoRegistro *UltLido, int *NRArea);
void escreveMax(FILE **ArqLEs, TipoRegistro R, int *Es);
void escreveMin(FILE **ArqEi, TipoRegistro R, int *Ei);
void retiraMax(TipoArea *Area, TipoRegistro R, int *NRArea);
void retiraMin(TipoArea *Area, TipoRegistro R, int *NRArea);
void particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea Area, int Esq, int Dir, int *i, int *j);




#endif 