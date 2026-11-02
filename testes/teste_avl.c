/* teste_avl.c - executa os testes comuns sobre a AVL. */
#include "teste_comum.h"
#include "../src/avl.h"

#include <stdlib.h>

static void *criar(void) { AVL *t = malloc(sizeof *t); if (t) avl_init(t); return t; }
static void destruir(void *t) { avl_liberar(t); free(t); }
static int inserir(void *t, const Registro *r) { return avl_inserir(t, r); }
static const Registro *buscar(void *t, const Registro *k) { return avl_buscar(t, k); }
static int remover(void *t, const Registro *k) { return avl_remover(t, k); }
static int altura(const void *t) { return avl_altura(t); }
static size_t tamanho(const void *t) { return ((const AVL *)t)->tamanho; }
static int validar(const void *t) { return avl_validar(t); }
static int percorrer(const void *t, int m, VisitaFn v, void *u) { return avl_percorrer(t, m, v, u); }
static unsigned long long comparacoes(const void *t) { return ((const AVL *)t)->comparacoes; }
static void zerar(void *t) { ((AVL *)t)->comparacoes = 0; }
static unsigned long long rotacoes(const void *t) { return ((const AVL *)t)->rotacoes; }

int main(int argc, char **argv)
{
    static const OpsArvore ops = {
        "avl", 1, 0, /* AVL: sem limite no cenario B */
        criar, destruir, inserir, buscar, remover, altura, tamanho, validar,
        percorrer, comparacoes, zerar, rotacoes
    };
    return teste_principal(&ops, argc, argv);
}
