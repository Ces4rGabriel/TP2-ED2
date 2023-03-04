#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct{
    long matricula;
    double nota;
    char estado[4];
    char cidade[52];
    char curso[40];
}TipoRegistro;

void salvar_arquivo_binario(const char *nome_arquivo_txt, const char *nome_arquivo_bin) {
    TipoRegistro r;
    FILE *arquivo_txt = fopen(nome_arquivo_txt, "r");
    FILE *arquivo_bin = fopen(nome_arquivo_bin, "wb");

    while (fscanf(arquivo_txt, "%ld", &r.matricula) == 1) {
        fscanf(arquivo_txt, "%lf", &r.nota);
        fscanf(arquivo_txt, "%3s", r.estado);
        fgets(r.cidade, 52, arquivo_txt);
        fgets(r.curso, 40, arquivo_txt);

        fwrite(&r, sizeof(TipoRegistro), 1, arquivo_bin);
    }

    fclose(arquivo_txt);
    fclose(arquivo_bin);
}

void transform_txt(const char *nome_arquivo_bin, const char *nome_arquivo_txt) {
    TipoRegistro r;
    FILE *arquivo_bin = fopen(nome_arquivo_bin, "rb");
    FILE *arquivo_txt = fopen(nome_arquivo_txt, "w");

    while (fread(&r, sizeof(TipoRegistro), 1, arquivo_bin) != 0) {
        fprintf(arquivo_txt, "%08ld ", r.matricula);
        fprintf(arquivo_txt, "%05.1f ", r.nota);
        fprintf(arquivo_txt, "%s", r.estado);
        fprintf(arquivo_txt, "%s", r.cidade);
        fprintf(arquivo_txt, "%s", r.curso);
    }

    fclose(arquivo_bin);
    fclose(arquivo_txt);
}
//ler arquivo binario ascendente e criar um arquivo binario descrescente
void cria_descr(const char *nome_arquivo_bin, const char *nome_arquivo_bin_desc) {
    TipoRegistro r;
    int qtd_registros;
    FILE *arquivo_bin, *arquivo_bin_desc;

    // Abre o arquivo binário de entrada em modo leitura
    arquivo_bin = fopen(nome_arquivo_bin, "rb");
    if (arquivo_bin == NULL) {
        printf("Erro ao abrir o arquivo %s em modo leitura\n", nome_arquivo_bin);
        return;
    }

    // Abre o arquivo binário de saída em modo escrita
    arquivo_bin_desc = fopen(nome_arquivo_bin_desc, "wb");
    if (arquivo_bin_desc == NULL) {
        printf("Erro ao abrir o arquivo %s em modo escrita\n", nome_arquivo_bin_desc);
        fclose(arquivo_bin);
        return;
    }

    // Obtém a quantidade de registros no arquivo de entrada
    fseek(arquivo_bin, 0, SEEK_END);
    qtd_registros = ftell(arquivo_bin) / sizeof(TipoRegistro);
    fseek(arquivo_bin, 0, SEEK_SET);

    // Escreve os registros do arquivo de entrada no arquivo de saída em ordem descendente
    for (int i = qtd_registros - 1; i >= 0; i--) {
        // Lê o registro do arquivo de entrada
        fseek(arquivo_bin, i * sizeof(TipoRegistro), SEEK_SET);
        fread(&r, sizeof(TipoRegistro), 1, arquivo_bin);

        // Escreve o registro no arquivo de saída
        fwrite(&r, sizeof(TipoRegistro), 1, arquivo_bin_desc);
    }

    // Fecha os arquivos
    fclose(arquivo_bin);
    fclose(arquivo_bin_desc);
}

void ler_arqb(const char *nome_arquivo_bin) {
    TipoRegistro r;
    FILE *arquivo_bin = fopen(nome_arquivo_bin, "rb");

    while (fread(&r, sizeof(TipoRegistro), 1, arquivo_bin) != 0) {
        printf("%08ld ", r.matricula);
        printf("%05.1f ", r.nota);
        printf("%s", r.estado);
        printf("%s", r.cidade);
        printf("%s", r.curso);
    }

    fclose(arquivo_bin);
}

int main(){
    //arquivo desordenado
    //salvar_arquivo_binario("PROVAO.TXT", "PROVAO.bin");
    //cria_descr("PROVAO_ORDENADO.bin", "PROVAO_ORDENADO2.bin");
    

    //arquivo ordenado
    //cria_descr("PROVAO_ORDENADO.bin", "PROVAO_DESORDENADO.bin");
    //transform_txt("PROVAO_DESCRESCENTE.bin", "PROVAO_DESCRESCENTE.TXT");

    ler_arqb("PROVAO_DESORDENADO.bin");

    return 0;
}