#ifndef quicksort_h
#define quicksort_h

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <float.h>

#define TAM_AREA 10
#define TAM_TOTAL 471705

typedef struct{
    long matricula;
    double nota;
    char estado[4];
    char cidade[52];
    char curso[40];
}TipoRegistro;

typedef TipoRegistro* TipoArea;
void quicksort_main(int nRegistros, int printResult, int situacao);
void quicksortExterno(FILE **arqLi, FILE **arqEi, FILE **arqLEs, int esq, int dir);
void particao(FILE **arqLi, FILE **arqEi, FILE **arqLEs, TipoArea *area, int esq, int dir, int *i, int *j);
void leSup(FILE **arqLEs, TipoRegistro *ultimoLido, int *ls, short *ondeLer);
void leInf(FILE **arqLi, TipoRegistro *ultimoLido, int *li, short *ondeLer);
void inserirArea(TipoArea *area, TipoRegistro *ultimoLido, int *NRArea);
void insereItem(TipoRegistro *ultimoLido, TipoArea *area);
void escreveMax(FILE **arqLEs, TipoRegistro R, int *es);
void escreveMin(FILE **arqEi, TipoRegistro R, int *ei);
void retiraMax(TipoArea *area, TipoRegistro *R, int *NRArea);
void retiraMin(TipoArea *area, TipoRegistro *R, int *NRArea);
void retiraUltimo(TipoArea *area, TipoRegistro *R);
void retiraPrimeiro(TipoArea *area, TipoRegistro *R);
TipoArea* criaAreaVazia(TipoArea *area);
int obterNumCelulasOcupadas(TipoArea *area);
void ordenaArea(TipoArea *area);
void copiarAluno(TipoRegistro *destino, TipoRegistro origem);
void exibir(int opcional, char *nomeArquivo, int quantidade);
#endif
