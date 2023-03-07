#include "quicksort.h"
extern Analise analise;

clock_t tempoExecucaoInicioQuick;
clock_t tempoExecucaoFimQuick;


void quicksort_main(int nRegistros, int printResult, int situacao){
	FILE *arqLi;
	FILE *arqEi;
	FILE *arqLEs;
	int quantidade = nRegistros;
	char nomeArq[50];
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

	arqLi = fopen(nomeArq, "r+b");
	arqEi = fopen(nomeArq, "r+b");
	arqLEs = fopen(nomeArq, "r+b");

	if(arqLi == NULL || arqEi == NULL || arqLEs == NULL){
		printf("\nERRO: Falha ao abrir arquivo");
		return;
	}
	//Chamada da função para imprimir arquivos iniciais

	/**
	 * Chamada para ordenação
	 */  
	//tempoExecucaoInicioQuick = clock();
	quicksortExterno(&arqLi, &arqEi, &arqLEs, 1, quantidade);
	analise.tempoFinal = clock();

	fclose(arqLi);
	fclose(arqEi);
	fclose(arqLEs);

	/**
	 * Exibição de resultados de desempenho
	 */ 

	exibir(printResult, nomeArq, quantidade);
	return;
}

//Função para imprimir os arquivos iniciais

void quicksortExterno(FILE **arqLi, FILE **arqEi, FILE **arqLEs, int esq, int dir){
	int i, j;

	/**
	 * Se o indice da direita for menor ou igual ao indice da esquerda,
	 * significa que não há registros para serem ordenados ou as informações
	 * passadas por parâmetro estão incorretas.
	 */ 
	if(dir <= esq)
		return;

	/**
	 * Malloca área e define todas as células com valores inválidos
	 */
	TipoArea *area = criaAreaVazia();

	// Particiona
	particao(arqLi, arqEi, arqLEs, area, esq, dir, &i, &j);

	// Recursão
	if(i - esq < dir - j){
		quicksortExterno(arqLi, arqEi, arqLEs, esq, i);
		quicksortExterno(arqLi, arqEi, arqLEs, j, dir);
	}

	else {
		quicksortExterno(arqLi, arqEi, arqLEs, j, dir);
		quicksortExterno(arqLi, arqEi, arqLEs, esq, i);
	}
	//libera memoria da área
	for (int i = 0; i < TAM_AREA; i++)
		free(area[i]);
	free(area);
}

void particao(FILE **arqLi, FILE **arqEi, FILE **arqLEs, TipoArea *area, int esq, int dir, int *i, int *j){
	int ls = dir, es = dir, li = esq, ei = esq, NRArea = 0, Linf = INT_MIN, Lsup = INT_MAX;
	short ondeLer = true; 
	TipoRegistro ultimoLido, R;
	
	fseek(*arqLi, (li - 1) * sizeof(TipoRegistro), SEEK_SET);
	fseek(*arqEi, (li - 1) * sizeof(TipoRegistro), SEEK_SET);
	
	*i = esq - 1; 
	*j = dir + 1;
	
	/**
	 * A partição é feita até que Li e Ls se cruzem
	 */ 
	while(ls >= li){
		if(NRArea < TAM_AREA - 1){	
			if (ondeLer)
				leSup(arqLEs, &ultimoLido, &ls, &ondeLer);
			else 
				leInf(arqLi, &ultimoLido, &li, &ondeLer);
				
			inserirArea(area, &ultimoLido, &NRArea);
			continue;
		}
		
		/**
		 * Para garantir que os apontadores de escrita estejam atrás 
		 * dos apontadores de leitura, a ordem alternada de leitura é 
		 * interrompida se (Li = Ei) ou (Ls = Es).
		 */
		if(ls == es)
			leSup(arqLEs, &ultimoLido, &ls, &ondeLer);
		
		else if(li == ei)
			leInf(arqLi, &ultimoLido, &li, &ondeLer);
		
		else if(ondeLer)
			leSup(arqLEs, &ultimoLido, &ls, &ondeLer);
		
		else
			leInf(arqLi, &ultimoLido, &li, &ondeLer);
		analise.numComparacoes++;

		if(ultimoLido.nota > Lsup){
			*j = es;
			escreveMax(arqLEs, ultimoLido, &es);
			continue;
		}
		analise.numComparacoes++;

		if(ultimoLido.nota < Linf){
			*i = ei;
			escreveMin(arqEi, ultimoLido, &ei);
			continue;
		}
		
		inserirArea(area, &ultimoLido, &NRArea);
		
		if(ei - esq < dir - es){
			retiraMin(area, &R, &NRArea);
			escreveMin(arqEi, R, &ei);
			
			Linf = R.nota;
		}

		else {
			retiraMax(area, &R, &NRArea);
			escreveMax(arqLEs, R, &es);
			
			Lsup = R.nota;
		}
	}

	while (ei <= es){
		retiraMin(area, &R, &NRArea);
		escreveMin(arqEi, R, &ei);
	}
}


void leSup(FILE **arqLEs, TipoRegistro *ultimoLido, int *ls, short *ondeLer){
	fseek(*arqLEs, (*ls - 1) * sizeof(TipoRegistro), SEEK_SET);
	fread(ultimoLido, sizeof(TipoRegistro), 1, *arqLEs);

	(*ls)--; 

	*ondeLer = false;
	analise.numLeitura++;

}

