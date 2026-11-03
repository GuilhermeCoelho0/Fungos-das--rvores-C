/*
 * bst.h - Arvore binaria de busca SEM balanceamento.
 *
 * - Convencao de altura: arvore vazia = -1, folha = 0.
 * - Os nos guardam ponteiros para Registro (const Registro *); quem possui
 *   a memoria dos registros e o vetor carregado do CSV, compartilhado entre
 *   BST e AVL. A arvore so aloca/libera seus proprios nos.
 * - Todas as operacoes sao ITERATIVAS: uma BST degenerada (altura ~ n) nao
 *   pode estourar a pilha de chamadas.
 */
#ifndef BST_H
#define BST_H

#include "registro.h"

typedef struct NoBST {
    const Registro *reg;
    struct NoBST *esq, *dir;
} NoBST;

typedef struct {
    NoBST *raiz;
    size_t tamanho;                 /* numero de nos */
    unsigned long long comparacoes; /* comparacoes de chave feitas por inserir/buscar/remover */
} BST;

void bst_init(BST *t);
int  bst_inserir(BST *t, const Registro *r);        /* 1 = inserido, 0 = chave repetida (rejeitada), -1 = sem memoria */
const Registro *bst_buscar(BST *t, const Registro *chave); /* NULL se nao existe */
int  bst_remover(BST *t, const Registro *chave);    /* 1 = removido, 0 = chave inexistente */
int  bst_altura(const BST *t);                      /* -1 se vazia; -2 se faltou memoria */
int  bst_validar(const BST *t);                     /* 1 = valida (em ordem estritamente crescente e tamanho coerente) */
int  bst_percorrer(const BST *t, int modo, VisitaFn visita, void *usuario); /* 0 ok, -1 sem memoria */
void bst_liberar(BST *t);

#endif
