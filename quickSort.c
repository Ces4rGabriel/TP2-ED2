#include "quickSort.h"

int main(int argc, char *argv[]){
    ArqLi = fopen("teste.dat", "wb");
    if(ArqLi == NULL){printf("Arquivo não pode ser aberto\n"); exit(1);}
}

void quickSortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir){
    int i, j;
    TipoArea Area;

    if(Dir - Esq < 1) return;

    FAVazia(&Area);
    Particao(ArqLi, ArqEi, ArqLEs, Area, Esq, Dir, &i, &j);

    if(i - Esq < Dir - j){ //Ordene o primeiro subarquivo menor"
        quickSortExterno(ArqLi, ArqEi, ArqLEs, j, Dir);
        quickSortExterno(ArqLi, ArqEi, ArqLEs, Esq, i);  
    }
}

void LeSup(FILE **ArqLEs, TipoRegistro *UltLido, int *Ls, short *OndeLer){
    fseek(*ArqLEs, (*Ls - 1) * sizeof(TipoRegistro), SEEK_SET);
    fread(UltLido, sizeof(TipoRegistro), 1, *ArqLEs);
    (*Ls)--;
    *OndeLer = 0;
}

void LeInf(FILE **ArqLi, TipoRegistro *UltLido, int *Li, short *OndeLer){
    fread(UltLido, sizeof(TipoRegistro), 1, *ArqLi);
    (*Li)++;
    *OndeLer = 1;
}

void inserirArea(TipoArea *Area, TipoRegistro *UltLido, int *NRArea){
    //Insere UltLido de forma ordenada na area
    insereItem(*UltLido, Area);
    (*NRArea) = ObterNumCelOcupadas(Area);
}

void escreveMax(FILE **ArqLEs, TipoRegistro R, int *Es){
    fseek(*ArqLEs, (*Es - 1) * sizeof(TipoRegistro), SEEK_SET);
    fwrite(&R, sizeof(TipoRegistro), 1, *ArqLEs);
    (*Es)--;
}

void escreveMin(FILE **ArqEi, TipoRegistro R, int *Ei){
    fwrite(&R, sizeof(TipoRegistro), 1, *ArqEi);
    (*Ei)++;
}

void retiraMax(TipoArea *Area, TipoRegistro R, int *NRArea){
    retiraUltimo(Area, R);
    *NRArea = ObterNumCelOcupadas(Area);
}

void retiraMin(TipoArea *Area, TipoRegistro R, int *NRArea){
    retiraPrimeiro(Area, R);
    *NRArea = ObterNumCelOcupadas(Area);
}

void particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea Area, int Esq, int Dir, int *i, int *j){
    int INT_MAX = 10000, INT_MIN = -10000;
    int TAMAREA = 10;
    int Ls = Dir, Es = Dir, Li = Esq, Ei = Esq, NRArea = 0, Linf = INT_MAX, Lsup = INT_MIN;
    short OndeLer = 1;
    TipoRegistro UltLido, R;

    fseek(*ArqLi, (Li - 1) * sizeof(TipoRegistro), SEEK_SET);
    fseek(*ArqEi, (Ei - 1) * sizeof(TipoRegistro), SEEK_SET);
    *i = Esq - 1;
    *j = Dir + 1;

    while (Ls >= Li)
    {
        if(NRArea < TAMAREA - 1){
            if(OndeLer){
                LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
            }else{
                LeInf(ArqLi, &UltLido, &Li, &OndeLer);
            }
            inserirArea(&Area, &UltLido, &NRArea);
            continue;
        }
        if(Ls == Es)
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
        else if(Li == Ei)
            LeInf(ArqLi, &UltLido, &Li, &OndeLer);
        else if(OndeLer)
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
        else
            LeInf(ArqLi, &UltLido, &Li, &OndeLer);

        if(UltLido.Chave > Lsup){
            *j = Es;
            escreveMax(ArqLEs, UltLido, &Es);
            continue;
        }
        if(UltLido.Chave < Linf){
            *i = Ei;
            escreveMin(ArqEi, UltLido, &Ei);
            continue;
        }

        inserirArea(&Area, &UltLido, &NRArea);
        if(Ei - Esq < Dir - Es){
            retiraMin(&Area, &R, &NRArea);
            escreveMin(ArqEi, R, &Ei);
            Linf = R.Chave;
        }else {
            retiraMax(&Area, &R, &NRArea);
            escreveMax(ArqLEs, R, &Es);
            Lsup = R.Chave;
        }
    }
    while (Ei <= Es)
    {
        retiraMin(&Area, &R, &NRArea);
        escreveMin(ArqEi, R, &Ei);
    }
}
