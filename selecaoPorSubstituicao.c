#include "selecaoPorSubstituicao.h"

int verificaMarcado(TipoItem *Area, int TamArea){
    int c = 0;
    for (int i = 0; i < TamArea; i++)
        if(Area[i].marcado == 1)
            c++;

    return c;
}

void adicionaNaFita(TipoItem Item, int nArquivo, TipoItem *UltSair){
    FILE *arq;
    arq = fopen(concatenar("fita", nArquivo), "wb");
    UltSair->chave = Item.chave;
    //escrever na fita aberta
    fwrite(&Item, sizeof(TipoItem), 1, arq);
}

TipoRegistro* inicializa(int TamArea){
    TipoRegistro *reg;
    reg->Area = malloc(sizeof(TipoItem) * TamArea);
    reg->ne = 0;

    return reg;
}

TipoItem retiraArea(TipoRegistro *reg){
    return reg->Area[0];
}

void adicionaArea(TipoRegistro *reg, TipoItem Item, TipoItem UltSair){
    if(Item.chave < UltSair.chave)
        Item.marcado = 1;
    else
        Item.marcado = 0;

    reg->Area[0] = Item;
    reg->ne++;
}

TipoItem retiraArquivo(FILE *arq){
    TipoItem Item;
    int chave, marcado;
    fread(&chave, sizeof(int), 1, arq);
    fread(&marcado, sizeof(int), 1, arq);

    Item.chave = chave;
    Item.marcado = marcado;

    return Item;
}

int selecaoPorSubstituicao(int TamArea){
    int i = 0, j = 1;
    int nArquivos = 20;
    TipoItem Aux, Aux2;
    TipoItem UltElem;
    TipoRegistro *reg = inicializa(TamArea);
    FILE *arq;
    arq = fopen("entrada", "rb");

    //Inicia Ultimo Elemento como -1
    UltElem.chave = -1;
    UltElem.marcado = 0;

    
    while(j < nArquivos)
    {
        while(!verificaMarcado(reg->Area, TamArea))
        {
            while(i < TamArea){
                Aux = retiraArquivo(arq);
                adicionaArea(reg, Aux, UltElem);
                i++;
            }    
            Aux2 = retiraArea(reg);
            UltElem = Aux2;
            Aux2.origem = j;
            adicionaNaFita(Aux2, j, &UltElem);
        }
        j++;
    }
    
    free(reg->Area);
}