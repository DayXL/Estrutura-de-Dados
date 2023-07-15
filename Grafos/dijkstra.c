#include <stdio.h>
#include <stdlib.h>

struct ligacoes {
    int no1;
    int no2;
    int pesoArestas;
};

int preencherVetor(struct ligacoes* vetor){
    FILE *arquivo;
    int i = 0;

    arquivo = fopen("robot.tgf", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");

        return 1;

    }

   
    while (fscanf(arquivo, "%d %d %d", &vetor[i].no1, &vetor[i].no2, &vetor[i].pesoArestas) == 3) {
        i++;
    }

    fclose(arquivo);
    
    return 0;
}

int main(void) {
    struct ligacoes vetor[48]; 

    int aux = preencherVetor(vetor);

    if (aux) {
        return 1;
    }

    for (int i = 0; i < 48; i++) {
        printf("Linha %d: no1 = %d, no2 = %d, pesoArestas = %d\n", i + 1, vetor[i].no1, vetor[i].no2, vetor[i].pesoArestas);
    }

    return 0;
}