void leInf(FILE **arqLi, TipoRegistro *ultimoLido, int *li, short *ondeLer){
	fread(ultimoLido, sizeof(TipoRegistro), 1, *arqLi);

	(*li)++; 

	*ondeLer = true;
	analise.numLeitura++;

}

void inserirArea(TipoArea *area, TipoRegistro *ultimoLido, int *NRArea){
	insereItem(ultimoLido, area);
	*NRArea = obterNumCelulasOcupadas(area);
}

void escreveMax(FILE **arqLEs, TipoRegistro R, int *es){
	fseek(*arqLEs, (*es - 1) * sizeof(TipoRegistro), SEEK_SET);
	fwrite(&R, sizeof(TipoRegistro), 1, *arqLEs);
	(*es)--;
	analise.numEscrita++;
	
}

void escreveMin(FILE **arqEi, TipoRegistro R, int *ei){
	fwrite(&R, sizeof(TipoRegistro), 1, *arqEi);
	(*ei)++;
	analise.numEscrita++;
	
}


void insereItem(TipoRegistro *ultimoLido, TipoArea *area){

	int i = obterNumCelulasOcupadas(area);
	

	copiarAluno(area[i], *ultimoLido);

	ordenaArea(area);
}

void retiraMax(TipoArea *area, TipoRegistro *R, int *NRArea){
	retiraUltimo(area, R);
	*NRArea = obterNumCelulasOcupadas(area);
}

void retiraMin(TipoArea *area, TipoRegistro *R, int *NRArea){
	retiraPrimeiro(area, R);
	*NRArea = obterNumCelulasOcupadas(area);
}

void retiraUltimo(TipoArea *area, TipoRegistro *R){
	//Obtém a última célula ocupada
	int i = obterNumCelulasOcupadas(area) - 1;
	copiarAluno(R, *area[i]);

	//Marca a célula como vazia
	area[i]->matricula = -1;
	area[i]->nota = FLT_MAX;
}

void retiraPrimeiro(TipoArea *area, TipoRegistro *R){
	int i = 0;
	copiarAluno(R, *area[i]);

	area[i]->matricula = -1;
	area[i]->nota = FLT_MAX;

	ordenaArea(area);
}

TipoArea* criaAreaVazia(){
	TipoArea* area = malloc(sizeof(TipoArea) * TAM_AREA);
	for (int i = 0; i < TAM_AREA; i++){
		TipoRegistro *aluno = malloc(sizeof(TipoRegistro));
		aluno->matricula = -1;
		aluno->nota = FLT_MAX;

		area[i] = aluno;
	}

	return area;
}

//-----------------------------------------------
void quicksortArea(TipoArea *area, int esquerda, int direita) {
    if (esquerda >= direita) return;

    int i = esquerda;
    int j = direita;
    TipoRegistro *pivo = area[(esquerda + direita) / 2];

    while (i <= j) {
        while (area[i]->nota < pivo->nota){
			i++;
			analise.numComparacoes++;
		} 
        while (area[j]->nota > pivo->nota)
		 {
			j--;
			analise.numComparacoes++;
			}
        if (i <= j) {
            TipoRegistro *temp = area[i];
            area[i] = area[j];
            area[j] = temp;
            i++;
            j--;
        }
    }

    quicksortArea(area, esquerda, j);
    quicksortArea(area, i, direita);
}

void ordenaArea(TipoArea *area) {
    quicksortArea(area, 0, TAM_AREA - 1);
}

//-----------------------------------------------

int obterNumCelulasOcupadas(TipoArea *area){
	int contador = 0;
	for (int i = 0; i < TAM_AREA; i++){
		if(area[i]->matricula != -1)
			contador++;
	}

	return contador;
}

void copiarAluno(TipoRegistro *destino, TipoRegistro origem){
	destino->matricula = origem.matricula;
	destino->nota = origem.nota;
	strcpy(destino->estado, origem.estado);
	strcpy(destino->cidade, origem.cidade);
	strcpy(destino->curso, origem.curso);
}

void escolherArquivoPorSituacaoQuick(int situacao, char* nomeArquivo){
	switch(situacao){
		case 1: strcpy(nomeArquivo, "data/ProvaoAscendente.dat");
				break;
		case 2: strcpy(nomeArquivo, "data/ProvaoDescendente.dat");
				break;
		case 3: strcpy(nomeArquivo, "data/ProvaoAleatorio.dat");
				break; 
	}
}

void exibir(int printResults, char *nomeArquivo, int quantidade){
    FILE *resultFile = fopen("resultado.txt", "w");
    if(printResults){
        FILE *arq;
        arq = fopen(nomeArquivo, "r+b");
        TipoRegistro res;
        for(int i = 0; i < quantidade; i++){
            fread(&res, sizeof(TipoRegistro), 1, arq);
            printf("%ld\t%05.1lf\t%s\t%s\t%s", res.matricula, res.nota, res.estado, res.cidade, res.curso);
            fprintf(resultFile, "%08ld\t%05.1lf\t%s\t%s\t%s", res.matricula, res.nota, res.estado, res.cidade, res.curso);
        }
        fclose(arq);
    }

    printf("\nAnalise:\n");
    printf("\nNumero de Leituras: %d", analise.numLeitura);
    printf("\nNumero de Escritas: %d",analise.numEscrita);
    printf("\nNumero de Comparacoes: %lld",  analise.numComparacoes);
    printf("\nTempo de Execucao: %lfs", ((double)analise.tempoFinal - (double) analise.tempoInicial) / CLOCKS_PER_SEC);
    printf("\nFim.\n\n");


    fclose(resultFile);
}