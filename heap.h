#ifndef heap_h
#define heap_h

#include <stdbool.h>
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


void HEAP_REFAZ(Estrutura *v, int esq, int dir);
int HEAP_CONSTROI(Estrutura *v, int tam);
void Insere_No(Estrutura *v, TipoRegistro elem, int *tam);
bool remove_No(Estrutura *v, int *tam, Analise *comp);
bool substitui(Estrutura *v, int *tam, TipoRegistro elem, Analise *comp);

#endif