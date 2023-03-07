#include "2F.h"

extern Analise analise;

void intercalcaoMain2F(int quantidade, int situacao, int printResult) {
    analise.numComparacoes = 0;
    analise.numEscrita = 0;
    analise.numLeitura = 0;
    char nomeArq[50];

    FILE *prova;
    	switch (situacao)
	{
	case 1:
		strcpy(nomeArq, "arquivos/PROVAO_ORDENADO_TMP.bin");
		break;
	case 2:
		strcpy(nomeArq, "arquivos/PROVAO_DESCRESCENTE_TMP.bin");
		break;
	case 3:
		strcpy(nomeArq, "arquivos/PROVAO_DESORDENADO_TMP.bin");
		break;
	}
    prova = fopen(nomeArq, "rb");
    char nomes[TOTALFITA][TOTALFITA] = {""};  // Vetor de nomes para criar as fitas
    nomeiaArquivo(nomes);

    FILE *arqvs[TOTALFITA];  // Apontador para as fitas
    criaArquivo(arqvs, nomes);

    // início da fase de criação de blocos ordenados
    int vetTam = 0;
    TipoAluno alunosEmMemoria[FF_TAMFITAINT];  // heap da memória
    for (int i = 0; i < FF_TAMFITAINT; i++) {
        alunosEmMemoria[i].aluno = readFile(prova);
        alunosEmMemoria[i].marcado = 0;
        vetTam++;
    }

    heapsort(alunosEmMemoria, vetTam);
    FF_geraBlocos(arqvs, alunosEmMemoria, prova, &vetTam, quantidade);
    // fim da fase de criação de blocos ordenados

    // início da intercalação
    analise.tempoInicial = clock();
    int tamEntrada = FF_inicializarMemoriaIntercalacao(arqvs, alunosEmMemoria, 0);
    int fitaSaida = FF_intercalacao(arqvs, alunosEmMemoria, 0, FF_POSFITAEXT, tamEntrada, 1);
    analise.tempoFinal = clock();
    // fim da intercalação

    FF_exibirResultados(quantidade, printResult, arqvs, fitaSaida);

    fechaArq(arqvs);
    fclose(prova);
    system("rm -rf data/*");
}

void esvaziarHeap(TipoAluno alunosEmMemoria[FF_TAMFITAINT]) {
    TipoRegistro alunoNulo = getAlunoVazio();
    for (int i = 0; i < 10; i++) {
        alunosEmMemoria[i].aluno = alunoNulo;
    }
}

void FF_geraBlocos(FILE *arqvs[TOTALFITA], TipoAluno alunosEmMemoria[FF_TAMFITAINT], FILE *prova, int *vetTam, int quantidade) {
    int count = 0;
    int numfita = 0;
    TipoRegistro alunoNulo = getAlunoVazio();

    while (*vetTam > 1 && count < quantidade) {
        fwrite(&alunosEmMemoria[0].aluno, sizeof(TipoRegistro), 1, arqvs[numfita]);
        if (feof(prova)) {
            if (removeArea(alunosEmMemoria, vetTam, &analise)) {
                fwrite(&alunoNulo, sizeof(TipoRegistro), 1, arqvs[numfita]);
                numfita += 1;
            }
        } else if (adicionaArea(alunosEmMemoria, vetTam, readFile(prova), &analise)) {
            fwrite(&alunoNulo, sizeof(TipoRegistro), 1, arqvs[numfita]);
            numfita += 1;
        }

        if (*vetTam == 1)
            fwrite(&alunoNulo, sizeof(TipoRegistro), 1, arqvs[numfita]);

        if (numfita >= FF_TAMFITAINT)
            numfita = 0;
        count++;
    }
    *vetTam = numfita + 1;
    esvaziarHeap(alunosEmMemoria);

    for (int i = 0; i < TOTALFITA; i++) {
        rewind(arqvs[i]);
    }
}
/*
 * Preenche o heap o primeiro valor de cada fita de entrada
 * retorna a quantidade de fitas com valores reais
 */
