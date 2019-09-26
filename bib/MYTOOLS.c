// MAC0328 (2019) 
// Altere, modifique, e acrescente à vontade. Sugiro marcar os trechos 
// de código modificados/acrescentados com um comentário do tipo
// "// novo". Preserve o meu bom layout (veja www.ime.usp.br/~pf/
// algoritmos/aulas/layout.html) e não use tabs. 

// Este arquivo: MYTOOLS.c (codificação UTF-8)
// Data: 2019-09-21
// Autor: Paulo Feofiloff
//
// Esta é a implementação de minha biblioteca particular de ferramentas 
// genéricas muito básicas.
////////////////////////////////////////////////////////////////////////


#include "MYTOOLS.h"
// contém a interface padrão stdlib.h


// Minha versão de malloc().
void *
mallocc( size_t n) {
   void *pointer = malloc( n);
   if (pointer == NULL) {
      fprintf( stderr, "\nmalloc devolveu NULL!\n"); 
      exit( EXIT_FAILURE);
   }
   return pointer;
}

int 
randInteger( int a, int b) {
    double d = (double) rand() / ((double) RAND_MAX + 1); 
    // 0 <= d < 1
    int k = d * (b - a + 1); // 0 <= k <= b-a
    return a + k;
}

// A função rand() da biblioteca stdlib produz um número inteiro 
// aleatório no intervalo fechado 0..RAND_MAX.

double 
randReal( double a, double b) {
    double d = (double) rand() / ((double) RAND_MAX + 1);
    // 0 <= d < 1
    return a + d * (b - a);
}

void 
randPermutation( int v[], int V) {
   for (int n = V-1; n > 0; --n) {
      int r = randInteger( 0, n); // 0 <= r <= n
      int t;
      t = v[n], v[n] = v[r], v[r] = t;
   }
}

bool
noOverflow( int t, int c) {
   const int M = INT_MAX;
   // Fato 1: c >= 0 sse M-c é bom.
   // Fato 2: Suponha que c >= 0. Então t <= M-c sse t+c é bom.
   // Fato 1': c <= 0 sse -M-c é bom.
   // Fato 2': Suponha que c <= 0. Então t >= -M-c sse t+c é bom.
   if (c >= 0) {
      if (t <= M-c) return true; // M-c não produz overflow
      else return false;
   } else {
      if (t >= -M-c) return true; // -M-c não produz overflow
      else return false;
   }
}

// Função auxiliar usada por de quickSort(). Rearanja os elementos do
// vetor v[p..r] (com p <= r) e devolve j em p..r tal que
// v[p..j-1] <= v[j] < v[j+1..r].
// (Veja www.ime.usp.br/~pf/algoritmos/aulas/quick.html.)
static int
separa( int v[], int p, int r) {
   int c = v[r]; // pivô
   int t, j = p;
   for (int k = p; k < r; ++k)
      if (v[k] <= c) {
         t = v[j], v[j] = v[k], v[k] = t;
         ++j; 
      } 
   t = v[j], v[j] = v[r], v[r] = t;
   return j; 
}

void 
quickSort( int v[], int p, int r) {
   while (p < r) {      
      int j = separa (v, p, r);    
      if (j - p < r - j) {     
         quickSort (v, p, j-1);
         p = j + 1;            
      } else {                 
         quickSort (v, j+1, r);
         r = j - 1;
      }
   }
}

int 
compare( const void *a, const void *b) {
    int x = *((int *) a);
    int y = *((int *) b);
    if (x < y) return -1;
    if (x > y) return  1;
    return 0;
}


