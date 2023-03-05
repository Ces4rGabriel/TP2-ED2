#include "arquivo.h"

// Fecha todos arquivos do vetor de arquivos
void fechaArq(FILE *arqvs[TOTALFITA]) {
    for (int i = 0; i < TOTALFITA; i++) {
        fclose(arqvs[i]);
    }
}
// Cria uma lista de nomes para cada fita enumerando-as
void nomeiaArquivo(char nomes[TOTALFITA][TOTALFITA]) {
    char buffer[3];
    char aux[TOTALFITA] = {"./data/fita"};

    for (int i = 1; i <= TOTALFITA; i++) {
        sprintf(buffer, "%d", i);
        strcat(aux, buffer);
        strcpy(nomes[i - 1], strcat(aux, ".dat"));
        aux[11] = '\0';
    }
}
// Apre todos as fitas
void criaArquivo(FILE *arqvs[TOTALFITA], char nomes[TOTALFITA][TOTALFITA]) {
    for (int i = 0; i < 20; i++) {
        arqvs[i] = fopen(nomes[i], "wb+");
    }
}
// Faz a leitura de um único elemento(TipoRegistro), de um arquivo passado como parâmetro
TipoRegistro readFile(FILE *prova) {
    TipoRegistro aluno;

    fread(&aluno, sizeof(TipoRegistro), 1, prova);

    return aluno;
}
// Trocc dois arquivos de posição
void trocarArquivos(FILE *arqvs[TOTALFITA], int a, int b) {
    FILE *aux = arqvs[a];
    arqvs[a] = arqvs[b];
    arqvs[b] = aux;
}
