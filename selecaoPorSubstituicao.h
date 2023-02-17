#ifndef SELECAOPORSUBSTITUICAO_H_INCLUDED
#define SELECAOPORSUBSTITUICAO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "Fitas\geraFitas.h"

typedef struct TipoItem {
    int chave;
    int marcado;
    int origem;
} TipoItem;

typedef struct TipoRegistro {
    int ne;
    TipoItem *Area;
} TipoRegistro;


void adicionaArea(TipoRegistro *reg, TipoItem Item, TipoItem UltSair);
void adicionaNaFita(TipoItem Item, int nArquivo, TipoItem *UltSair);
int verificaMarcado(TipoItem *Area, int TamArea);
int selecaoPorSubstituicao(int TamArea);
TipoItem retiraArea(TipoRegistro *reg);
TipoRegistro* inicializa(int TamArea);
TipoItem retiraArquivo(FILE *arq);

#endif // SELECAOPORSUBSTITUICAO_H_INCLUDED