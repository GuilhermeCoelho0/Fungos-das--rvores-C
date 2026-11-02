/* teste_bst.c - executa os testes comuns sobre a BST. */
#include "teste_comum.h"
#include "../src/bst.h"

#include <stdlib.h>

static void *criar(void) { BST *t = malloc(sizeof *t); if (t) bst_init(t); return t; }
static void destruir(void *t) { bst_liberar(t); free(t); }
static int inserir(void *t, const Registro *r) { return bst_inserir(t, r); }
static const Registro *buscar(void *t, const Registro *k) { return bst_buscar(t, k); }
static int remover(void *t, const Registro *k) { return bst_remover(t, k); }
static int altura(const void *t) { return bst_altura(t); }
static size_t tamanho(const void *t) { return ((const BST *)t)->tamanho; }
static int validar(const void *t) { return bst_validar(t); }
static int percorrer(const void *t, int m, VisitaFn v, void *u) { return bst_percorrer(t, m, v, u); }
static unsigned long long comparacoes(const void *t) { return ((const BST *)t)->comparacoes; }
static void zerar(void *t) { ((BST *)t)->comparacoes = 0; }
static unsigned long long rotacoes(const void *t) { (void)t; return 0; }

int main(int argc, char **argv)
{
    static const OpsArvore ops = {
        "bst", 0, 50000, /* BST: cenario B limitado a 50 mil chaves (insercao ordenada custa O(n^2)) */
        criar, destruir, inserir, buscar, remover, altura, tamanho, validar,
        percorrer, comparacoes, zerar, rotacoes
    };
    return teste_principal(&ops, argc, argv);
}