int FF_inicializarMemoriaIntercalacao(FILE *arqvs[TOTALFITA], TipoAluno alunosEmMemoria[FF_TAMFITAINT], int fitaEntradaInicial) {
    TipoRegistro aluno;
    int countFitasNaoVazias = 0;
    int firstIndex = getFirstCurrentIndex(fitaEntradaInicial);
    for (int i = 0; i < FF_TAMFITAINT; i++) {
        if (fread(&aluno, sizeof(TipoRegistro), 1, arqvs[firstIndex + i]) && aluno.nota != -1) {
            analise.numLeitura++;
            alunosEmMemoria[countFitasNaoVazias].aluno = aluno;
            alunosEmMemoria[countFitasNaoVazias].marcado = 0;
            alunosEmMemoria[countFitasNaoVazias].posFita = firstIndex + i;
            countFitasNaoVazias++;
        }
    }
    heapsort(alunosEmMemoria, countFitasNaoVazias);
    return countFitasNaoVazias;
}

void limparEntradas(FILE *arqvs[TOTALFITA], int startIndex, int endIndex) {
    char nomes[TOTALFITA][TOTALFITA] = {""};
    nomeiaArquivo(nomes);
    for (int i = 0; i < TOTALFITA; i++) {
        if (i >= startIndex && i <= endIndex) {
            fclose(arqvs[i]);
            arqvs[i] = fopen(nomes[i], "wb+");
        } else {
            rewind(arqvs[i]);
        }
    }
}

/*
 * função recursiva para intercalação
 * retorna o índice da fita de saída ordenada com o resultado
 */
int FF_intercalacao(FILE *arqvs[TOTALFITA], TipoAluno alunosEmMemoria[FF_TAMFITAINT], int fitaEntradaAtual, int fitaSaidaAtual, int tamEntrada, int countNiveis) {
    TipoRegistro vazio = getAlunoVazio();

    while (tamEntrada > 0) {
        // guarda a fita de origem do item a ser inserido na saída
        fitaEntradaAtual = alunosEmMemoria[0].posFita;

        // escreve o item na saída
        fwrite(&alunosEmMemoria[0].aluno, sizeof(TipoRegistro), 1, arqvs[fitaSaidaAtual]);
        analise.numEscrita++;

        TipoRegistro aluno;
        bool novaFita = 0;
        analise.numLeitura++;
        // caso tenha acabado a fita de entrada, remove o ultimo item do heap
        if (feof(arqvs[fitaEntradaAtual]) || !fread(&aluno, sizeof(TipoRegistro), 1, arqvs[fitaEntradaAtual]) || aluno.nota == -1) {
            novaFita = removeArea(alunosEmMemoria, &tamEntrada, &analise);
        } else {
            alunosEmMemoria[0].posFita = fitaEntradaAtual;
            // substitui o proximo item da fita de entrada no heap
            novaFita = adicionaArea(alunosEmMemoria, &tamEntrada, aluno, &analise);
        }

        // caso não exista mais itens na fita, ou todos os itens do heap estão marcados
        if (novaFita) {
            // escreve o delimitador vazio na saida
            fwrite(&vazio, sizeof(TipoRegistro), 1, arqvs[fitaSaidaAtual]);

            // verifica se há mais itens na fita de entrada
            int newTamEntrada = FF_inicializarMemoriaIntercalacao(arqvs, alunosEmMemoria, fitaEntradaAtual);
            int newFitaEntrada = alunosEmMemoria[0].posFita;
            if (newTamEntrada > 0) {
                // caso haja entradas, escreve na próxima fita de saída ou volta para primeira, subindo um nível
                if (fitaSaidaAtual + 1 <= getLastCurrentIndex(fitaSaidaAtual)) {
                    return FF_intercalacao(arqvs, alunosEmMemoria, newFitaEntrada, fitaSaidaAtual + 1, newTamEntrada, countNiveis);
                } else {
                    return FF_intercalacao(arqvs, alunosEmMemoria, newFitaEntrada, getFirstCurrentIndex(fitaSaidaAtual), newTamEntrada, countNiveis + 1);
                }
            } else {
                // caso não haja mais entradas, verifica se apenas uma fita de saída foi escrita
                if (fitaSaidaAtual == getFirstCurrentIndex(fitaSaidaAtual) && countNiveis == 1) {
                    // caso sim, finaliza a intercalação retornando o índice da fita de saída
                    return fitaSaidaAtual;
                } else {
                    // limpa os arquivos de entrada
                    limparEntradas(arqvs, getFirstCurrentIndex(fitaEntradaAtual), getLastCurrentIndex(fitaEntradaAtual));
                    // preenche o heap para a próxima intercalação
                    newTamEntrada = FF_inicializarMemoriaIntercalacao(arqvs, alunosEmMemoria, getFirstOppositeIndex(fitaEntradaAtual));

                    // faz a chamada recursiva trocando as entradas e saídas
                    return FF_intercalacao(arqvs, alunosEmMemoria, getFirstOppositeIndex(fitaEntradaAtual), getFirstCurrentIndex(fitaEntradaAtual), newTamEntrada, 1);
                }
            }
        }
    }
    return fitaEntradaAtual;
}

