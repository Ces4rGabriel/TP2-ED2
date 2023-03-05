#ifndef arquivo_h
#define arquivo_h

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTALFITA 20

typedef struct{
    long matricula;
    double nota;
    char estado[4];
    char cidade[52];
    char curso[40];
}TipoRegistro;

typedef struct {
    TipoRegistro aluno;
    bool maior;
    int posFita;
} Estrutura;

void fechaArq(FILE *arqvs[TOTALFITA]);
void nomeiaArquivo(char nomes[TOTALFITA][TOTALFITA]);
void criaArquivo(FILE *arqvs[TOTALFITA], char nomes[TOTALFITA][TOTALFITA]);
TipoRegistro readFile(FILE *prova);
void trocarArquivos(FILE *arqvs[TOTALFITA], int a, int b);

#endif