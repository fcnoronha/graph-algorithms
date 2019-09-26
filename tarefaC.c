// MAC0328 (2019) Algoritmos em Grafos
// Aluno1:      FELIPE CASTRO DE NORONHA
// Número USP1: 10737032
// Aluno2:      CAROLINA SENRA MARQUES
// Número USP2: 10737101
// Tarefa:     Tarefa C
// Data:       2019-08-29
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

// Prototipos de funções auxiliares
bool dfsRcycle( Graph G, vertex v, int *pre, int *post, int *pa,
    int *cnt, int *cntt);
bool dfsDetect( Graph G, vertex v, vertex *pa, bool *deleted);
void printTopo( int nv, int *post);
void printTopoV2( int nv, int *topo);
void printCycle( vertex v, vertex w, int nv, vertex *pa);

// Versão modificada da primeira versão de GRAPHhasCycle().
// Chama GRAPHdfs para ter acesso aos vetores pre[], post[] e pa[] do
// grafo usando os dois primeiros para procurar por um arco de retorno.
// Caso um seja encontrado, ele chama printCycle para imprimir um ciclo,
// e chama printTopo para imprimir a permutação topológica caso o
// contrario
bool
CouT1( Graph G) {
    int *pre = malloc( G->V * sizeof (int));
    int *post = malloc( G->V * sizeof (int));
    vertex *pa = malloc( G->V * sizeof (vertex));
    GRAPHdfs( G, pre, post, pa); // calcula pre[] e post[]
    for (vertex v = 0; v < G->V; ++v) {
        for (link a = G->adj[v]; a != NULL; a = a->next) {
            vertex w = a->w;
            if (post[v] < post[w]) {// v-w é de retorno
                printCycle( v, w, G->V, pa);
                free( pre);
                free( post);
                free( pa);
                return true;
            }
        }
    }
    // post[v] > post[w] para todo arco v-w
    printTopo( G->V, post);
    free( pre);
    free( post);
    free( pa);
    return false;
}

// Versão baseada na versão on-the-fly de GRAPHhasCycle()
// Faz o mesmo que a CouT1 mas para assim que um ciclo é encontrado
// Ela compartilha as funções de printCycle e printTopo com CouT1
bool
CouT2( Graph G) {
    static int cnt = 0;
    static int cntt = 0;
    int *pre = malloc( G->V * sizeof (int));
    int *post = malloc( G->V * sizeof (int));
    vertex *pa = malloc( G->V * sizeof (vertex));
    for (vertex v = 0; v < G->V; ++v)
        pre[v] = post[v] = -1;
    for (vertex v = 0; v < G->V; ++v)
        if (pre[v] == -1)
            if (dfsRcycle( G, v, pre, post, pa, &cnt, &cntt)) {
                free( pre);
                free( post);
                free( pa);
                return true;
            }
    printTopo( G->V, post);
    free( pre);
    free( post);
    free( pa);
    return false;
}

// Versão baseada na função top() da tarefa B.
// A função usa um vetor ind[] que guarda o grau de entrada de cada
// vertice para evidenciar quais são fontes para que elas sejam
// desconsideradas. Depois, usa GRAPHreverse para transformar os
// sorvedouros em fontes e e tambem desconsiderá-las.
// Já o vetor lista[] guarda quem se torna fonte ao longo do processo
// baseando-se em que posições de ind[] tem valor 0. Se todos os
// vertices forem eiminados, o grafo é topologico, e portanto,
// printTopoV2 é chamada para imprimir a permutação topologica, caso o
// contrario, dfsDetect encontra o ciclo e o imprime
bool
CouT3( Graph G) {
    int *topo = malloc( G->V * sizeof (int));
    int *ind = calloc( G->V, sizeof (int));
    vertex *lista = calloc( G->V, sizeof (vertex));
    vertex *pa = malloc( G->V * sizeof (vertex));
    bool *deleted = calloc( G->V, sizeof (bool));
    int cnt_list = 0, cnt_topo = 0, idx;
    vertex v;
    // Removendo as fontes
    for (v = 0; v < G->V; v++)
        for (link a = G->adj[v]; a != NULL; a = a->next)
            ind[a->w]++;
    for (v = 0; v < G->V; v++)
        if (ind[v] == 0) {
            lista[cnt_list++] = v;
            deleted[v] = true;
            topo[v] = cnt_topo++;
        }
    for (idx = 0, v = lista[idx]; idx < cnt_list; v = lista[++idx])
        for (link a = G->adj[v]; a != NULL; a = a->next) {
            vertex w = a->w;
            ind[w]--;
            if (ind[w] == 0) {
                lista[cnt_list++] = w;
                deleted[w] = true;
                topo[w] = cnt_topo++;
            }
        }
    // Não tem ciclo, imprime a permutação topológica
    if (cnt_list == G->V) {
        printTopoV2( G->V, topo);
        free( topo);
        free( deleted);
        free( pa);
        free( lista);
        free( ind);
        return false;
    }
    // Removendo os sorvedouros
    for (int i = 0; i < G->V; i++) {
        pa[i] = -1;
        ind[i] = 0;
    }
    cnt_list = 0;
    Graph GV = GRAPHreverse(G);
    for (v = 0; v < GV->V; v++) {
        if (deleted[v]) continue;
        for (link a = GV->adj[v]; a != NULL; a = a->next)
            ind[a->w]++;
    }
    for (v = 0; v < GV->V; v++)
        if (ind[v] == 0 && !deleted[v]) {
            lista[cnt_list++] = v;
            deleted[v] = true;
        }
    for (idx  = 0, v = lista[idx]; idx < cnt_list; v = lista[++idx])
        for (link a = GV->adj[v]; a != NULL; a = a->next) {
            vertex w = a->w;
            ind[w]--;
            if (ind[w] == 0) {
                lista[cnt_list++] = w;
                deleted[w] = true;
            }
        }
    for (v = 0; v < G->V; v++) {
        if (deleted[v]) continue;
        if (dfsDetect( G, v, pa, deleted)) {
            free( topo);
            free( deleted);
            free( pa);
            free( lista);
            free( ind);
            return true;
        }
    }
    free( topo);
    free( deleted);
    free( pa);
    free( lista);
    free( ind);
    return true;
}

