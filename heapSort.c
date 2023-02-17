#include "heapSort.h"

void heapify(TipoItem *Area, int n, int i) {
    int maior = i;
    int esq = 2*i + 1;
    int dir = 2*i + 2;

    if (esq < n && Area[esq].chave > Area[maior].chave)
        maior = esq;

    if (dir < n && Area[dir].chave > Area[maior].chave)
        maior = dir;

    if (maior != i) {
        int aux;
        aux = Area[i].chave;
        Area[i] = Area[maior];
        Area[maior].chave = aux;
        heapify(Area, n, maior);
    }
}

void heapSort(TipoRegistro *Reg, int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(Reg->Area, n, i);

    for (int i=n-1; i>=0; i--) {
        TipoRegistro aux = Reg[0];
        Reg[0] = Reg[i];
        Reg[i] = aux;
        heapify(Reg->Area, i, 0);
    }
}