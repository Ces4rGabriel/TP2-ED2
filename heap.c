#include "heap.h"
#include "arquivo.h"

int numComp = 0;

//verifica se o vetor de alunos está com todos elementos marcados. Caso ele encontre um elemento não marcado, ele já retorna para continuar com as verificações
int verificaMarcado(TipoAreaS *A){

    for(int i = 0; i < A->ne; i++){
        if(A->area->marcado != 1)
            return 1;
    }
    
    return 0;
}


// Faz as comparações e substituições de posições necessárias para gerar o heap
void heapfy(TipoAluno *vAlunos, int esq, int dir) {
    int i = esq;
    int j = i * 2 + 1;
    TipoAluno aux = vAlunos[i];

    while (j <= dir) {
        if (j < dir && ((vAlunos[j].aluno.nota > vAlunos[j + 1].aluno.nota || vAlunos[j].marcado == 1) && vAlunos[j + 1].marcado != 1)) {
            j = j + 1;  // j recebe o outro filho de i
            numComp += 1;  
        }
        if (j < dir && vAlunos[j].aluno.nota > vAlunos[j + 1].aluno.nota && vAlunos[j].marcado == 1 && vAlunos[j + 1].marcado == 1) {
            j = j + 1;
            numComp += 1;  
        }
        if (aux.aluno.nota <= vAlunos[j].aluno.nota && aux.marcado != 1) {
            numComp += 1;
            break;
        }
        if (aux.aluno.nota >= vAlunos[j].aluno.nota && vAlunos[j].marcado == 1 && aux.marcado != 1) {
            numComp += 1;
            break;
        }

        vAlunos[i] = vAlunos[j];

        i = j;
        j = i * 2 + 1;
    }
    vAlunos[i] = aux;
}

// Constroi a heap a partir de um vetor v[TAMFITAINT], e retorna o valor de comparações feitas em HEAP_REFAZ.
int heapsort(TipoAluno *vAlunos, int tam) {
    numComp = 0;
    int esq = (tam / 2) - 1;
    while (esq >= 0) {
        heapfy(vAlunos, esq, tam - 1);
        esq--;
    }
    return numComp;
}

// Função que remove o primeiro elemento do vetor, e verifica o valor de .maior em cada posição
int removeArea(TipoAluno *vAlunos, int *tam, Analise *comp) {
    if (*tam == 0) {
        return 0;
    } else {
        vAlunos[0] = vAlunos[*tam - 1];
        *tam -= 1;
    }
    // Verifica se todos elemntos foram atribuidos como maior igual a true.
    int normal = 1;
    for (int i = 0; i < *tam; i++) {
        if (vAlunos[i].marcado == 0) {
            normal = 0;
            break;
        }
    }
    // Se todos elementos foram atribuidos como maior igual a true, então é necessário mudar de fita/bloco.
    if (normal) {
        for (int i = 0; i < *tam; i++) {
            if (vAlunos[i].aluno.nota != -1) {
                vAlunos[i].marcado = 0;
            }
        }
    }

    comp->numComparacoes += heapsort(vAlunos, *tam);

    return normal;
}

// Função que substitui o primeiro elemento do vetor, e respeitando as regras da seleção por substituição, atribui um valor
// 0 ou true, para caso o valor (nota) do elemento a ser substituido seja maior que o elemento que o substituirá. 
int adicionaArea(TipoAluno *vAlunos, int *tam, TipoRegistro elem, Analise *comp) {
    if (*tam == 0) {
        return 0;
    } else {
        if (vAlunos[0].aluno.nota > elem.nota) {
            vAlunos[0].aluno = elem;
            vAlunos[0].marcado = 1;
        } else {
            vAlunos[0].aluno = elem;
            vAlunos[0].marcado = 0;
        }
    }
    // Verifica se todos elemntos foram atribuidos como maior igual a true.
    int normal = 1;
    for (int i = 0; i < *tam; i++) {
        if (vAlunos[i].marcado == 0) {
            normal = 0;
            break;
        }
    }
    // Se todos elementos foram atribuidos como maior igual a true, então é necessário mudar de fita/bloco.
    if (normal) {
        for (int i = 0; i < *tam; i++) {
            if ((int)vAlunos[i].aluno.nota != -1) {
                vAlunos[i].marcado = 0;
            }
        }
    }
    
    comp->numComparacoes += heapsort(vAlunos, *tam);

    return normal;
}