// Função que executa a rotina principal.
void
run( int V, int A, int S) {

    // Terceira validação da entrada. Necessaria pela suposição de
    // GRAPHrand1() que A <= V*(V-1)
    if (A > V*(V-1)) {
        printf( "\nERROR: A and V must hold A <= V*(V-1)\n");
        exit( EXIT_FAILURE);
    }

    // Semente
    srand( S);
    // Grafo a ser gerado
    Graph G;
    G = GRAPHrand1( V, A);

    // Variaveis usadas para medição d tempo
    double start, stop, elapsedTime;

    start = (double) clock () / CLOCKS_PER_SEC;
    bool b1 = CouT1( G);
    stop = (double) clock () / CLOCKS_PER_SEC;
    elapsedTime = stop - start;
    printf( "Tempo gasto na Cout1: %.6fs\n\n", elapsedTime);

    start = (double) clock () / CLOCKS_PER_SEC;
    bool b2 = CouT2( G);
    stop = (double) clock () / CLOCKS_PER_SEC;
    elapsedTime = stop - start;
    printf( "Tempo gasto na Cout2: %.6fs\n\n", elapsedTime);

    start = (double) clock () / CLOCKS_PER_SEC;
    bool b3 = CouT3( G);
    stop = (double) clock () / CLOCKS_PER_SEC;
    elapsedTime = stop - start;
    printf( "Tempo gasto na Cout3: %.6fs\n\n", elapsedTime);

    if (b1 != b2 || b2 != b3 || b1 != b3)
        printf("\aSaidas não são iguais!\n");

    // Limpeza final:
    printf( "\n");
    GRAPHdestroy( G);
}

int
main (int argc, char* arg[]) {

    int V, A, S;

    // Primeira validação da entrada
    if (argc < 4) {
        printf( "\nERRO: Usage: %s V A S\n\n", arg[0]);
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
    if (sscanf( arg[3], "%d", &S) != 1 || S < 0) {
        printf( "\nERROR: S must be a non-negative integer\n\n");
        exit( EXIT_FAILURE);
    }

    // Chamando a rotina
    run( V, A, S);

    return EXIT_SUCCESS;
}

// Função auxiliar para a chamada on-the-fly() para GRAPHhasCycle()
// Função recursiva que faz a busca em profundidade e preenche os
// vetores pre[], post[] e pa[]
bool
dfsRcycle( Graph G, vertex v, int *pre, int *post, int *pa, int *cnt,
        int *cntt) {

    pre[v] = (*cnt)++;
    for (link a = G->adj[v]; a != NULL; a = a->next) {
        vertex w = a->w;
        pa[w] = v;
        if (pre[w] == -1) {
            if (dfsRcycle( G, w, pre, post, pa, cnt, cntt))
                return true;
        }
        else if (post[w] == -1) {
                printCycle( v, w, G->V, pa);
                return true;
        }
    }
    post[v] = (*cntt)++;
    return false;
}

// Vai iterando pelos vertices colocando seus respectivos pais em pa[]
// até que se passe por um vertice que já tem pai para assim, encontrar
// o ciclo e imprimi-lo
bool
dfsDetect( Graph G, vertex v, vertex *pa, bool *deleted) {
    for (link a = G->adj[v]; a != NULL; a = a->next) {
        vertex w = a->w;
        if (deleted[w]) continue;
        if (pa[w] == -1) {
            pa[w] = v;
            if (dfsDetect( G, w, pa, deleted))
                return true;
        }
        else {
            printCycle( v, w, G->V, pa);
            return true;
        }
    }
    return false;
}

// Função auxiliar que recebe a numeração em post ordem e a imprime,
// usando a difereça com nv para gerar a permutação topologica.
void printTopo( int nv, int *post) {
    vertex *perm = malloc( nv * sizeof (vertex));
    for (vertex v = 0; v < nv; v++)
        perm[nv - post[v] - 1] = v;
    for (int i = 0; i < nv; i++)
        printf( "%d ", perm[i]);
    printf( "\n");
    free( perm);
}

// Função auxiliar que recebe a numeração topologica e gerar a
//permutação topolica, imprimindo-a
void printTopoV2( int nv, int *topo) {
    vertex *perm = malloc( nv * sizeof (vertex));
    for (vertex v = 0; v < nv; v++)
        perm[topo[v]] = v;
    for (int i = 0; i < nv; i++)
        printf( "%d ", perm[i]);
    printf( "\n");
}

// Função que recebe um vertice V, um vetor de pais pa[] e imprime o
// primeiro ciclo a ser detectado.
void printCycle( vertex v, vertex w, int nv, vertex *pa) {
    vertex *cycle = malloc( (nv+1) * sizeof (vertex));
    int cnt = 0;
    cycle[cnt++] = w;
    while (v != w) {
        cycle[cnt++] = v;
        v = pa[v];
    }
    cycle[cnt] = v;
    for (int i = cnt; i >= 0; i--) {
        printf( "%d", cycle[i]);
        if (i > 0) printf( "-");
        else printf( "\n");
    }
    free( cycle);
}