/*
 * input:   0-9|10-19
 * returns:  0 | 10
 */
int getFirstCurrentIndex(int currentFita) {
    return (currentFita >= FF_POSFITAEXT) * FF_POSFITAEXT;
}
/*
 * input:   0-9|10-19
 * returns: 10 | 0
 */
int getFirstOppositeIndex(int currentFita) {
    return !(currentFita >= FF_POSFITAEXT) * FF_POSFITAEXT;
}

/*
 * input:   0-9|10-19
 * returns:  9 | 19
 */
int getLastCurrentIndex(int currentFita) {
    return (currentFita >= FF_POSFITAEXT) * FF_POSFITAEXT + FF_POSFITAEXT - 1;
}

/*
 * input:   0-9|10-19
 * returns:  19 | 9
 */
int getLastOppositeIndex(int currentFita) {
    return getLastCurrentIndex(getFirstOppositeIndex(currentFita));
}

/*
 * input:   0-9|10-19
 * returns:  1 | 0
 */
int isInFirstTape(int currentTape) {
    return currentTape < FF_POSFITAEXT;
}

TipoRegistro getAlunoVazio() {
    TipoRegistro aluno = (TipoRegistro){-1};
    aluno.nota = (float)-1.0;
    return aluno;
}

void FF_exibirResultados(int quantidade, int printResult, FILE *arqvs[TOTALFITA], int fitaSaida) {
    FILE *resultFile = fopen("resultado.txt", "w");
    if (printResult) {
        printf("\n\n\nRESULTADO: FITA %d \n", fitaSaida + 1);
        fprintf(resultFile, "\n\n\nRESULTADO: FITA %d \n", fitaSaida + 1);
        TipoRegistro res;
        rewind(arqvs[fitaSaida]);
        while (fread(&res, sizeof(TipoRegistro), 1, arqvs[fitaSaida]) && quantidade--) {
            if (res.nota != -1) {
                printf("%08ld\t%05.1lf\t%s\t%s\t%s", res.matricula, res.nota, res.estado, res.cidade, res.curso);
                fprintf(resultFile, "%08ld\t%05.1lf\t%s\t%s\t%s", res.matricula, res.nota, res.estado, res.cidade, res.curso);
            }
        }
        rewind(arqvs[fitaSaida]);
    }

    printf("\nAnalise:\n");
    printf("\nNumero de Leituras: %d", analise.numLeitura);
    printf("\nNumero de Escritas: %d", analise.numEscrita);
    printf("\nNumero de Comparacoes: %lld", analise.numComparacoes);
    printf("\nTempo  de Execucao: %lf s", ((double)analise.tempoFinal - analise.tempoInicial) / CLOCKS_PER_SEC);
    printf("\nFim.\n\n");

    fclose(resultFile);
}

void imprimirFitas(FILE *arqvs[TOTALFITA]) {
    TipoRegistro teste;
    for (int i = 0; i < 19; i++) {
        rewind(arqvs[i]);
        printf("FITA %d \n", i + 1);
        while (fread(&teste, sizeof(TipoRegistro), 1, arqvs[i])) {
            if (teste.nota != -1) {
                printf("%ld ", teste.matricula);
                printf("%.2f ", teste.nota);
                printf("%s ", teste.estado);
                printf("%s ", teste.cidade);
                printf("%s \n", teste.curso);
            }
        }
        printf("FITA %d \n", i + 1);
        getchar();
        printf("\n\n\n");
        rewind(arqvs[i]);
    }
}