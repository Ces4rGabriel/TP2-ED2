#include "heap.h"

#include <stdbool.h>

#include "arquivo.h"

int numComp = 0;

// Faz as comparações e substituições de posições necessárias para gerar o heap
void HEAP_REFAZ(Estrutura *v, int esq, int dir) {
    int i = esq;
    int j = i * 2 + 1;
    Estrutura aux = v[i];

    while (j <= dir) {
        if (j < dir && ((v[j].aluno.nota > v[j + 1].aluno.nota || v[j].maior == true) && v[j + 1].maior != true)) {
            j = j + 1;  // j recebe o outro filho de i
            numComp += 1;  
        }
        if (j < dir && v[j].aluno.nota > v[j + 1].aluno.nota && v[j].maior == true && v[j + 1].maior == true) {
            j = j + 1;
            numComp += 1;  
        }
        if (aux.aluno.nota <= v[j].aluno.nota && aux.maior != true) {
            numComp += 1;
            break;
        }
        if (aux.aluno.nota >= v[j].aluno.nota && v[j].maior == true && aux.maior != true) {
            numComp += 1;
            break;
        }

        v[i] = v[j];

        i = j;
        j = i * 2 + 1;
    }
    v[i] = aux;
}

// Constroi a heap a partir de um vetor v[TAMFITAINT], e retorna o valor de comparações feitas em HEAP_REFAZ.
int HEAP_CONSTROI(Estrutura *v, int tam) {
    numComp = 0;
    int esq = (tam / 2) - 1;
    while (esq >= 0) {
        HEAP_REFAZ(v, esq, tam - 1);
        esq--;
    }
    return numComp;
}

// Função que remove o primeiro elemento do vetor, e verifica o valor de .maior em cada posição
bool remove_No(Estrutura *v, int *tam, Analise *comp) {
    if (*tam == 0) {
        return false;
    } else {
        v[0] = v[*tam - 1];
        *tam -= 1;
    }
    // Verifica se todos elemntos foram atribuidos como maior igual a true.
    bool normal = true;
    for (int i = 0; i < *tam; i++) {
        if (v[i].maior == false) {
            normal = false;
            break;
        }
    }
    // Se todos elementos foram atribuidos como maior igual a true, então é necessário mudar de fita/bloco.
    if (normal) {
        for (int i = 0; i < *tam; i++) {
            if (v[i].aluno.nota != -1) {
                v[i].maior = false;
            }
        }
    }

    comp->numComparacoes += HEAP_CONSTROI(v, *tam);

    return normal;
}

// Função que substitui o primeiro elemento do vetor, e respeitando as regras da seleção por substituição, atribui um valor
// false ou true, para caso o valor (nota) do elemento a ser substituido seja maior que o elemento que o substituirá. 
bool substitui(Estrutura *v, int *tam, TipoRegistro elem, Analise *comp) {
    if (*tam == 0) {
        return false;
    } else {
        if (v[0].aluno.nota > elem.nota) {
            v[0].aluno = elem;
            v[0].maior = true;
        } else {
            v[0].aluno = elem;
            v[0].maior = false;
        }
    }
    // Verifica se todos elemntos foram atribuidos como maior igual a true.
    bool normal = true;
    for (int i = 0; i < *tam; i++) {
        if (v[i].maior == false) {
            normal = false;
            break;
        }
    }
    // Se todos elementos foram atribuidos como maior igual a true, então é necessário mudar de fita/bloco.
    if (normal) {
        for (int i = 0; i < *tam; i++) {
            if ((int)v[i].aluno.nota != -1) {
                v[i].maior = false;
            }
        }
    }
    
    comp->numComparacoes += HEAP_CONSTROI(v, *tam);

    return normal;
}
