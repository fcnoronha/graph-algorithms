// MAC0328 (2019) Algoritmos em Grafos
// Aluno:      FELIPE CASTRO DE NORONHA
// Número USP: 10737032
// Tarefa:     Tarefa C
// Data:       2019-08-29
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
// Implementação ingenua de um algoritmo que checa se um grafo é ou não
// aresta-biconexo. Para isso, o algoritmo, primeiramente, checa se o
// grafo é conexo, retornando false caso negativo. Em seguida, uma copia
// do grafo é criada para a seguinte operação: removemos uma aresta s-t,
// checamos se s continua ao alcance de t. Caso positivo, essa aresta
// não é uma ponte, pontanto, seguimos com a execução do algoritmo para
// as demais arestas. Caso os vertices não estejam mais alcance, essa
// aresta era uma ponte, portanto, o algoritmo termina, retornando false
bool
isEdgeBiconnected( UGraph G) {
    if (!UGRAPHisConnected( G))
        return false;
    UGraph GG = UGRAPHcopy( G);
    for (vertex v = 0; v < GG->V; v++) {
        link a;
        for (a = GG->adj[v]; a != NULL; a = a->next) {
            vertex w = a->w;
            UGRAPHremoveEdge( GG, v, w);
            if (!GRAPHreach( GG, v, w)) {
                UGRAPHdestroy( GG);
                return false;
            }
            UGRAPHinsertEdgeQuick( GG, v, w);
        }
    }
    UGRAPHdestroy( GG);
    return true;
}
