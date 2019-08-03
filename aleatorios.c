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

int main (int na, char* arg[]) {

    int V, A;

    // caso seja igual á 1, usa a função GRAPHrand1(). Caso seja 2, usa
    // a função GRAPHrand2()
    int randN = 1;

    // primeira validação da entrada
    if (na < 2) {
        printf( "\nUsage: %s V A\n\n", arg[0]);
        exit( EXIT_FAILURE);
    }

    // seguna validação da entrada
    if (sscanf( arg[1], "%d", &V) != 1 || V < 0) {
        printf( "\nV must be a non-negative integer\n\n");
        exit( EXIT_FAILURE);
    }
    if (sscanf( arg[2], "%d", &A) != 1 || A < 0) {
        printf( "\nA must be a non-negative integer\n\n");
        exit( EXIT_FAILURE);
    }

    // terceira validação da entrada. Necessaria pela suposição de
    // GRAPHrand1() que A <= V*(V-1)
    if (randN == 1 && A > V*(V-1)) {
        printf( "\nA and V must hold A <= V*(V-1)\n\n");
        exit( EXIT_FAILURE);
    }

    // quarta validação da entrada. Necessaria pela suposição de
    // GRAPHrand2() que V >= 2 e A <= V*(V-1).
    if (randN == 2 && (V < 2 || A > V*(V-1))) {
        printf( "\nA and V must hold V >= 2 and A <= V*(V-1)\n\n");
        exit( EXIT_FAILURE);
    }

    // tempo antes da criação do grafo
    double start = (double) clock () / CLOCKS_PER_SEC;

    // grafo a ser gerado
    Graph G;
    if (randN == 1) {
        G = GRAPHrand1( V, A);
    }
    else {
        G = GRAPHrand2( V, A);
    }

    // tempos após a criação do grafo
    double stop = (double) clock () / CLOCKS_PER_SEC;

    // tempo gasto para a criação do grafo
    double elapsedTime = stop - start;

    printf( "Numero de vertices: %d\n", G->V);
    printf( "Numero de arestas: %f\n", G->A);
    printf( "Tempo decorrido: %fs\n", elapsedTime);

    // exibindo na tela uma tabela com o número de vértices que têm grau
    // de saída g, com g pertecente ao intervalo [0, m], onde m é o grau
    // de saida maximo.


    // tabela analoga á anterior, mas para o grau de entrada

    // imprimindo as listas de adjacência do grafo caso ele tenha menos
    // que 30 vertices
    if (G->V < 30) {
        GRAPHshow( G);
    }

    // limpeza final:
    printf( "\n");
    GRAPHdestroy( G);
    return EXIT_SUCCESS;
}
