// MAC0328 (2019) Algoritmos em Grafos
// Aluno1:      FELIPE CASTRO DE NORONHA
// Número USP1: 10737032
// Aluno2:      CAROLINA SENRA MARQUES
// Número USP2: 10737101
// Tarefa:      Tarefa G
// Data:        2019-09-30
//
// DECLARO QUE MINHA DUPLA É A UNICA RESPONSÁVEL POR ESTE PROGRAMA.
// TODAS AS PARTES DO PROGRAMA, EXCETO AS QUE FORAM FORNECIDAS PELO
// PROFESSOR OU COPIADAS DO LIVRO OU DAS BIBLIOTECAS DE SEDGEWICK OU
// ROBERTS, FORAM DESENVOLVIDAS POR MIM.  DECLARO TAMBÉM QUE SOU
// RESPONSÁVEL POR TODAS AS EVENTUAIS CÓPIAS DESTE PROGRAMA E QUE NÃO
// DISTRIBUI NEM FACILITEI A DISTRIBUIÇÃO DE CÓPIAS.
//
////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "GRAPHlists.h"
#include "PQ.h"

// Funcao auxiliar que confere se um vertice v pode receber uma cor c
bool
is_safe( UGraph G, int c, vertex v, int *colors);

// Funcao auxiliar que confere se todos os vertices ja receberam uma
// cor conferindo se ainda existe algum -1 no vetor colors[]
bool
is_complete( UGraph G, int *colors);

// Retirado de GRAPhlists.c com uma leve alteracao feita
// Esta funcao auxiliar sorteia aleatoriamente um vertice de G
vertex
rand_v( UGraph G);

// Funcao auxiliar de heuristica_1 que e' responsavel por fazer o
// backtracking testando todas as combinacoes possiveis de cores ate
// encontrar uma combinacao valida
bool
backtracking( UGraph G, vertex v, int *colors);

// Funcao auxiliar que imprime a cor associada a cada vertice
void
print_colors( UGraph G, int *colors);

// Funcao que procura uma coloracao valida para o grafo G sorteando um
// vertice v e guardando o menor valor de cor valido em colors[v] e
// repetindo este processo ate que todos os vertices tenham uma cor
int
heuristica_1( UGraph G, int *colors) {
    int c = 0, max = 0;
    for (int v = 0; v < G->V; v++)
        colors[v] = -1;
    while (!is_complete( G, colors)){
        vertex v = rand_v( G);
        if (colors[v] == -1){
            while (!is_safe( G, c, v, colors)) c++;
            colors[v] = c;
        }
    }
    for (int v = 0; v < G-> V; v++)
        if (colors[v] > max) max = colors[v];
    return max + 1;
}

// Funcao que procura uma coloracao valida para o grafo G usando
// forca bruta, ou seja, testa todas as combinacoes de cores possiveis
// ate encontrar uma coloracao valida
int
heuristica_2 (UGraph G, int *colors) {
    int max = 0;
    for (int v = 0; v < G->V; v++)
        colors[v] = -1;
    backtracking( G, 0, colors);
    for (int v = 0; v < G-> V; v++)
        if (colors[v] > max) max = colors[v];
    return max + 1;
}

// Implementação da heuristica 'largest first'. Nesse metodo, a cada
// iteração, se pega o vertice com o maior grau que ainda não possui
// uma cor, e então, tenta associar a cor com menor valor possivel para
// ele.
int
heuristica_4( UGraph G, int *colors) {
    int *inv_deg = calloc( G->V, sizeof(int));
    memset( colors, -1, G->V*sizeof(int));

    for (vertex v  = 0; v < G->V; v++)
        inv_deg[v] = G->V;
    for (vertex v  = 0; v < G->V; v++)
        for (link a = G->adj[v]; a != NULL; a = a->next)
            inv_deg[a->w]--;

    PQinit( G->V);
    for (vertex v  = 0; v < G->V; v++)
        PQinsert( v, inv_deg);

    int *used = calloc( G->V, sizeof(int));
    int lowest_color;
    while (!PQempty()) {
        vertex v = PQdelmin(inv_deg);
        memset( used, 0, G->V*sizeof(int));
        for (link a = G->adj[v]; a != NULL; a = a->next) {
            vertex w = a->w;
            if (colors[w] != -1)
                used[colors[w]] = 1;
        }
        lowest_color = G->V;
        for (int c = 0; c < G->V; c++)
            if (!used[c]) {
                lowest_color = c;
                break;
            }
        colors[v] = lowest_color;
    }
    lowest_color = -1;
    for (vertex v = 0; v < G->V; v++)
        if (lowest_color < colors[v])
            lowest_color = colors[v];

    free(inv_deg);
    free(used);
    PQfree();
    return lowest_color+1;
}

