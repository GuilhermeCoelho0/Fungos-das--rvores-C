/* bst.c - BST sem balanceamento, totalmente iterativa. */
#include "bst.h"

#include <stdlib.h>

/* Pilha dinamica de nos (com profundidade opcional). */
typedef struct {
    NoBST **no; /* 'no' guarda os ponteiros */
    int *prof; /* 'prof' guarda a profundidade de cada um */
    size_t n, cap; /* n = quantos itens tem agora, cap = quantos cabem */
} Pilha;

/* Empilha um no. retorna 1 se deu certo, 0 se faltou memoria. */
static int pilha_push(Pilha *p, NoBST *no, int prof)
{
    if (p->n == p->cap) { /* pilha cheia: aumenta */
        size_t nc = p->cap ? p->cap * 2 : 64; /* comeca com 64 e vai dobrando */
        NoBST **a = realloc(p->no, nc * sizeof *a);
        if (!a) return 0;
        p->no = a;
        int *b = realloc(p->prof, nc * sizeof *b);
        if (!b) return 0;
        p->prof = b;
        p->cap = nc;
    }
    p->no[p->n] = no;
    p->prof[p->n] = prof;
    p->n++;
    return 1;
}

static void pilha_free(Pilha *p)
{
    free(p->no);
    free(p->prof);
}

/* Cria uma arvore vazia. Sempre chamar antes de usar. */
void bst_init(BST *t)
{
    t->raiz = NULL;
    t->tamanho = 0;
    t->comparacoes = 0;
}

/*INSERIR
 * Desce da raiz ate achar um lugar vazio (NULL), indo para a esquerda se a
 * chave for menor e para a direita se for maior. Se achar uma chave igual,
 * rejeita.*/

int bst_inserir(BST *t, const Registro *r)
{
    NoBST **pp = &t->raiz;
    while (*pp) {
        int c = registro_comparar(r, (*pp)->reg);
        t->comparacoes++; /* metrica: toda comparacao de chave e contada */
        if (c == 0) return 0; /* chave composta repetida: rejeita */
        pp = (c < 0) ? &(*pp)->esq : &(*pp)->dir;
    }
    NoBST *n = malloc(sizeof *n);
    if (!n) return -1;
    n->reg = r; /* guarda so o ponteiro, nao copia o registro */
    n->esq = n->dir = NULL; /* todo no novo entra como folha */
    *pp = n; /* liga o no novo ao pai (ou a raiz) */
    t->tamanho++;
    return 1;
}

/* BUSCAR: mesma descida da insercao, devolve o registro ou NULL se nao achar */
const Registro *bst_buscar(BST *t, const Registro *chave)
{
    const NoBST *n = t->raiz;
    while (n) {
        int c = registro_comparar(chave, n->reg);
        t->comparacoes++;
        if (c == 0) return n->reg;
        n = (c < 0) ? n->esq : n->dir;
    }
    return NULL;
}
/* REMOVER
 * Passo 1: localizar o no (mesma descida, guardando em pp quem aponta para ele)
 * Passo 2: remover conforme o numero de filhos:
 *   -> 0 filhos ou so filho direito: o pai passa a apontar para o filho direito
 *     (que pode ser NULL, caso da folha)
 *   -> so filho esquerdo: o pai passa a apontar para o filho esquerdo
 *   -> 2 filhos: nao da para simplesmente "pular" o no. Pegamos o SUCESSOR EM
 *     ORDEM (menor chave da subarvore direita), copiamos o registro dele para
 *     o no alvo e removemos o no do sucessor (que tem no maximo 1 filho)
 */

int bst_remover(BST *t, const Registro *chave)
{
    NoBST **pp = &t->raiz;
    while (*pp) {
        int c = registro_comparar(chave, (*pp)->reg);
        t->comparacoes++;
        if (c == 0) break;
        pp = (c < 0) ? &(*pp)->esq : &(*pp)->dir;
    }
    if (!*pp) return 0; /* inexistente (inclui arvore vazia): nada muda */

    NoBST *alvo = *pp;
    if (!alvo->esq) {            /* folha ou so filho direito */
        *pp = alvo->dir;
        free(alvo);
    } else if (!alvo->dir) {     /* so filho esquerdo */
        *pp = alvo->esq;
        free(alvo);
    } else {                     /* dois filhos: troca pelo sucessor em ordem */
        NoBST **sp = &alvo->dir;
        while ((*sp)->esq) sp = &(*sp)->esq; /* vai o mais a esquerda possivel */
        NoBST *suc = *sp;
        *sp = suc->dir; /* o pai do sucessor herda o filho direito dele */
        alvo->reg = suc->reg; /* o alvo "vira" o sucessor (so troca o registro) */
        free(suc);
    }
    t->tamanho--;
    return 1;
}

/* ALTURA
 * A BST nao guarda a altura nos nos (a AVL guarda), entao precisamos calcular.
 * Fazemos uma busca em profundidade com pilha, empilhando cada no junto com a
 * sua profundidade, a maior profundidade vista e a altura
 * Retorna -1 para arvore vazia e -2 se faltar memoria*/
