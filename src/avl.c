/* avl.c - Arvore AVL (recursiva: a altura e O(log n), entao a pilha e segura). */
#include "avl.h"

#include <stdlib.h>

/*
 * DIFERENCA PARA A BST:
 *  - A AVL usa RECURSAO sem medo: como ela se mantem balanceada, a altura e
 *    ~log2(n) (uns 20-30 niveis para 1 milhao), entao a pilha de chamadas nao
 *    estoura. Na BST degenerada isso nao seria seguro.
 *  - Cada no guarda a propria altura e, depois de cada insercao/remocao, o
 *    codigo sobe corrigindo as alturas e fazendo rotacoes quando preciso.
 */


static int alt(const NoAVL *n) { return n ? n->altura : -1; }

static void atualizar(NoAVL *n)
{
    int a = alt(n->esq), b = alt(n->dir);
    n->altura = 1 + (a > b ? a : b);
}

/* rotacao a direita (lado esquerdo pesado): x sobe, y desce */
static NoAVL *rot_dir(AVL *t, NoAVL *y)
{
    NoAVL *x = y->esq;
    y->esq = x->dir;
    x->dir = y;
    atualizar(y);
    atualizar(x);
    t->rotacoes++;
    return x;
}

/* espelho da rot_dir */
static NoAVL *rot_esq(AVL *t, NoAVL *x)
{
    NoAVL *y = x->dir;
    x->dir = y->esq;
    y->esq = x;
    atualizar(x);
    atualizar(y);
    t->rotacoes++;
    return y;
}

/* atualiza a altura de n e rotaciona se o fator de balanceamento sair de [-1, 1].
 * esq-esq / dir-dir: rotacao simples. esq-dir / dir-esq: rotacao dupla.
 * devolve a nova raiz da subarvore. */
static NoAVL *balancear(AVL *t, NoAVL *n)
{
    atualizar(n);
    int fb = alt(n->esq) - alt(n->dir);
    if (fb > 1) {
        if (alt(n->esq->esq) < alt(n->esq->dir)) n->esq = rot_esq(t, n->esq); /* caso esquerda-direita */
        return rot_dir(t, n);
    }
    if (fb < -1) {
        if (alt(n->dir->dir) < alt(n->dir->esq)) n->dir = rot_dir(t, n->dir); /* caso direita-esquerda */
        return rot_esq(t, n);
    }
    return n;
}

void avl_init(AVL *t)
{
    t->raiz = NULL;
    t->tamanho = 0;
    t->comparacoes = 0;
    t->rotacoes = 0;
}

/* st: 1 = inseriu, 0 = chave repetida, -1 = sem memoria */
static NoAVL *ins(AVL *t, NoAVL *n, const Registro *r, int *st)
{
    if (!n) {
        NoAVL *novo = malloc(sizeof *novo);
        if (!novo) { *st = -1; return NULL; }
        novo->reg = r;
        novo->esq = novo->dir = NULL;
        novo->altura = 0;
        *st = 1;
        return novo;
    }
    int c = registro_comparar(r, n->reg);
    t->comparacoes++;
    if (c == 0) { *st = 0; return n; }
    if (c < 0) n->esq = ins(t, n->esq, r, st);
    else       n->dir = ins(t, n->dir, r, st);
    if (*st != 1) return n; /* nada mudou na subarvore */
    return balancear(t, n);
}

int avl_inserir(AVL *t, const Registro *r)
{
    int st = 0;
    t->raiz = ins(t, t->raiz, r, &st);
    if (st == 1) t->tamanho++;
    return st;
}

/* nao rebalanceia, so conta comparacoes (por isso recebe AVL * e nao const) */
const Registro *avl_buscar(AVL *t, const Registro *chave)
{
    const NoAVL *n = t->raiz;
    while (n) {
        int c = registro_comparar(chave, n->reg);
        t->comparacoes++;
        if (c == 0) return n->reg;
        n = (c < 0) ? n->esq : n->dir;
    }
    return NULL;
}

static NoAVL *min_no(NoAVL *n)
{
    while (n->esq) n = n->esq;
    return n;
}

