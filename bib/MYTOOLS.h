// MAC0328 (2019) 
// Altere, modifique, e acrescente à vontade. Sugiro marcar os trechos 
// de código modificados/acrescentados com um comentário do tipo
// "// novo". Preserve o meu bom layout (veja www.ime.usp.br/~pf/
// algoritmos/aulas/layout.html) e não use tabs. 

// Este arquivo: MYTOOLS.h (codificação UTF-8)
// Data: 2019-09-21
// Autor: Paulo Feofiloff
//
// Esta é a interface de minha biblioteca particular que contém algumas 
// ferramentas genéricas muito básicas.
////////////////////////////////////////////////////////////////////////


#ifndef _MYTOOLS_h
#define _MYTOOLS_h

#include <stdlib.h> // malloc(), free(), rand()
#include <stdio.h> // printf()
#include <limits.h> // INT_MAX
#include <stdbool.h>

// Muitas funções usam strings.
typedef char *string;

// Minha versão de malloc() interrompe execução se malloc() devolver
// NULL.
void *
mallocc( size_t n);

// Esta função devolve um inteiro aleatório entre a e b inclusive, ou
// seja, no intervalo fechado [a,b]. Vamos supor que a <= b e que
// b-a <= RAND_MAX. (O código foi copiado da biblioteca random de Eric
// Roberts.)
int 
randInteger( int a, int b);

// Devolve um número real aleatório no intervalo semi-aberto [a,b).
// (O código foi copiado da biblioteca random de Eric Roberts.)
double 
randReal( double a, double b);

// Esta função faz uma permutação aleatória (random shuffle) dos
// elementos do vetor v[0..V-1].
void 
randPermutation( int v[], int V);

// Dizemos que um número x é bom se -M <= x <= +M, sendo M = INT_MAX.
// A função noOverflow() recebe números bons t e c e decide se t+c é
// bom. 
bool
noOverflow( int t, int c);

// Esta função rearranja o vetor v[p..r] em ordem crescente. (Veja
// www.ime.usp.br/~pf/algoritmos/aulas/quick.html.)
void 
quickSort( int v[], int p, int r);

// Em lugar de minha função quicksort(), é melhor usar a poderosa função
// qsort() da biblioteca padrão stdlib. Essa função é muito genérica e
// por isso exige que o cliente defina uma função de comparação
// compare().
//
// Exemplo: A função compare() compara inteiros apontados por elem1 e
// elem2. Devolve -1 se *elem1 é menor que *elem2, devolve +1 se *elem1
// é maior que *elem2, devolve 0 se *elem1 é igual a *elem2.
int 
compare( const void *elem1, const void *elem2);
// Para ordenar um vetor arr[0..N-1] de inteiros, diga
// qsort( arr, N, sizeof (int), compare).

// Esta função decide se as partições do conjunto 0..V-1 determinadas
// pelos vetores sc1[] e sc2[] são iguais. Para cada v em 0..V-1, sc1[v]
// é o "nome" do bloco que contém v na primeira partição. Analogamente,
// sc2[v] é o "nome" do bloco que contém v na segunda partição. Os dois
// vetores são indexados por 0..V-1 e têm valores em 0..V-1. As duas
// partições são iguais se v e w estão no mesmo bloco da primeira
// partição sempre que estão no mesmo bloco da segunda partição, e vice-
// versa
bool 
comparePartitions( int *sc1, int *sc2, int V);


#endif
