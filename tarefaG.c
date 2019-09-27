// MAC0328 (2019) Algoritmos em Grafos
// Aluno1:      FELIPE CASTRO DE NORONHA
// Número USP1: 10737032
// Aluno2:      CAROLINA SENRA MARQUES
// Número USP2: 10737101
// Tarefa:      Tarefa G
// Data:        2019-09-27
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

// Para testes
#define DEBUG 0

// Funcao auxiliar que confere se um vertice v pode receber uma cor c
bool
is_safe( UGraph G, int c, vertex v, int *colors);

// Funcao auxiliar que confere se todos os vertices ja receberam uma
// cor conferindo se ainda existe algum -1 no vetor colors[]
bool
is_complete( UGraph G, int *colors);

// Retirado de GRAPHlists.c com uma leve alteracao feita
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

// Implementação de uma heuristica aleatoria. Esse metodo procura uma
// coloracao valida para o grafo G sorteando um vertice v e guardando o
// menor valor de cor valido em colors[v] e repetindo este processo ate
// que todos os vertices tenham uma cor
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
        c = 0;
    }
    for (int v = 0; v < G-> V; v++)
        if (colors[v] > max) max = colors[v];
    return max + 1;
}

// Implementação da heuristica de forca bruta, ou seja, testa todas as
// combinacoes de cores possiveis ate encontrar uma coloracao valida
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

// Implementação da heuristica gulosa. Nesse metodo, para cada vertice v
// a função atribui à este vertice a cor com menor valor possivel
int
heuristica_3 (UGraph G, int *colors) {
    int k = 0;
    for (vertex v = 0; v < G->V; ++v) colors[v] = -1;
    bool *disponivel = mallocc( G->V * sizeof (int));
    for (vertex v = 0; v < G->V; ++v) {
        int i;
        for (i = 0; i < k; ++i)
            disponivel[i] = true;
        for (link a = G->adj[v]; a != NULL; a = a->next) {
            i = colors[a->w];
            if (i != -1) disponivel[i] = false;
        }
        for (i = 0; i < k; ++i)
            if (disponivel[i]) break;
        if (i < k) colors[v] = i;
        else colors[v] = k++;
    }
    free( disponivel);
    return k;
}