/* tira o menor no da subarvore e rebalanceia na volta */
static NoAVL *rem_min(AVL *t, NoAVL *n)
{
    if (!n->esq) {
        NoAVL *d = n->dir;
        free(n);
        return d;
    }
    n->esq = rem_min(t, n->esq);
    return balancear(t, n);
}

static NoAVL *rem(AVL *t, NoAVL *n, const Registro *k, int *st)
{
    if (!n) { *st = 0; return NULL; }
    int c = registro_comparar(k, n->reg);
    t->comparacoes++;
    if (c < 0) {
        n->esq = rem(t, n->esq, k, st);
    } else if (c > 0) {
        n->dir = rem(t, n->dir, k, st);
    } else {
        *st = 1;
        if (!n->esq || !n->dir) {            /* zero ou um filho */
            NoAVL *f = n->esq ? n->esq : n->dir;
            free(n);
            return f;
        }
        /* dois filhos: copia o sucessor pra ca e remove ele da direita */
        n->reg = min_no(n->dir)->reg;
        n->dir = rem_min(t, n->dir);
    }
    if (*st != 1) return n;
    return balancear(t, n);
}

int avl_remover(AVL *t, const Registro *chave)
{
    int st = 0;
    t->raiz = rem(t, t->raiz, chave, &st);
    if (st == 1) t->tamanho--;
    return st;
}

/* O(1): a altura ja esta guardada no no. Arvore vazia = -1 */
int avl_altura(const AVL *t) { return alt(t->raiz); }

/* o modo so muda o momento em que o no e visitado (antes, entre ou depois dos filhos) */
static void percorrer_rec(const NoAVL *n, int modo, VisitaFn v, void *u)
{
    if (!n) return;
    if (modo == MODO_PRE_ORDEM) v(n->reg, u);
    percorrer_rec(n->esq, modo, v, u);
    if (modo == MODO_EM_ORDEM) v(n->reg, u);
    percorrer_rec(n->dir, modo, v, u);
    if (modo == MODO_POS_ORDEM) v(n->reg, u);
}

int avl_percorrer(const AVL *t, int modo, VisitaFn visita, void *usuario)
{
    percorrer_rec(t->raiz, modo, visita, usuario);
    return 0;   /* nunca falha, assinatura igual a da BST */
}

/*
 * VALIDAR (recursivo, em ordem). Confere tres coisas de uma vez:
 *   1. propriedade de busca: cada chave e estritamente maior que a anterior;
 *   2. a altura GUARDADA em cada no e igual a altura REAL (calculada dos filhos);
 *   3. o fator de balanceamento de cada no esta entre -1 e 1.
 * 'h' devolve a altura real da subarvore; 'ant' e 'cont' acompanham o percurso.
 */
static int validar_rec(const NoAVL *n, const Registro **ant, size_t *cont, int *h)
{
    if (!n) { *h = -1; return 1; }
    int hl, hr;
    if (!n->reg) return 0;
    if (!validar_rec(n->esq, ant, cont, &hl)) return 0;
    if (*ant && registro_comparar(*ant, n->reg) >= 0) return 0; /* propriedade de busca */
    *ant = n->reg;
    (*cont)++;
    if (!validar_rec(n->dir, ant, cont, &hr)) return 0;
    int real = 1 + (hl > hr ? hl : hr);
    if (n->altura != real) return 0;            /* altura armazenada = altura real */
    if (hl - hr < -1 || hl - hr > 1) return 0;  /* fator de balanceamento */
    *h = real;
    return 1;
}

/* alem disso, confere se a contagem de nos bate com 'tamanho' */
int avl_validar(const AVL *t)
{
    const Registro *ant = NULL;
    size_t cont = 0;
    int h;
    return validar_rec(t->raiz, &ant, &cont, &h) && cont == t->tamanho;
}

static void liberar_rec(NoAVL *n)
{
    if (!n) return;
    liberar_rec(n->esq);
    liberar_rec(n->dir);
    free(n);
}

/* cuidado: avl_init zera tambem as metricas (comparacoes e rotacoes) */
void avl_liberar(AVL *t)
{
    liberar_rec(t->raiz);
    avl_init(t);
}