// Implementação da heuristica de Brelaz (DSatur). Nesse metodo,
// definimos a saturação de uma vertice v como o numero de cores
// diferentes de seus vizinhos. Com isso, a cada iteração, pegamos o
// vertice com maior saturação, usando o seu grau como criterio de
// desempate (quero o vertice com maior saturação ou com maior grau),
// que ainda não foi colorido e definimos a cor com menor valor possivel
// a ele.
int
heuristica_5( UGraph G, int *colors) {
    int *degree = calloc( G->V, sizeof(int));
    int *saturation = calloc( G->V, sizeof(int));
    int *used = calloc(G->V, sizeof(int));
    memset( colors, -1, G->V*sizeof(int));

    for (vertex v  = 0; v < G->V; v++)
        for (link a = G->adj[v]; a != NULL; a = a->next)
            degree[a->w]++;

    for (int it = 0; it < G->V; it++) {
        vertex now = -1;
        for (vertex v = 0; v < G->V; v++) {
            if (colors[v] != -1) continue;
            if (now == -1) {
                now = v;
                continue;
            }

            if (saturation[v] > saturation[now])
                now = v;
            else if (saturation[v] == saturation[now] &&
                degree[v] > degree[now])
                now = v;
        }

        memset( used, 0, G->V*sizeof(int));
        for (link a = G->adj[now]; a != NULL; a = a->next)
            if (colors[a->w] != -1)
                used[colors[a->w]] = 1;
        int lowest_color = G->V;
        for (int c = 0; c < G->V; c++)
            if (!used[c]) {
                lowest_color = c;
                break;
            }

        colors[now] = lowest_color;
        // Atualizando saturação
        for (link a = G->adj[now]; a != NULL; a = a->next) {
            vertex w = a->w;
            memset( used, 0, G->V*sizeof(int));
            for (link b = G->adj[w]; b != NULL; b = b->next)
                if (colors[b->w] != -1)
                    used[colors[b->w]] = 1;
            int color_count = 0;
            for (int i = 0; i < G->V; i++)
                color_count += used[i];
            saturation[w] = color_count;
        }
    }
    int lowest_color = -1;
    for (vertex v = 0; v < G->V; v++)
        if (lowest_color < colors[v])
            lowest_color = colors[v];

    free(used);
    free(degree);
    free(saturation);
    return lowest_color+1;
}

// Função que executa a rotina principal.
void
run( int V, int E, int S) {

    // Terceira validação da entrada. Necessaria pela suposição de
    // GRAPHrand1() que A <= (V*(V-1))/2
    if (E > (V*(V-1))/2) {
        printf( "\nERROR: E and V must hold A <= (V*(V-1))/2\n");
        exit( EXIT_FAILURE);
    }

    // Para testes
    int DEBUG = 0;

    // Semente
    srand( S);
    // Grafo a ser gerado
    UGraph G;
    G = UGRAPHrand1( V, E);

    if (DEBUG)
        GRAPHshow( G);

    // Variaveis usadas para medição d tempo
    double start, stop, elapsedTime;

    // Armazena as colorações
    int *colors = malloc( G->V * sizeof(int));

    // Array de funções que armazena as heuristicas
    int (*h[5])(UGraph, int*);
    h[0] = heuristica_1;
    h[1] = heuristica_2;
    h[2] = UGRAPHseqColoring;
    h[3] = heuristica_4;
    h[4] = heuristica_5;

    for (int i = 0; i < 5; i++) {

        start = (double) clock () / CLOCKS_PER_SEC;
        int n_colors = h[i]( G, colors);
        stop = (double) clock () / CLOCKS_PER_SEC;

        elapsedTime = stop - start;
        printf( "Numero de cores usado pela heuristica %d: %d\n", i+1,
            n_colors);
        printf( "Tempo gasto na heuristica %d: %.6fs\n\n", i+1,
            elapsedTime);

        if (DEBUG)
            print_colors(G, colors);
    }

    // Limpeza final:
    free(colors);
    GRAPHdestroy( G);
}

int
main (int argc, char* arg[]) {

    int V, E, S;

    // Primeira validação da entrada
    if (argc < 4) {
        printf( "\nERRO: Usage: %s V(# vertices) E(# edges) S(seed)\
            \n\n", arg[0]);
        exit( EXIT_FAILURE);
    }
    // Segunda validação da entrada
    if (sscanf( arg[1], "%d", &V) != 1 || V < 0) {
        printf( "\nERROR: V must be a non-negative integer\n\n");
        exit( EXIT_FAILURE);
    }
    if (sscanf( arg[2], "%d", &E) != 1 || E < 0) {
        printf( "\nERROR: E must be a non-negative integer\n\n");
        exit( EXIT_FAILURE);
    }
    if (sscanf( arg[3], "%d", &S) != 1 || S < 0) {
        printf( "\nERROR: S must be a non-negative integer\n\n");
        exit( EXIT_FAILURE);
    }

    // Chamando a rotina
    run( V, E, S);

    return (EXIT_SUCCESS);
}

bool
is_safe( UGraph G, int c, vertex v, int *colors) {
    for (link a = G->adj[v]; a != NULL; a = a->next)
        if (colors[a->w] == c) return false;
    return true;
}

bool
is_complete( UGraph G, int *colors) {
    for (int i = 0; i < G->V; i++)
        if (colors[i] == -1)
            return false;
    return true;
}

vertex
rand_v( UGraph G) {
   double r = rand() / (RAND_MAX + 1.0);
   return r * G->V;
}

bool
backtracking( UGraph G, vertex v, int *colors) {
    if (v == G->V) return true;
    for (int c = 0; c < G->V; c++) {
        if (is_safe( G, c, v, colors)){
            colors[v] = c;
            if (backtracking( G, v+1, colors))
                return true;
            colors[v] = -1;
        }
    }
    return false;
}

void
print_colors( UGraph G, int *colors) {
    printf( "-- coloração --\n");
    for (int v = 0; v < G->V; v++)
        printf( "cor[%d] = %d\n", v, colors[v]);
    printf( "\n");
}
