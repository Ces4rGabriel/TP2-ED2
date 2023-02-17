#include <stdio.h>
#include <stdlib.h>
#include "Fitas\geraFitas.h"

void gerador(){
    FILE *arq;
    arq = fopen("entrada", "wb");
    int n = 1000000;
    int z = 0;

    for (int i = 0; i < n; i++){
        fwrite(&i, sizeof(int), 1, arq);  
        fwrite(&z, sizeof(int), 1, arq);
    }
    
    fclose(arq);
}

int main(){
    gerador();
    return 0;
}