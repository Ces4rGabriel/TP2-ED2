#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "quicksort.h"

void duplicaArq(char *nomeArq, int situacao);

int main(int argc, char const *argv[]){
    int metodo, nRegistros, situacao, printResult = 0;
    char nomeArquivo[50];
    metodo = atoi(argv[1]); //1 = xx, 2 = Axxx, 3 = QuickSort Externo
    nRegistros = atoi(argv[2]);
    situacao = atoi(argv[3]);  
    if ((argc == 5) && strcmp(argv[4], "-P") == 0)
        printResult = 1;
        
    //escolher arquivo
    switch (situacao)
    {
    case 1:
        strcpy(nomeArquivo, "arquivos/PROVAO_ORDENADO.bin");
        break;
    case 2:
        strcpy(nomeArquivo, "arquivos/PROVAO_DESCRESCENTE.bin");
        break;
    case 3:
        strcpy(nomeArquivo, "arquivos/PROVAO_DESORDENADO.bin");
        break;
    }
    //duplica arquivo
    duplicaArq(nomeArquivo, situacao);

    //escolher metodo
    switch (metodo)
    {
    case 1:
        break;
    case 2:
        break;
    case 3:
        quicksort_main(nRegistros, printResult, situacao);
        break;
    }
}


void duplicaArq(char *nomeArq, int situacao){
    char nomeArq2[50];
    TipoRegistro aluno;
    FILE *arq1, *arq2;

    switch (situacao)
    {
    case 1:
        strcpy(nomeArq2, "arquivos/PROVAO_ORDENADO_TMP.bin");
        break;
    case 2:
        strcpy(nomeArq2, "arquivos/PROVAO_DESCRESCENTE_TMP.bin");
        break;
    case 3:
        strcpy(nomeArq2, "arquivos/PROVAO_DESORDENADO_TMP.bin");
        break;
    }
    arq1 = fopen(nomeArq, "rb");
    arq2 = fopen(nomeArq2, "wb");
    //copia
    while (fread(&aluno, sizeof(TipoRegistro), 1, arq1) == 1){
        fwrite(&aluno, sizeof(TipoRegistro), 1, arq2);
    }
    fclose(arq1);
    fclose(arq2);
}