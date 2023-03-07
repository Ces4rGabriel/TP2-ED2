#ifndef heap_h
#define heap_h


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arquivo.h"

typedef struct {
    int numLeitura;
    int numEscrita;
    long long int numComparacoes;
    clock_t tempoInicial;
    clock_t tempoFinal; 
} Analise;


void heapfy(TipoAluno *vAlunos, int esq, int dir);
int heapsort(TipoAluno *vAlunos, int tam);
void Insere_No(TipoAluno *vAlunos, TipoRegistro elem, int *tam);
int removeArea(TipoAluno *vAlunos, int *tam, Analise *comp);
int adicionaArea(TipoAluno *vAlunos, int *tam, TipoRegistro elem, Analise *comp);
int verificaMarcados(TipoAreaS *v);

#endif