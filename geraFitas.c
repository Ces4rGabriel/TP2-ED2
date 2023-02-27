#include "geraFitas.h"

//Junta um numero com uma string
char* concatena(char *str, int num){  
    char *strNum = malloc(sizeof(char) * 10);
    sprintf(strNum, "%d", num);

    char *nomeArq = malloc(sizeof(char) * 10);
    strcpy(nomeArq, str);
    strcat(nomeArq, strNum);
    char* extensao = ".bin";
    strcat(nomeArq, extensao);

    free(strNum);

    return nomeArq;
}

void geradorFitas(int nFitas){
    FILE *arq;
    
    for (int i = 0; i < nFitas; i++)
        arq = fopen(concatena("fita", i), "wb");

}


int main(){
    int nFitas;
    nFitas = 20;
    geradorFitas(nFitas);

    return 0;
}

