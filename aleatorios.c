// MAC0328 (2019) Algoritmos em Grafos
// Aluno:      FELIPE CASTRO DE NORONHA
// Número USP: 10737032
// Tarefa:     Tarefa A
// Data:       2019-08-08
//
// Solução baseada em ... <se for o caso> ...
//
// DECLARO QUE SOU O ÚNICO AUTOR E RESPONSÁVEL POR ESTE PROGRAMA. TODAS
// AS PARTES DO PROGRAMA, EXCETO AS QUE FORAM FORNECIDAS PELO PROFESSOR
// OU COPIADAS DO LIVRO OU DAS BIBLIOTECAS DE SEDGEWICK OU ROBERTS,
// FORAM DESENVOLVIDAS POR MIM.  DECLARO TAMBÉM QUE SOU RESPONSÁVEL POR
// TODAS AS EVENTUAIS CÓPIAS DESTE PROGRAMA E QUE NÃO DISTRIBUI NEM
// FACILITEI A DISTRIBUIÇÃO DE CÓPIAS.
//
////////////////////////////////////////////////////////////////////////
//
// Este programa recebe os numeros V e A pela linha de comando e gera um
// grafo aleatorio com V vertices e A arcos. Desse modo, se quisermos,
// por exemplo, criar um grafo com 100 vertices e 5000 arestas, basta
// invocar:
// $ ./aleatorios 100 5000

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "GRAPHlists.h"

// Prototipos de funções auxiliares
void showInDegree (Graph G);
void showOutDegree (Graph G);

// Função que executa a rotina principal. Recebe atraves do parametro
// randN qual função ira usar para gerar o grafo.
void run (int V, int A, int randN) {

    // Terceira validação da entrada. Necessaria pela suposição de
    // GRAPHrand1() que A <= V*(V-1)
    if (randN == 1 && A > V*(V-1)) {
        printf( "\nERROR: A and V must hold A <= V*(V-1)\n");
        exit( EXIT_FAILURE);
    }

    // Quarta validação da entrada. Necessaria pela suposição de
    // GRAPHrand2() que V >= 2 e A <= V*(V-1).
    if (randN == 2 && (V < 2 || A > V*(V-1))) {
        printf( "\nERROR: A and V must hold V >= 2 and A <= V*(V-1)\n");
        exit( EXIT_FAILURE);
    }

    printf("- UTILIZANDO A FUNÇÃO GRAPHrand%d()\n\n", randN);

    // Tempo antes da criação do grafo
    double start = (double) clock () / CLOCKS_PER_SEC;

    // Grafo a ser gerado
    Graph G;
    if (randN == 1) {
        G = GRAPHrand1( V, A);
    }
    else {
        G = GRAPHrand2( V, A);
    }

    // Tempo após a criação do grafo
    double stop = (double) clock () / CLOCKS_PER_SEC;

    // Tempo gasto para a criação do grafo
    double elapsedTime = stop - start;

    printf( "Numero de vertices: %d\n", G->V);
    printf( "Numero de arestas: %d\n", G->A);
    printf( "Tempo decorrido: %fs\n\n", elapsedTime);

    showOutDegree( G);
    showInDegree( G);

    // Imprimindo as listas de adjacência do grafo caso ele tenha
    // menos que 30 vertices
    if (G->V < 30)
        GRAPHshow( G);

    // Limpeza final:
    printf( "\n");
    GRAPHdestroy( G);
}

int main (int na, char* arg[]) {

    int V, A;

    // Primeira validação da entrada
    if (na < 2) {
        printf( "\nERRO: Usage: %s V A\n\n", arg[0]);
        exit( EXIT_FAILURE);
    }

    // Segunda validação da entrada
    if (sscanf( arg[1], "%d", &V) != 1 || V < 0) {
        printf( "\nERROR: V must be a non-negative integer\n\n");
        exit( EXIT_FAILURE);
    }
    if (sscanf( arg[2], "%d", &A) != 1 || A < 0) {
        printf( "\nERROR: A must be a non-negative integer\n\n");
        exit( EXIT_FAILURE);
    }

    // Chamando as rotinas.
    run( V, A, 1);
    run( V, A, 2);

    return EXIT_SUCCESS;
}

// Função que imprime uma tabela com o numero de vertices com grau de
// saida g, com g pertencente ao intervalo [0, m], onde m é o grau de
// saida maximo.
void
showOutDegree( Graph G) {
    int *grauSaida = calloc( G->V, sizeof(int));
    int grauMaximo = 0;

    for (vertex v = 0; v < G->V; v++) {
        int grauV = 0;
        link adj = G->adj[v];
        while (adj != NULL) {
            grauV++;
            adj = adj->next;
        }
        grauSaida[grauV]++;
        if (grauV > grauMaximo) grauMaximo = grauV;
    }

    printf("Numero de vertices com grau de saida igual á [g]:\n");
    for (int g = 0; g <= grauMaximo; g++)
        printf("[%d]: %d\n", g, grauSaida[g]);
    printf("\n");
    free(grauSaida);
}

// Função que imprime uma tabela com o numero de vertices com grau de
// entrada g, com g pertencente ao intervalo [0, m], onde m é o grau de
// entrada maximo.
void
showInDegree( Graph G) {
    int *grauEntrada = calloc( G->V, sizeof(int));
    int *grauEntradaUnitario = calloc( G->V, sizeof(int));

    for (vertex v = 0; v < G->V; v++) {
        link adj = G->adj[v];
        while (adj != NULL) {
            grauEntradaUnitario[adj->w]++;
            adj = adj->next;
        }
    }

    int grauMaximo = 0;
    for (vertex v = 0; v < G->V; v++) {
        grauEntrada[ grauEntradaUnitario[v] ]++;
        if (grauEntradaUnitario[v] > grauMaximo)
            grauMaximo = grauEntradaUnitario[v];
    }

    printf( "Numero de vertices com grau de entrada igual á [g]:\n");
    for (int g = 0; g <= grauMaximo; g++)
        printf( "[%d]: %d\n", g, grauEntrada[g]);
    printf( "\n");
    free( grauEntrada);
    free( grauEntradaUnitario);
}
