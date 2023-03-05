#include "quicksort.h"

/**
 * Conjunto de variáveis que armazena o desempenho do método 
 */
int nLeitura = 0;
int nEscrita = 0;
int nComparacoes = 0;
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
	tempoExecucaoInicioQuick = clock();
	quicksortExterno(&arqLi, &arqEi, &arqLEs, 1, quantidade);
	tempoExecucaoFimQuick = clock();

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
	TipoArea *area;
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
	area = criaAreaVazia(area);

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
		
		nComparacoes++;
		if(ultimoLido.nota > Lsup){
			*j = es;
			escreveMax(arqLEs, ultimoLido, &es);
			continue;
		}

		nComparacoes++;
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

	nLeitura++;
}

void leInf(FILE **arqLi, TipoRegistro *ultimoLido, int *li, short *ondeLer){
	fread(ultimoLido, sizeof(TipoRegistro), 1, *arqLi);

	(*li)++; 

	*ondeLer = true;

	nLeitura++;
}

void inserirArea(TipoArea *area, TipoRegistro *ultimoLido, int *NRArea){
	insereItem(ultimoLido, area);
	*NRArea = obterNumCelulasOcupadas(area);
}

void escreveMax(FILE **arqLEs, TipoRegistro R, int *es){
	fseek(*arqLEs, (*es - 1) * sizeof(TipoRegistro), SEEK_SET);
	fwrite(&R, sizeof(TipoRegistro), 1, *arqLEs);
	(*es)--;

	nEscrita++;
}

void escreveMin(FILE **arqEi, TipoRegistro R, int *ei){
	fwrite(&R, sizeof(TipoRegistro), 1, *arqEi);
	(*ei)++;

	nEscrita++;
}

void insereItem(TipoRegistro *ultimoLido, TipoArea *area){
	/**
	 * Variável i representa o indice da primeira celula desocupada da área
	 */ 
	int i = obterNumCelulasOcupadas(area);
	
	/**
	 * Copia as informações do ultimo lido para a primeira celula desocupada da área
	 */
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
	/**
	 * Variável i representa o indice da última célula da área
	 */
	int i = obterNumCelulasOcupadas(area) - 1;
	copiarAluno(R, *area[i]);

	/**
	 * Valores inválidos que representam célula vazia
	 */ 
	area[i]->matricula = -1;
	area[i]->nota = FLT_MAX;
}

void retiraPrimeiro(TipoArea *area, TipoRegistro *R){
	/**
	 * Variável i representa o primeiro indice da área
	 */
	int i = 0;
	copiarAluno(R, *area[i]);

	/**
	 * Valores inválidos que representam célula vazia
	 */ 
	area[i]->matricula = -1;
	area[i]->nota = FLT_MAX;

	/**
	 * Reordenando
	 */ 
	ordenaArea(area);
}

TipoArea* criaAreaVazia(TipoArea *area){
	area = malloc(sizeof(TipoArea) * TAM_AREA);
	for (int i = 0; i < TAM_AREA; i++){
		TipoRegistro *aluno = malloc(sizeof(TipoRegistro));
		aluno->matricula = -1;
		aluno->nota = FLT_MAX;

		area[i] = aluno;
	}

	return area;
}


void ordenaArea(TipoArea *area){
	/**
	 * O método Shell Sort foi escohido para a ordenação da área
	 * pois sua implementação independe de recursão
	 */ 

	int n = TAM_AREA;
	int h;
	for(h = 1; h < n; h = 3 * h + 1);
	
	do {
		h = (h - 1) / 3;

		for(int i = h; i < n; i++){
			TipoRegistro *aux = area[i];
			int j = i;
			
			while(area[j-h]->nota > aux->nota){
				nComparacoes++;
				area[j] = area[j-h];
				j = j - h;
				
				if(j < h)
					break;
			}
			area[j] = aux;
		}
	} while (h != 1);
}

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
	
	if(printResults){
		FILE *arq;
		arq = fopen(nomeArquivo, "r+b");
		TipoRegistro res;
		for(int i = 0; i < quantidade; i++){
			fread(&res, sizeof(TipoRegistro), 1, arq);
			printf("%ld\t%05.1lf\t%s\t%s\t%s", res.matricula, res.nota, res.estado, res.cidade, res.curso);
		}
		fclose(arq);
	}

	/**
	 * Tempo de execução em segundos
	 */
	double tempoExecucao = (((double) tempoExecucaoFimQuick) - ((double) tempoExecucaoInicioQuick)) / CLOCKS_PER_SEC;

	printf("\nAnalise:\n");
	printf("\nTempo de Execucao: %lf seg  ", tempoExecucao);
	printf("\nNumero de Leituras: %d      ", nLeitura);
	printf("\nNumero de Escritas: %d      ", nEscrita);
	printf("\nNumero de Comparacoes: %d   ", nComparacoes);
	printf("\nFim.\n\n");
}