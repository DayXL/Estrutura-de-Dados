#include <stdio.h>
#include <stdlib.h>

typedef struct ligacoes {
    int no1;
    int no2;
    int pesoArestas;
} Ligacoes;

typedef struct no {
    int valor;
    int visitado;
    int pesoAresta;
    struct no *proximo;
} No;

typedef struct vetor {
    int quantArestas;
    int quantNos; 
    No **lista;
} Vetor;

void dijkstra(int origem, Ligacoes grafo) {

}

void dotGrafo(FILE* file, Vetor* vetor, int tam) {
    fprintf(file, "  main_table [shape=record, label=\"");

    for (int i = 0; i < tam; i++) {
        if (i > 0) {
            fprintf(file, "|");
        }

        if (vetor->lista[i] == NULL) {
            fprintf(file, "<slot%d> NULL", i);
        } else {
            fprintf(file, "<slot%d> %p", i, (void*)vetor->lista[i]);
        }
    }

    fprintf(file, "\"];\n");

    for (int i = 0; i < vetor->quantNos; i++) {
        No* no = vetor->lista[i];

        if (no != NULL) {
            fprintf(file, "  \"%p\" [shape=record, label=\"{valor: %d|visitado: %d|pesoAresta: %d|proximo: %p}\"];\n",
                    (void*)no, no->valor, no->visitado, no->pesoAresta, (void*)no->proximo);
            fprintf(file, "  main_table:slot%d -> \"%p\";\n", i, (void*)no);

            while (no->proximo != NULL) {
                fprintf(file, "  \"%p\" -> \"%p\" [style=dotted];\n", (void*)no, (void*)no->proximo);
                no = no->proximo;
                fprintf(file, "  \"%p\" [shape=record, label=\"{valor: %d|visitado: %d|pesoAresta: %d|proximo: %p}\"];\n",
                        (void*)no, no->valor, no->visitado, no->pesoAresta, (void*)no->proximo);
            }
        }
    }
}

int preencherVetor(Ligacoes* vetor){
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
    Ligacoes vetor[48]; 

    int aux = preencherVetor(vetor);

    if (aux) {
        return 1;
    }

    for (int i = 0; i < 48; i++) {
        printf("Linha %d: no1 = %d, no2 = %d, pesoArestas = %d\n", i + 1, vetor[i].no1, vetor[i].no2, vetor[i].pesoArestas);
    }

    Vetor grafo;

    grafo.quantArestas =  sizeof(vetor) / sizeof(Ligacoes);
    grafo.lista = malloc(sizeof(No*)*(vetor[47].no1));

    for (int i = 0; i <  vetor[47].no1; i++) {
        grafo.lista[i] = NULL;
    }

    for (int i = 0; i < grafo.quantArestas; i++) {
        No* novoNo = malloc(sizeof(No));
        novoNo->valor = vetor[i].no2;
        novoNo->pesoAresta = vetor[i].pesoArestas;
        novoNo->visitado = 0;
        novoNo->proximo = NULL;

        int indice = vetor[i].no1 - 1; // Subtrai 1 porque o índice do vetor começa em 0
        No* listaAtual = grafo.lista[indice];

        if (listaAtual == NULL) {
            grafo.lista[indice] = novoNo;
        } else {
            while (listaAtual->proximo != NULL) {
                listaAtual = listaAtual->proximo;
            }
            listaAtual->proximo = novoNo;
        }
    }

    grafo.quantNos = 0;

    for (int i = 0; i < vetor[47].no1; i++) {
        if (grafo.lista[i] != NULL) {
            grafo.quantNos++;
        }
    }

    printf("%d", grafo.quantNos);

    FILE *dot_file = fopen("grafo.dot", "w");
    if (dot_file == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return 1;
    }

    fprintf(dot_file, "digraph G {\n node [shape=record, height=0.6, width=1.5];\n edge [arrowhead=vee, arrowsize=0.8];\n");
    dotGrafo(dot_file, &grafo, vetor[47].no1);
    fprintf(dot_file, "}\n");

    fclose(dot_file);

    for (int i = 0; i < grafo.quantNos; i++) {
        No* listaAtual = grafo.lista[i];
        while (listaAtual != NULL) {
            No* proximo = listaAtual->proximo;
            free(listaAtual);
            listaAtual = proximo;
        }
    }

    free(grafo.lista);


    return 0;
}