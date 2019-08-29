// MAC0328 (2019) Algoritmos em Grafos
// Aluno:      FELIPE CASTRO DE NORONHA
// Número USP: 10737032
// Tarefa:     tarefaB.c
// Data:       2019-08-15
//
// Solução feita em conjunto com CAROLINA SENRA MARQUES (NUSP: 10737101)
//
// DECLARO QUE MINHA DUPLA É A UNICA RESPONSÁVEL POR ESTE PROGRAMA.
// TODAS AS PARTES DO PROGRAMA, EXCETO AS QUE FORAM FORNECIDAS PELO
// PROFESSOR OU COPIADAS DO LIVRO OU DAS BIBLIOTECAS DE SEDGEWICK OU
// ROBERTS, FORAM DESENVOLVIDAS POR MIM.  DECLARO TAMBÉM QUE SOU
// RESPONSÁVEL POR TODAS AS EVENTUAIS CÓPIAS DESTE PROGRAMA E QUE NÃO
// DISTRIBUI NEM FACILITEI A DISTRIBUIÇÃO DE CÓPIAS.
//
////////////////////////////////////////////////////////////////////////
//
// Neste EP implementamos duas funções, tperm() e top().
// As 3 questoes foram feita em conjunto pela dupla.

#include <stdlib.h>
#include "GRAPHlists.h"

//Essa função recebe um grafo e uma permutação vv[] e decide se essa
//permutação é topologica ou não, retornando true ou false,
//respectivamente. Para realizar isso, ela percorre toda a permutação e
//para cada vertice vv[i] = v checa se algum vizinho de v ja foi
//visitado, caso isso ocorra, a permutação não é topolica.
bool
tperm( Graph G, int *vv) {
    bool *visto = calloc( G->V, sizeof(bool));
    for (int i = 0; i < G->V; i++) {
        vertex v = vv[i];
        for (link a = G->adj[v]; a != NULL; a = a->next) {
            vertex w = a->w;
            if (visto[w]) {
                free( visto);
                return false;
            }
        }
        visto[vv[i]] = true;
    }
    free( visto);
    return true;
}

// Função que recebe um grafo G e devolve true se ele for topologico e
// false caso o contrário. A função usa um vetor ind[] que guarda o grau
// de entrada de cada vertice para evidenciar quais são fontes.
// Já o vetor lista[] guarda quem se torna fonte ao longo do processo
// baseando-se em que posições de ind[] tem valor 0.
bool
top( Graph G) {
    int *ind = calloc( (G->V), sizeof(int));
    int *lista = calloc( (G->V), sizeof(int));
    int cnt = 0;
    for (vertex k = 0; k < G->V; k++)
        for (link a = G->adj[k]; a != NULL; a = a->next)
            ind[a->w]++;

    for (int i = 0; i < G->V; i++)
        if (ind[i] == 0)
            lista[cnt++] = i;

    for (int idx = 0, v = lista[idx]; idx < cnt; v = lista[++idx])
        for (link a = G->adj[v]; a != NULL; a = a->next) {
            ind[a->w]--;
            if (ind[a->w] == 0)
                lista[cnt++] = a->w;
        }

    free( ind);
    free( lista);
    return (cnt == G->V);
}

/*
2) Todo grafo topologico tem que ter pelo menos uma fonte, assim toda
vez que essa fonte é removida, por indução, pelo menos uma nova fonte
tem que ser criada e esse processo pode ser repetido até que todos os
vertices do grafo tenham sido removidos pois todo subgrafo de um grafo
topologico tambem é topologico. Se houvesse um ciclo no grafo, o que o
impede de ser topologico, pelo menos um vertice nunca se tornaria
fonte, algo que seria apontado pelo algoritmo que, portanto, funciona

3)
v-w dfsR(G,w)               pa[w]       pre[w]

7 dfsR(G, 7)                7           0
7-5 dfsR(G, 5)              7           1
  5-7
  5-4 dfsR(G, 4)            5           2
    4-7
    4-5
    4
  5-1 dfsR(G, 1)            5           3
    1-3 dfsR(G, 3)          1           4
      3-6 dfsR(G, 6)        3           5
        6-4
        6-2 dfsR(G, 2)      6           6
          2-7
          2
        6-0 dfsR(G, 0)      6           7
          0-4
          0-2
          0
        6
      3
    1
  5
7-3
7
*/