// Implementação da heuristica 'largest first'. Nesse metodo, a cada
// iteração, se pega o vertice com o maior grau que ainda não possui
// uma cor, e então, tenta associar a cor com menor valor possivel para
// ele.
int
heuristica_4( UGraph G, int *colors) {
    int *inv_deg = calloc( G->V, sizeof(int));
    memset( colors, -1, G->V * sizeof(int));

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
        vertex v = PQdelmin( inv_deg);
        memset( used, 0, G->V * sizeof(int));
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

    free( inv_deg);
    free( used);
    PQfree();
    return (lowest_color + 1);
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
    int *used = calloc( G->V, sizeof(int));
    int *degree = calloc( G->V, sizeof(int));
    int *saturation = calloc( G->V, sizeof(int));
    memset( colors, -1, G->V * sizeof(int));

    for (vertex v  = 0; v < G->V; v++)
        for (link a = G->adj[v]; a != NULL; a = a->next)
            degree[a->w]++;

    for (int it = 0; it < G->V; it++) {
        // Procurando o melhor vertice para a iteração atual
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
        // Achando melhor cor
        memset( used, 0, G->V * sizeof(int));
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

    free( used);
    free( degree);
    free( saturation);
    return (lowest_color + 1);
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

    // Semente
    srand( S);
    // Grafo a ser gerado
    UGraph G;
    G = UGRAPHrand1( V, E);

    if (DEBUG)
        GRAPHshow( G);

    // Variaveis usadas para medição de tempo
    double start, stop, elapsedTime;

    // Armazena as colorações
    int *colors = malloc( G->V*sizeof(int));

    // Array de funções que armazena as heuristicas
    int (*h[5])( UGraph, int*) = {heuristica_1,
                                heuristica_2,
                                heuristica_3,
                                heuristica_4,
                                heuristica_5};

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
    free( colors);
    UGRAPHdestroy( G);
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

    return( EXIT_SUCCESS);
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

/*

RELATORIO

- Como executar

$ make 101 && ./101 V E s

V = numero de vertices
E = numero de arestas
s = semente

- Observações

Foram escolhidas para esta tarefa, 5 heuristicas: aleatorizada, força
bruta, gulosa, largest first e a de Brelaz.

As três primeiras não apresentaram muita dificuldade acerca de sua
implementação.

Para implementação da heuristica de largest first, foi usado um min-heap
para o consulta do vertice com maior grau que ainda não teve sua cor
definida.

A heuristica de Brelaz (DSatur) não usou nenhuma estrutura adicional,
porem, afim de não realizarmos um uso excessivo da memoria do computador
optamos por fazer a implementação que faz uso excessivo do processador.

A fim de curiosidade, qundo estavamos realizando testes, fizemos alguns
grafos bipartidos manualmente e verificamos, que, de fato, a heuristica
de Brelaz produz o resultado otimo quando o grafo é deste tipo.

- Conclusoes

Realizamos testes para sabermos a performance de cada algoritmo, tanto no
numero de cores usadas quanto no tempo gasto.

A tabela abaixo, mostra o resultado e relação ao numero de cores da
execução de cada heuristica para diversos valores de V e E, com a
semente s = 62.

┌────────┬─────────┬────────────┬────────────────┬──────────────────┐
│ V - E  │ 10 - 30 │ 100 - 3500 │ 10000 - 400000 │ 100000 - 5000000 │
├────────┼─────────┼────────────┼────────────────┼──────────────────┤
│ heur 1 │       5 │         32 │             27 │              185 │
│ heur 2 │       5 │         29 │             27 │              184 │
│ heur 3 │       5 │         29 │             27 │              184 │
│ heur 4 │       5 │         27 │             25 │              180 │
│ heur 5 │       5 │         27 │             22 │              170 │
└────────┴─────────┴────────────┴────────────────┴──────────────────┘

Conforme o aumento dos valores de V e E, fica cada vez mais claro que a
heuristica 1 é a mais distante de entregar uma coloração otima, enquanto
a heuristica 5 é quem fica mais proximo de devolver a coloração otima.


A proxima tabela, mostra o resultado e relação ao tempo gasto por cada
heuristica para diversos valores de V e E, com a semente s = 62. Para
obter maior consistencia nos dados o tempo apresentado é uma media de 50
execuções da mesma chamada de funcao.

┌────────┬───────────┬────────────┬────────────────┬──────────────────┐
│ V - E  │  10 - 30  │ 100 - 3500 │ 10000 - 400000 │ 100000 - 5000000 │
├────────┼───────────┼────────────┼────────────────┼──────────────────┤
│ heur 1 │ 0.000008s │ 0.000554s  │  0.236832s     │   3.480093s      │
│ heur 2 │ 0.000003s │ 0.000468s  │  0.172865s     │   2.697173s      │
│ heur 3 │ 0.000005s │ 0.000185s  │  0.156896s     │   1.473255s      │
│ heur 4 │ 0.000008s │ 0.000355s  │  0.326953s     │   2.778469s      │
│ heur 5 │ 0.000017s │ 0.012338s  │  41.37471s     │   1813.358s      │
└────────┴───────────┴────────────┴────────────────┴──────────────────┘

Com ela, fica claro que as quatro primeiras heuristicas consomem quase o
mesmo tempo, mas a 5 (DSatur) consome um tempo consideravelmente maior
para calcular a coloração
No geral, a heuristica aleatoriza usa um número maior de cores do que as
outras heuristicas, apesar disso, em alguns casos ela pode ser mais
rapida do que algumas das heuristicas mais complexas.

*/
