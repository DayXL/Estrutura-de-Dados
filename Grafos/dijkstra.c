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

typedef struct auxVisitas {
    int valor;
    int visitado;
} AuxVisitas;

typedef struct vetor {
    int quantArestas;
    int quantNos; 
    No **lista;
} Vetor;

typedef struct memoria {
    int tam;
    int **lista;
} Memoria;

void liberarMemoriaGrafo(Vetor* grafo) {
    for (int i = 0; i < grafo->quantNos; i++) {
        No* listaAtual = grafo->lista[i];
        while (listaAtual != NULL) {
            No* proximo = listaAtual->proximo;
            free(listaAtual);
            listaAtual = proximo;
        }
    }

    free(grafo->lista);
};

void removeFila(Memoria* memoria) {
    Memoria nova;
    nova.tam = memoria->tam - 1;
    nova.lista = malloc(sizeof(int*) * nova.tam);

    for (int i = 0; i < nova.tam; i++) {
        nova.lista[i] = memoria->lista[i + 1];

    }

    free(memoria->lista);

    (*memoria) = nova;

}

void adicionarFila(Memoria* memoria, int* novoValor) {
    Memoria nova;
    nova.tam = memoria->tam + 1;
    nova.lista = malloc(sizeof(int*) * nova.tam);

    for (int i = 0; i < nova.tam - 1; i++) {
        nova.lista[i] = memoria->lista[i];
    }

    nova.lista[nova.tam - 1] = novoValor;

    free(memoria->lista);
    (*memoria) = nova;

}

int naoVisitado(AuxVisitas* visitados, int i, int valor) {
    for (int j = 0; j < i; j ++) {
        if (visitados[j].valor == valor) {
            return visitados[j].visitado;
        }
    }

    return 0;
}

void dijkstra(int origem, Vetor* grafo, int tam) {
    Memoria memoria;
    memoria.tam = 1;
    memoria.lista = malloc(sizeof(int*)*(tam));

    memoria.lista[0] = &origem;

    (*grafo).lista[origem - 1]->visitado = origem;

    AuxVisitas numVisitados[(*grafo).quantNos + 1];
    numVisitados[0].valor = origem;
    numVisitados[0].visitado = origem;
    int i = 1;

    while (memoria.tam) {
        int* w = memoria.lista[0];
        removeFila(&memoria);
        
        No* list;

        for (list = grafo->lista[(*w) - 1]; list != NULL; list = list->proximo){

            int visitado = naoVisitado(numVisitados, i, list->valor);

            if (!visitado) { 
                if (list->visitado == 0) {
                    adicionarFila(&memoria, &list->valor);
                    list->visitado = *w;
                    numVisitados[i].valor = list->valor;
                    numVisitados[i].visitado = *w;
                    i = i + 1;
                    
                }
            }

            else {
                list->visitado = visitado;
            }
        }

    }
    
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

    for (int i = 0; i < tam; i++) {
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

void exibeNo(No *valorAdic) {
    while (valorAdic != NULL){
        printf("%d", valorAdic->valor);
        printf(" --> ");
        valorAdic = valorAdic->proximo;
    }
}


void exibeGrafo(Vetor *vetor, int tam) {
    for (int i = 0; i < tam; i++){
        printf("Nó: %d", i + 1);
        printf(" --> ");
        exibeNo(vetor->lista[i]);
        printf("\n");
    }
}

int main(void) {
    Ligacoes vetor[48]; 

    int aux = preencherVetor(vetor);

    if (aux) {
        return 1;
    }

    // for (int i = 0; i < 48; i++) {
    //     printf("Linha %d: no1 = %d, no2 = %d, pesoArestas = %d\n", i + 1, vetor[i].no1, vetor[i].no2, vetor[i].pesoArestas);
    // }

    Vetor grafo;

    grafo.quantArestas =  (sizeof(vetor) / sizeof(Ligacoes));
    
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

    //exibeGrafo(&grafo, vetor[47].no1);

    grafo.quantNos = 0;

    for (int i = 0; i < vetor[47].no1; i++) {
        if (grafo.lista[i] != NULL) {
            grafo.quantNos++;
        }
    }

    dijkstra(11, &grafo,vetor[47].no1 );

    FILE *dot_file = fopen("grafo.dot", "w");
    if (dot_file == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return 1;
    }

    fprintf(dot_file, "digraph G {\n node [shape=record, height=0.6, width=1.5];\n edge [arrowhead=vee, arrowsize=0.8];\n");
    dotGrafo(dot_file, &grafo, vetor[47].no1);
    fprintf(dot_file, "}\n");

    fclose(dot_file);

    liberarMemoriaGrafo(&grafo);

    return 0;
}