int bst_altura(const BST *t)
{
    if (!t->raiz) return -1;
    Pilha p = {0};
    int maior = 0;
    if (!pilha_push(&p, t->raiz, 0)) return -2;
    while (p.n) {
        p.n--; /* desempilha */
        NoBST *no = p.no[p.n];
        int d = p.prof[p.n];
        if (d > maior) maior = d;
        if (no->esq && !pilha_push(&p, no->esq, d + 1)) { pilha_free(&p); return -2; }
        if (no->dir && !pilha_push(&p, no->dir, d + 1)) { pilha_free(&p); return -2; }
    }
    pilha_free(&p);
    return maior;
}

/* PERCORRER (pre-ordem, em ordem e pos-ordem) sem recursao
 * A pilha faz o papel da "pilha de chamadas" que a recursao teria
 * 'cur' e o no onde estamos, descer pela esquerda = empilhar e ir ao filho
 * Para cada no, chama visita(registro, usuario) */
int bst_percorrer(const BST *t, int modo, VisitaFn visita, void *usuario)
{
    Pilha p = {0};
    NoBST *cur = t->raiz, *ultimo = NULL; /* 'ultimo' so e usado na pos-ordem */
    while (cur || p.n) {
        if (modo == MODO_PRE_ORDEM) {
            /* visita o no ANTES de descer, depois de acabar a esquerda,
             * volta ao no empilhado e segue pela direita */
            if (cur) {
                visita(cur->reg, usuario);
                if (!pilha_push(&p, cur, 0)) goto erro;
                cur = cur->esq;
            } else {
                cur = p.no[--p.n]->dir;
            }
        } else if (modo == MODO_EM_ORDEM) {
            /* desce tudo pela esquerda, ao voltar, visita o no e vai para a
             * direita. Resultado: chaves em ordem CRESCENTE (e o que o
             * validador usa para conferir a propriedade da BST) */
            if (cur) {
                if (!pilha_push(&p, cur, 0)) goto erro;
                cur = cur->esq;
            } else {
                NoBST *no = p.no[--p.n];
                visita(no->reg, usuario);
                cur = no->dir;
            }
        } else { /* pos-ordem
            * e o mais chato: so podemos visitar o no depois de ter visitado
             * as DUAS subarvores, 'ultimo' lembra o ultimo no visitado para
             * saber se ja voltamos da direita ou se ainda falta ir la. */
            if (cur) {
                if (!pilha_push(&p, cur, 0)) goto erro;
                cur = cur->esq;
            } else {
                NoBST *topo = p.no[p.n - 1];
                if (topo->dir && ultimo != topo->dir) {
                    cur = topo->dir; /* ainda falta a direita */
                } else {
                    visita(topo->reg, usuario);
                    ultimo = topo;
                    p.n--;
                }
            }
        }
    }
    pilha_free(&p);
    return 0;
erro: /* unico uso de goto: sair limpando a memoria se o push falhar */
    pilha_free(&p);
    return -1;
}

/*Validacao da propriedade de busca*/
/* Estado carregado durante o percurso em ordem do validador */
typedef struct {
    const Registro *ant; /* registro visitado imediatamente antes */
    size_t cont; /* quantos registros ja foram visitados */
    int ok; /* vira 0 se alguma regra for violada */
} EstVal;


/* Chamada em cada no, em ordem. Se a chave atual nao for ESTRITAMENTE maior
 * que a anterior, a arvore esta errada (fora de ordem ou com duplicata) */
static void visita_validar(const Registro *r, void *u)
{
    EstVal *e = u;
    if (!r || (e->ant && registro_comparar(e->ant, r) >= 0)) e->ok = 0;
    e->ant = r;
    e->cont++;
}

/* Valida: (1) percurso em ordem estritamente crescente e (2) o numero de nos
 * visitados bate com o contador 'tamanho' (acusa no "perdido" ou contador torto) */
int bst_validar(const BST *t)
{
    EstVal e = {NULL, 0, 1};
    if (bst_percorrer(t, MODO_EM_ORDEM, visita_validar, &e) != 0) return 0;
    return e.ok && e.cont == t->tamanho;
}
/* LIBERAR: apaga todos os nos. Usa a pilha para visitar cada no, empilhar os
 * filhos e so entao dar free no pai (assim nao perdemos o acesso aos filhos)
 * Os Registros nao sao apagados: pertencem ao vetor lido do CSV */

void bst_liberar(BST *t)
{
    Pilha p = {0};
    if (t->raiz && pilha_push(&p, t->raiz, 0)) {
        while (p.n) {
            NoBST *no = p.no[--p.n];
            if (no->esq) pilha_push(&p, no->esq, 0);
            if (no->dir) pilha_push(&p, no->dir, 0);
            free(no);
        }
    }
    pilha_free(&p);
    bst_init(t);
}
