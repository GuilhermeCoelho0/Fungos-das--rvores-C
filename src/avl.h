/*
 * avl.h - Arvore AVL (autobalanceada).
 *
 * - Mesmo comparador (registro_comparar) e mesma convencao de altura da BST:
 *   arvore vazia = -1, folha = 0.
 * - Fator de balanceamento = altura(esq) - altura(dir), sempre em [-1, 1].
 * - Rotacoes: cada rotacao simples conta 1; uma rotacao dupla (LR ou RL)
 *   conta 2 (sao duas rotacoes simples).
 * - Os nos guardam ponteiros para Registro; a arvore nao e dona dos registros.
 */
#ifndef AVL_H
#define AVL_H

#include "registro.h"

typedef struct NoAVL {
    const Registro *reg;
    struct NoAVL *esq, *dir;
    int altura;
} NoAVL;

typedef struct {
    NoAVL *raiz;
    size_t tamanho;
    unsigned long long comparacoes;
    unsigned long long rotacoes;
} AVL;

void avl_init(AVL *t);
int  avl_inserir(AVL *t, const Registro *r);        /* 1 inserido, 0 repetida, -1 sem memoria */
const Registro *avl_buscar(AVL *t, const Registro *chave);
int  avl_remover(AVL *t, const Registro *chave);    /* 1 removido, 0 inexistente */
int  avl_altura(const AVL *t);                      /* -1 se vazia */
int  avl_validar(const AVL *t);                     /* busca + alturas armazenadas + fator em [-1,1] + tamanho */
int  avl_percorrer(const AVL *t, int modo, VisitaFn visita, void *usuario);
void avl_liberar(AVL *t);

#endif
