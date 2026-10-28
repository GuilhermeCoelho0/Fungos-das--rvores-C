/* teste_comum.c - casos extremos, cenarios A-D, metricas e saida em CSV. */
#include "teste_comum.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_REPS 15
#define MAX_TAM  16
#define UL(x) ((unsigned long)(x))

static int g_falhas = 0, g_checks = 0;

#define CHECK(cond, ...)                                                     \
    do {                                                                     \
        g_checks++;                                                          \
        if (!(cond)) {                                                       \
            g_falhas++;                                                      \
            fprintf(stderr, "  [FALHA] " __VA_ARGS__);                       \
            fprintf(stderr, " (%s:%d)\n", __FILE__, __LINE__);               \
        }                                                                    \
    } while (0)

/* ------------------------------------------------------------------ */
/* Utilitarios                                                         */
/* ------------------------------------------------------------------ */
static unsigned long long splitmix(unsigned long long *s)
{
    unsigned long long z = (*s += 0x9E3779B97F4A7C15ULL);
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}

static int cmp_ptr_registro(const void *a, const void *b)
{
    return registro_comparar(*(const Registro *const *)a, *(const Registro *const *)b);
}

static int cmp_double(const void *a, const void *b)
{
    double x = *(const double *)a, y = *(const double *)b;
    return (x > y) - (x < y);
}

/* Mediana (media dos dois centrais se n for par). */
static double mediana(const double *v, int n)
{
    double tmp[MAX_REPS];
    if (n <= 0) return 0.0;
    memcpy(tmp, v, (size_t)n * sizeof *tmp);
    qsort(tmp, (size_t)n, sizeof *tmp, cmp_double);
    return (n % 2) ? tmp[n / 2] : (tmp[n / 2 - 1] + tmp[n / 2]) / 2.0;
}

static double segundos(clock_t ini) { return (double)(clock() - ini) / (double)CLOCKS_PER_SEC; }

/* Limite superior folgado da altura AVL: 1,45 * (floor(log2(n+2)) + 1). */
static int limite_altura_avl(size_t n)
{
    int bits = 0;
    size_t m = n + 2;
    while (m > 1) { bits++; m >>= 1; }
    return (int)(1.45 * (double)(bits + 1));
}

typedef struct { int v[16]; int n; } Col;

static void col_visita(const Registro *r, void *u)
{
    Col *c = u;
    if (c->n < 16) c->v[c->n] = r->year;
    c->n++;
}

static int col_igual(const Col *c, const int *esp, int n)
{
    if (c->n != n) return 0;
    for (int i = 0; i < n; i++)
        if (c->v[i] != esp[i]) return 0;
    return 1;
}

static Registro mk(const char *nome, int ano, long long id)
{
    Registro r;
    memset(&r, 0, sizeof r);
    snprintf(r.scientificName, sizeof r.scientificName, "%s", nome);
    r.year = ano;
    r.gbifID = id;
    return r;
}

static void estado(const OpsArvore *o, void *t, size_t esperado, const char *rot)
{
    CHECK(o->tamanho(t) == esperado, "%s: tamanho %lu (esperado %lu)", rot, UL(o->tamanho(t)), UL(esperado));
    CHECK(o->validar(t), "%s: validacao estrutural falhou", rot);
}

/* ------------------------------------------------------------------ */
/* Casos extremos e testes de corretude com dados artificiais          */
/* ------------------------------------------------------------------ */
static void teste_comparador(void)
{
    Registro a = mk("Amanita", 2020, 5), b = mk("Amanita", 2020, 6), c = mk("Amanita", 2021, 1);
    Registro d = mk("Boletus", 1999, 1), e = mk("Amanita", 2020, 5);
    CHECK(registro_comparar(&a, &b) < 0 && registro_comparar(&b, &a) > 0, "gbifID decide quando nome e ano empatam");
    CHECK(registro_comparar(&b, &c) < 0, "year tem prioridade sobre gbifID");
    CHECK(registro_comparar(&c, &d) < 0, "scientificName tem prioridade sobre year");
    CHECK(registro_comparar(&a, &e) == 0, "tres componentes iguais => 0");
}

static void teste_arvore_vazia_e_unica(const OpsArvore *o)
{
    Registro k = mk("Amanita muscaria", 2021, 10);
    Registro outro = mk("Amanita muscaria", 2021, 11);
    void *t = o->criar();
    CHECK(t != NULL, "criar arvore");
    if (!t) return;

    /* arvore vazia */
    CHECK(o->tamanho(t) == 0 && o->altura(t) == -1, "vazia: tamanho 0 e altura -1");
    CHECK(o->buscar(t, &k) == NULL, "vazia: busca nao encontra");
    CHECK(o->remover(t, &k) == 0, "vazia: remocao retorna 0");
    Col col = {{0}, 0};
    for (int m = 0; m < 3; m++) { col.n = 0; o->percorrer(t, m, col_visita, &col); CHECK(col.n == 0, "vazia: percurso %d sem visitas", m); }
    estado(o, t, 0, "vazia");

    /* um elemento */
    CHECK(o->inserir(t, &k) == 1, "um elemento: insercao aceita");
    CHECK(o->altura(t) == 0, "um elemento: altura 0");
    CHECK(o->buscar(t, &k) == &k, "um elemento: busca encontra");
    CHECK(o->buscar(t, &outro) == NULL, "um elemento: chave diferente nao encontrada");
    CHECK(o->inserir(t, &k) == 0, "duplicata rejeitada");
    estado(o, t, 1, "um elemento");
    CHECK(o->remover(t, &outro) == 0, "remocao de chave inexistente retorna 0");
    estado(o, t, 1, "apos remocao inexistente");
    CHECK(o->remover(t, &k) == 1, "um elemento: remocao ok");
    CHECK(o->remover(t, &k) == 0, "segunda remocao retorna 0");
    estado(o, t, 0, "apos remover unico");
    o->destruir(t);
}

static void teste_chaves_distintas(const OpsArvore *o)
{
    Registro r[5] = {
        mk("Amanita", 2020, 1), mk("Amanita", 2020, 2), /* so gbifID difere */
        mk("Amanita", 2021, 1),                          /* so year difere   */
        mk("Boletus", 2020, 1),                          /* so nome difere   */
        mk("Amanita", 2020, 1)                           /* copia exata da r[0] */
    };
    void *t = o->criar();
    if (!t) { CHECK(0, "criar arvore"); return; }
    for (int i = 0; i < 4; i++) CHECK(o->inserir(t, &r[i]) == 1, "chave distinta %d aceita", i);
    CHECK(o->inserir(t, &r[4]) == 0, "copia exata rejeitada");
    estado(o, t, 4, "chaves distintas");
    o->destruir(t);
}

static void teste_remocao_formas(const OpsArvore *o)
{
    /* chaves 50,30,70,20,40,60,80 (mesmo nome e id; muda so o year) */
    int anos[7] = {50, 30, 70, 20, 40, 60, 80};
    Registro r[7];
    for (int i = 0; i < 7; i++) r[i] = mk("N", anos[i], 1);
    void *t = o->criar();
    if (!t) { CHECK(0, "criar arvore"); return; }
    for (int i = 0; i < 7; i++) CHECK(o->inserir(t, &r[i]) == 1, "insercao %d", anos[i]);
    estado(o, t, 7, "remocao: montagem");

    CHECK(o->remover(t, &r[3]) == 1 && o->buscar(t, &r[3]) == NULL, "remove folha (20)");
    estado(o, t, 6, "apos folha");
    CHECK(o->remover(t, &r[1]) == 1 && o->buscar(t, &r[1]) == NULL, "remove no com um filho (30)");
    estado(o, t, 5, "apos um filho");
    CHECK(o->remover(t, &r[2]) == 1 && o->buscar(t, &r[2]) == NULL, "remove no com dois filhos (70)");
    estado(o, t, 4, "apos dois filhos");
    CHECK(o->remover(t, &r[0]) == 1 && o->buscar(t, &r[0]) == NULL, "remove a raiz (50)");
    estado(o, t, 3, "apos raiz");
    CHECK(o->buscar(t, &r[4]) == &r[4] && o->buscar(t, &r[5]) == &r[5] && o->buscar(t, &r[6]) == &r[6],
          "chaves restantes continuam encontradas");
    CHECK(o->remover(t, &r[0]) == 0, "remover de novo (inexistente) retorna 0");
    estado(o, t, 3, "apos remocao inexistente");
    o->destruir(t);
}

static void teste_percursos(const OpsArvore *o)
{
    Registro r[3] = {mk("N", 2, 1), mk("N", 1, 1), mk("N", 3, 1)};
    void *t = o->criar();
    if (!t) { CHECK(0, "criar arvore"); return; }
    for (int i = 0; i < 3; i++) o->inserir(t, &r[i]);
    Col c;
    static const int pre[3] = {2, 1, 3}, em[3] = {1, 2, 3}, pos[3] = {1, 3, 2};
    c.n = 0; o->percorrer(t, MODO_PRE_ORDEM, col_visita, &c);  CHECK(col_igual(&c, pre, 3), "pre-ordem = 2,1,3");
    c.n = 0; o->percorrer(t, MODO_EM_ORDEM, col_visita, &c);   CHECK(col_igual(&c, em, 3), "em ordem = 1,2,3");
    c.n = 0; o->percorrer(t, MODO_POS_ORDEM, col_visita, &c);  CHECK(col_igual(&c, pos, 3), "pos-ordem = 1,3,2");
    o->destruir(t);
}

static void teste_rotacoes(const OpsArvore *o)
{
    static const int seq[4][3] = {{1, 2, 3}, {3, 2, 1}, {3, 1, 2}, {1, 3, 2}};
    static const char *nome[4] = {"direita-direita (rot. esquerda simples)", "esquerda-esquerda (rot. direita simples)",
                                  "esquerda-direita (rot. dupla)", "direita-esquerda (rot. dupla)"};
    static const unsigned long long rot_esp[4] = {1, 1, 2, 2};
    for (int s = 0; s < 4; s++) {
        Registro r[3];
        for (int i = 0; i < 3; i++) r[i] = mk("N", seq[s][i], 1);
        void *t = o->criar();
        if (!t) { CHECK(0, "criar arvore"); return; }
        for (int i = 0; i < 3; i++) o->inserir(t, &r[i]);
        if (o->balanceada) {
            Col c;
            static const int pre[3] = {2, 1, 3};
            c.n = 0; o->percorrer(t, MODO_PRE_ORDEM, col_visita, &c);
            CHECK(o->altura(t) == 1, "%s: altura 1", nome[s]);
            CHECK(col_igual(&c, pre, 3), "%s: raiz 2 com filhos 1 e 3", nome[s]);
            CHECK(o->rotacoes(t) == rot_esp[s], "%s: %llu rotacao(oes) (obtido %llu)", nome[s], rot_esp[s], o->rotacoes(t));
        } else {
            CHECK(o->altura(t) == 2, "BST sem balanceamento: altura 2 com 3 chaves em cascata");
            CHECK(o->rotacoes(t) == 0, "BST nao faz rotacoes");
        }
        estado(o, t, 3, "rotacoes");
        o->destruir(t);
    }
}

/* Comparacao com um modelo simples: presenca[ano] (chave = ("F", ano, 1)). */
static void teste_modelo_aleatorio(const OpsArvore *o)
{
    enum { K = 1000, OPS = 20000 };
    static Registro r[K];
    static unsigned char presente[K];
    unsigned long long sem = 12345ULL;
    size_t esperado = 0;
    memset(presente, 0, sizeof presente);
    for (int i = 0; i < K; i++) r[i] = mk("F", i, 1);
    void *t = o->criar();
    if (!t) { CHECK(0, "criar arvore"); return; }
    int divergiu = 0, invalida = 0;
    for (int i = 0; i < OPS; i++) {
        int y = (int)(splitmix(&sem) % K), op = (int)(splitmix(&sem) % 3);
        if (op == 0) {
            int got = o->inserir(t, &r[y]);
            if (got != (presente[y] ? 0 : 1)) divergiu++;
            if (!presente[y]) { presente[y] = 1; esperado++; }
        } else if (op == 1) {
            int got = o->remover(t, &r[y]);
            if (got != (presente[y] ? 1 : 0)) divergiu++;
            if (presente[y]) { presente[y] = 0; esperado--; }
        } else {
            const Registro *got = o->buscar(t, &r[y]);
            if ((got != NULL) != (presente[y] != 0)) divergiu++;
        }
        if (i % 100 == 0 && (!o->validar(t) || o->tamanho(t) != esperado)) invalida++;
    }
    CHECK(divergiu == 0, "modelo aleatorio: %d divergencias entre a arvore e o modelo", divergiu);
    CHECK(invalida == 0, "modelo aleatorio: %d validacoes periodicas falharam", invalida);
    estado(o, t, esperado, "modelo aleatorio (final)");
    o->destruir(t);
}

static void casos_extremos(const OpsArvore *o)
{
    int f0 = g_falhas, c0 = g_checks;
    printf("== Casos extremos e corretude (%s) ==\n", o->nome);
    teste_comparador();
    teste_arvore_vazia_e_unica(o);
    teste_chaves_distintas(o);
    teste_remocao_formas(o);
    teste_percursos(o);
    teste_rotacoes(o);
    teste_modelo_aleatorio(o);
    printf("   %d verificacoes, %d falha(s)\n\n", g_checks - c0, g_falhas - f0);
}

/* ------------------------------------------------------------------ */
/* Contexto, fases e saida                                             */
/* ------------------------------------------------------------------ */
typedef struct {
    const OpsArvore *ops;
    Registro *vet;
    size_t n;
    const Registro **perm; /* vet embaralhado (semente fixa): a "ordem aleatoria" */
    int tem_dups;
    int reps;
    unsigned long long seed;
    FILE *saida;
    const char *cenarios;
    size_t sizes[MAX_TAM];
    int nsizes;
    int tamanhos_usuario;
} Ctx;

typedef struct {
    const char *nome;
    double t[MAX_REPS];
    unsigned long long comp, rot;
    int altura;
    size_t tam, ia, ir, bo, bi, ro, ri;
    int valid; /* -1 = nao se aplica */
} Fase;

typedef int (*CenarioFn)(Ctx *, size_t, int, Fase *);

static void fechar_fase(const OpsArvore *o, void *t, Fase *f, int rep, double dt, int com_rot)
{
    f->t[rep] = dt;
    f->comp = o->comparacoes(t);
    f->rot = com_rot ? o->rotacoes(t) : 0;
    f->altura = o->altura(t);
    f->tam = o->tamanho(t);
}

static void inserir_todos(const OpsArvore *o, void *t, const Registro *const *seq, size_t k, Fase *f, int *erro)
{
    f->ia = f->ir = 0;
    for (size_t i = 0; i < k; i++) {
        int r = o->inserir(t, seq[i]);
        if (r == 1) f->ia++;
        else if (r == 0) f->ir++;
        else { *erro = 1; return; }
    }
}

/* Cenario A: insercao em ordem aleatoria + busca de todas as chaves. */
static int cen_a(Ctx *c, size_t k, int rep, Fase *f)
{
    const OpsArvore *o = c->ops;
    void *t = o->criar();
    int erro = 0;
    if (!t) return -1;
    clock_t ini = clock();
    inserir_todos(o, t, c->perm, k, &f[0], &erro);
    double dt = segundos(ini);
    if (erro) { o->destruir(t); return -1; }
    fechar_fase(o, t, &f[0], rep, dt, 1);
    f[0].valid = o->validar(t);
    CHECK(f[0].valid, "A (n=%lu): estrutura invalida apos insercoes", UL(k));
    if (o->balanceada)
        CHECK(f[0].altura <= limite_altura_avl(f[0].tam), "A (n=%lu): altura %d acima do limite AVL", UL(k), f[0].altura);

    o->zerar_comparacoes(t);
    f[1].bo = f[1].bi = 0;
    ini = clock();
    for (size_t i = 0; i < k; i++) {
        if (o->buscar(t, c->perm[i])) f[1].bo++; else f[1].bi++;
    }
    dt = segundos(ini);
    fechar_fase(o, t, &f[1], rep, dt, 0);
    f[1].valid = -1;
    if (!c->tem_dups) CHECK(f[1].bi == 0, "A (n=%lu): %lu chaves inseridas nao foram encontradas", UL(k), UL(f[1].bi));
    o->destruir(t);
    return 0;
}

/* Cenario B: insercao em ordem crescente da chave composta + busca da 1a e da ultima. */
static int cen_b(Ctx *c, size_t k, int rep, Fase *f)
{
    const OpsArvore *o = c->ops;
    const Registro **ord = malloc(k * sizeof *ord);
    void *t = o->criar();
    int erro = 0;
    if (!ord || !t) { free(ord); if (t) o->destruir(t); return -1; }
    memcpy(ord, c->perm, k * sizeof *ord);
    qsort(ord, k, sizeof *ord, cmp_ptr_registro); /* ordenacao fora da medicao */

    clock_t ini = clock();
    inserir_todos(o, t, ord, k, &f[0], &erro);
    double dt = segundos(ini);
    if (erro) { o->destruir(t); free(ord); return -1; }
    fechar_fase(o, t, &f[0], rep, dt, 1);
    f[0].valid = o->validar(t);
    CHECK(f[0].valid, "B (n=%lu): estrutura invalida apos insercoes", UL(k));
    if (o->balanceada)
        CHECK(f[0].altura <= limite_altura_avl(f[0].tam), "B (n=%lu): altura %d acima do limite AVL", UL(k), f[0].altura);
    else
        CHECK(f[0].altura == (int)f[0].tam - 1, "B (n=%lu): BST deveria degenerar em lista (altura %d, n=%lu)", UL(k), f[0].altura, UL(f[0].tam));

    o->zerar_comparacoes(t);
    f[1].bo = f[1].bi = 0;
    ini = clock();
    if (o->buscar(t, ord[0])) f[1].bo++; else f[1].bi++;
    if (o->buscar(t, ord[k - 1])) f[1].bo++; else f[1].bi++;
    dt = segundos(ini);
    fechar_fase(o, t, &f[1], rep, dt, 0);
    f[1].valid = -1;
    CHECK(f[1].bi == 0, "B (n=%lu): primeira/ultima chave nao encontrada", UL(k));
    o->destruir(t);
    free(ord);
    return 0;
}

/* Cenario C: buscas e remocoes intercaladas sobre arvore montada em ordem aleatoria. */
static int cen_c(Ctx *c, size_t k, int rep, Fase *f)
{
    const OpsArvore *o = c->ops;
    void *t = o->criar();
    int erro = 0;
    Fase base;
    memset(&base, 0, sizeof base);
    if (!t) return -1;
    inserir_todos(o, t, c->perm, k, &base, &erro);
    if (erro) { o->destruir(t); return -1; }

    Registro *sonda = calloc(1, sizeof *sonda); /* chave inexistente: mesmo nome/id, year + 1.000.000 */
    if (!sonda) { o->destruir(t); return -1; }
    o->zerar_comparacoes(t);
    size_t ins_ok = base.ia, rem_exist_ok = 0, rem_exist_falha = 0, rem_inex_removidas = 0;
    size_t bus_ok = 0, bus_inesp = 0;
    clock_t ini = clock();
    for (size_t i = 0; i < k; i++) {
        const Registro *r = c->perm[i];
        switch (i % 10) {
        case 0: /* 10%: busca de chave existente */
            if (o->buscar(t, r)) bus_ok++; else bus_inesp++;
            break;
        case 1: /* 10%: busca de chave inexistente */
            strcpy(sonda->scientificName, r->scientificName);
            sonda->year = r->year + 1000000;
            sonda->gbifID = r->gbifID;
            if (o->buscar(t, sonda)) bus_inesp++; else bus_ok++;
            break;
        case 2: /* 10%: remocao de chave existente */
            if (o->remover(t, r)) rem_exist_ok++; else rem_exist_falha++;
            break;
        case 3: /* 10%: remocao de chave inexistente (nao pode alterar a arvore) */
            strcpy(sonda->scientificName, r->scientificName);
            sonda->year = r->year + 1000000;
            sonda->gbifID = r->gbifID;
            if (o->remover(t, sonda)) rem_inex_removidas++;
            break;
        default: break;
        }
    }
    double dt = segundos(ini);
    fechar_fase(o, t, &f[0], rep, dt, 1);
    f[0].ia = ins_ok;
    f[0].ir = base.ir;
    f[0].bo = bus_ok;
    f[0].bi = bus_inesp;
    f[0].ro = rem_exist_ok;
    f[0].ri = rem_exist_falha + rem_inex_removidas;

    /* Verificacoes (fora da medicao). */
    size_t ainda_la = 0;
    for (size_t i = 2; i < k; i += 10)
        if (o->buscar(t, c->perm[i])) ainda_la++;
    f[0].valid = o->validar(t);
    CHECK(f[0].valid, "C (n=%lu): estrutura invalida apos buscas/remocoes", UL(k));
    CHECK(f[0].tam == ins_ok - rem_exist_ok, "C (n=%lu): tamanho %lu != inseridas(%lu) - removidas(%lu)",
          UL(k), UL(f[0].tam), UL(ins_ok), UL(rem_exist_ok));
    CHECK(rem_inex_removidas == 0, "C (n=%lu): remocao de chave inexistente alterou a arvore", UL(k));
    if (!c->tem_dups) {
        CHECK(bus_inesp == 0, "C (n=%lu): %lu buscas com resultado inesperado", UL(k), UL(bus_inesp));
        CHECK(rem_exist_falha == 0, "C (n=%lu): %lu remocoes de chave existente falharam", UL(k), UL(rem_exist_falha));
        CHECK(ainda_la == 0, "C (n=%lu): %lu chaves removidas ainda sao encontradas", UL(k), UL(ainda_la));
    }
    free(sonda);
    o->destruir(t);
    return 0;
}

/* Cenario D: duplicidades forcadas e chaves que diferem em um componente. */
static int cen_d(Ctx *c, size_t k, int rep, Fase *f)
{
    const OpsArvore *o = c->ops;
    void *t = o->criar();
    int erro = 0;
    Fase base;
    memset(&base, 0, sizeof base);
    if (!t) return -1;
    inserir_todos(o, t, c->perm, k, &base, &erro);
    size_t ncop = (k + 99) / 100; /* 1% dos registros (pelo menos 1) */
    Registro *iguais = malloc(ncop * sizeof *iguais);
    Registro *outro_id = malloc(ncop * sizeof *outro_id);
    Registro *outro_ano = malloc(ncop * sizeof *outro_ano);
    if (erro || !iguais || !outro_id || !outro_ano) {
        o->destruir(t); free(iguais); free(outro_id); free(outro_ano);
        return -1;
    }
    for (size_t j = 0; j < ncop; j++) {
        const Registro *src = c->perm[j * 100];
        iguais[j] = *src;                       /* mesma chave composta */
        outro_id[j] = *src;
        outro_id[j].gbifID += 1000000000000000LL;   /* so o gbifID difere */
        outro_ano[j] = *src;
        outro_ano[j].year += 1000000;               /* so o year difere   */
    }
    size_t antes = o->tamanho(t);

    /* Fase 1: copias exatas (devem ser todas rejeitadas). */
    o->zerar_comparacoes(t);
    clock_t ini = clock();
    size_t rej = 0, acei = 0;
    for (size_t j = 0; j < ncop; j++) {
        int r = o->inserir(t, &iguais[j]);
        if (r == 0) rej++; else if (r == 1) acei++;
    }
    double dt = segundos(ini);
    fechar_fase(o, t, &f[0], rep, dt, 1);
    f[0].ia = acei; f[0].ir = rej; f[0].valid = o->validar(t);
    CHECK(rej == ncop && acei == 0, "D (n=%lu): %lu de %lu copias exatas foram aceitas", UL(k), UL(acei), UL(ncop));
    CHECK(o->tamanho(t) == antes, "D (n=%lu): duplicatas aumentaram o numero de nos", UL(k));
    CHECK(f[0].valid, "D (n=%lu): estrutura invalida apos duplicatas", UL(k));

    /* Fase 2: chaves que diferem em um unico componente (todas devem ser aceitas). */
    o->zerar_comparacoes(t);
    ini = clock();
    size_t ok2 = 0, rej2 = 0;
    for (size_t j = 0; j < ncop; j++) {
        if (o->inserir(t, &outro_id[j]) == 1) ok2++; else rej2++;
        if (o->inserir(t, &outro_ano[j]) == 1) ok2++; else rej2++;
    }
    dt = segundos(ini);
    fechar_fase(o, t, &f[1], rep, dt, 1);
    f[1].ia = ok2; f[1].ir = rej2; f[1].valid = o->validar(t);
    CHECK(ok2 == 2 * ncop && rej2 == 0, "D (n=%lu): chaves distintas rejeitadas (%lu aceitas de %lu)", UL(k), UL(ok2), UL(2 * ncop));
    CHECK(o->tamanho(t) == antes + ok2, "D (n=%lu): tamanho final incorreto", UL(k));
    CHECK(f[1].valid, "D (n=%lu): estrutura invalida apos chaves distintas", UL(k));

    o->destruir(t); /* a arvore aponta para as copias: so liberar depois */
    free(iguais); free(outro_id); free(outro_ano);
    return 0;
}

/* ------------------------------------------------------------------ */
/* Execucao de um cenario e relatorio                                  */
/* ------------------------------------------------------------------ */
static void emitir(Ctx *c, char id, const Fase *f, size_t k, double med, int usadas)
{
    printf("  [%c] n=%-8lu %-22s tempo(mediana)=%9.4f s  comp=%-14llu altura=%-8d rot=%-10llu val=%s\n",
           id, UL(k), f->nome, med, f->comp, f->altura, f->rot,
           f->valid < 0 ? "-" : (f->valid ? "OK" : "FALHOU"));
    if (c->saida) {
        fprintf(c->saida, "%s,%c,%s,%lu,%d,%.6f,%llu,%d,%llu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%s\n",
                c->ops->nome, id, f->nome, UL(k), usadas, med, f->comp, f->altura, f->rot, UL(f->tam),
                UL(f->ia), UL(f->ir), UL(f->bo), UL(f->bi), UL(f->ro), UL(f->ri),
                f->valid < 0 ? "NA" : (f->valid ? "OK" : "FALHOU"));
        fflush(c->saida);
    }
}

static void rodar(Ctx *c, char id, CenarioFn fn, const char *const *nomes, int nf, size_t k)
{
    Fase f[2];
    memset(f, 0, sizeof f);
    for (int i = 0; i < nf; i++) { f[i].nome = nomes[i]; f[i].valid = -1; }
    for (int rep = 0; rep < c->reps; rep++) {
        fprintf(stderr, "\r  [%c] n=%lu execucao %d/%d ...", id, UL(k), rep + 1, c->reps);
        fflush(stderr);
        if (fn(c, k, rep, f) != 0) {
            fprintf(stderr, "\n  [ERRO] cenario %c (n=%lu): falha de memoria\n", id, UL(k));
            g_falhas++;
            return;
        }
    }
    fprintf(stderr, "\r%60s\r", "");
    int ini = c->reps > 1 ? 1 : 0; /* descarta a 1a execucao (aquecimento) */
    for (int i = 0; i < nf; i++)
        emitir(c, id, &f[i], k, mediana(&f[i].t[ini], c->reps - ini), c->reps - ini);
}

/* Tamanhos a testar em um cenario (respeita limite da BST no cenario B). */
static int tamanhos_para(const Ctx *c, char id, size_t *out)
{
    static const size_t bst_ordenado[3] = {10000, 20000, 50000};
    size_t lim = (id == 'B') ? c->ops->limite_ordenado : 0;
    int m = 0;
    if (id == 'B' && lim > 0 && !c->tamanhos_usuario) {
        for (int i = 0; i < 3; i++) if (bst_ordenado[i] <= lim && bst_ordenado[i] <= c->n) out[m++] = bst_ordenado[i];
    } else {
        for (int i = 0; i < c->nsizes; i++)
            if (c->sizes[i] <= c->n && (lim == 0 || c->sizes[i] <= lim)) out[m++] = c->sizes[i];
    }
    if (m == 0) { /* conjunto pequeno demais: usa todos os registros disponiveis */
        size_t k = c->n;
        if (lim > 0 && k > lim) k = lim;
        if (k > 0) out[m++] = k;
    }
    return m;
}

static void executar_cenarios(Ctx *c)
{
    static const char *const fa[] = {"insercao", "busca_todas"};
    static const char *const fb[] = {"insercao_ordenada", "busca_primeira_ultima"};
    static const char *const fc[] = {"buscas_remocoes"};
    static const char *const fd[] = {"copias_exatas", "chaves_distintas"};
    size_t ks[MAX_TAM];
    for (const char *p = c->cenarios; *p; p++) {
        int m;
        switch (*p) {
        case 'A': printf("== Cenario A: insercao em ordem aleatoria (%s) ==\n", c->ops->nome);
            m = tamanhos_para(c, 'A', ks); for (int i = 0; i < m; i++) rodar(c, 'A', cen_a, fa, 2, ks[i]); break;
        case 'B': printf("== Cenario B: insercao em ordem crescente (%s) ==\n", c->ops->nome);
            m = tamanhos_para(c, 'B', ks); for (int i = 0; i < m; i++) rodar(c, 'B', cen_b, fb, 2, ks[i]); break;
        case 'C': printf("== Cenario C: buscas e remocoes intercaladas (%s) ==\n", c->ops->nome);
            m = tamanhos_para(c, 'C', ks); for (int i = 0; i < m; i++) rodar(c, 'C', cen_c, fc, 1, ks[i]); break;
        case 'D': printf("== Cenario D: duplicidades forcadas (%s) ==\n", c->ops->nome);
            {
                size_t kd = c->tamanhos_usuario ? c->sizes[0] : 100000;
                if (kd > c->n) kd = c->n;
                if (kd > 0) rodar(c, 'D', cen_d, fd, 2, kd);
            }
            break;
        default: break;
        }
        printf("\n");
    }
}

/* ------------------------------------------------------------------ */
/* Argumentos                                                          */
/* ------------------------------------------------------------------ */
static void uso(const char *prog)
{
    fprintf(stderr,
        "Uso: %s [arquivo.csv] [opcoes]\n"
        "  Sem arquivo CSV, roda apenas os casos extremos e a corretude com dados artificiais.\n"
        "Opcoes:\n"
        "  --max N            le no maximo N registros validos do CSV\n"
        "  --tamanhos a,b,c   tamanhos de entrada (padrao: 50000,100000,250000,500000,1000000)\n"
        "  --reps R           execucoes por medicao, 1..%d (padrao 5; a 1a e descartada)\n"
        "  --seed S           semente do embaralhamento (padrao 42)\n"
        "  --cenarios ABCD    subconjunto de cenarios a executar (padrao ABCD)\n"
        "  --saida arq.csv    arquivo de resultados (padrao resultados/<arvore>_resultados.csv)\n"
        "  --paises FI,NO,DK  filtro EXPLICITO por pais (compara com a coluna country/countryCode)\n"
        "  --anos A-B         filtro EXPLICITO por intervalo de anos (ex.: 2020-2022)\n"
        "  -h, --ajuda        esta mensagem\n", prog, MAX_REPS);
}

static int parse_ull(const char *s, unsigned long long *out)
{
    char *fim;
    if (!*s || *s == '-') return 0;
    errno = 0;
    unsigned long long v = strtoull(s, &fim, 10);
    if (errno || *fim) return 0;
    *out = v;
    return 1;
}

int teste_principal(const OpsArvore *ops, int argc, char **argv)
{
    Ctx c;
    memset(&c, 0, sizeof c);
    c.ops = ops;
    c.reps = 5;
    c.seed = 42;
    c.cenarios = "ABCD";
    static const size_t padrao[5] = {50000, 100000, 250000, 500000, 1000000};
    for (int i = 0; i < 5; i++) c.sizes[i] = padrao[i];
    c.nsizes = 5;

    const char *csv = NULL, *saida_caminho = NULL;
    char saida_padrao[128];
    size_t max = 0;
    FiltroCarga filtro;
    memset(&filtro, 0, sizeof filtro);

    for (int i = 1; i < argc; i++) {
        const char *a = argv[i];
        unsigned long long v;
        if (!strcmp(a, "-h") || !strcmp(a, "--ajuda")) { uso(argv[0]); return 0; }
        else if (!strcmp(a, "--max") && i + 1 < argc) {
            if (!parse_ull(argv[++i], &v) || v == 0) { fprintf(stderr, "Erro: --max exige inteiro positivo.\n"); uso(argv[0]); return 2; }
            max = (size_t)v;
        } else if (!strcmp(a, "--reps") && i + 1 < argc) {
            if (!parse_ull(argv[++i], &v) || v < 1 || v > MAX_REPS) { fprintf(stderr, "Erro: --reps deve estar entre 1 e %d.\n", MAX_REPS); uso(argv[0]); return 2; }
            c.reps = (int)v;
        } else if (!strcmp(a, "--seed") && i + 1 < argc) {
            if (!parse_ull(argv[++i], &v)) { fprintf(stderr, "Erro: --seed invalida.\n"); uso(argv[0]); return 2; }
            c.seed = v;
        } else if (!strcmp(a, "--tamanhos") && i + 1 < argc) {
            char *copia = malloc(strlen(argv[i + 1]) + 1), *p;
            if (!copia) return 3;
            strcpy(copia, argv[++i]);
            c.nsizes = 0;
            p = copia;
            while (*p) {
                char *fim = p;
                while (*fim && *fim != ',') fim++;
                char sep = *fim;
                *fim = '\0';
                if (!parse_ull(p, &v) || v == 0 || c.nsizes >= MAX_TAM) {
                    fprintf(stderr, "Erro: lista --tamanhos invalida (use inteiros positivos separados por virgula, no maximo %d).\n", MAX_TAM);
                    free(copia); uso(argv[0]); return 2;
                }
                c.sizes[c.nsizes++] = (size_t)v;
                p = sep ? fim + 1 : fim;
            }
            free(copia);
            if (c.nsizes == 0) { fprintf(stderr, "Erro: --tamanhos vazio.\n"); uso(argv[0]); return 2; }
            c.tamanhos_usuario = 1;
        } else if (!strcmp(a, "--cenarios") && i + 1 < argc) {
            c.cenarios = argv[++i];
            for (const char *p = c.cenarios; *p; p++)
                if (!strchr("ABCD", *p)) { fprintf(stderr, "Erro: --cenarios aceita apenas as letras A, B, C e D.\n"); uso(argv[0]); return 2; }
        } else if (!strcmp(a, "--saida") && i + 1 < argc) {
            saida_caminho = argv[++i];
        } else if (!strcmp(a, "--paises") && i + 1 < argc) {
            if (strlen(argv[i + 1]) >= sizeof filtro.paises) { fprintf(stderr, "Erro: lista de paises longa demais.\n"); return 2; }
            strcpy(filtro.paises, argv[++i]);
        } else if (!strcmp(a, "--anos") && i + 1 < argc) {
            const char *s = argv[++i];
            char *fim;
            long lo = strtol(s, &fim, 10), hi = lo;
            if (*fim == '-') { hi = strtol(fim + 1, &fim, 10); }
            if (*fim || lo < 1 || hi < lo || hi > 9999) { fprintf(stderr, "Erro: --anos deve ser A ou A-B (ex.: 2020-2022).\n"); uso(argv[0]); return 2; }
            filtro.ano_min = (int)lo; filtro.ano_max = (int)hi;
        } else if (a[0] == '-') {
            fprintf(stderr, "Erro: opcao desconhecida ou sem valor: %s\n", a);
            uso(argv[0]);
            return 2;
        } else if (!csv) {
            csv = a;
        } else {
            fprintf(stderr, "Erro: argumento inesperado: %s\n", a);
            uso(argv[0]);
            return 2;
        }
    }

    casos_extremos(ops);

    if (!csv) {
        printf("Nenhum CSV informado: cenarios A-D nao executados.\n");
        printf("Resultado: %d verificacoes, %d falha(s)\n", g_checks, g_falhas);
        return g_falhas ? 1 : 0;
    }

    /* Carga do dataset (fora de qualquer medicao de tempo das arvores). */
    char erro[256];
    EstatCarga st;
    clock_t ini = clock();
    if (registro_carregar_csv(csv, &filtro, max, &c.vet, &c.n, &st, erro, sizeof erro) != 0) {
        fprintf(stderr, "Erro ao carregar o CSV: %s\n", erro);
        return 3;
    }
    printf("== Dataset: %s ==\n", csv);
    if (filtro.paises[0]) printf("Filtro explicito de paises: %s\n", filtro.paises);
    if (filtro.ano_min) printf("Filtro explicito de anos: %d-%d\n", filtro.ano_min, filtro.ano_max);
    if (!filtro.paises[0] && !filtro.ano_min) printf("Nenhum filtro de pais/ano aplicado (todo o arquivo foi considerado).\n");
    registro_imprimir_estatisticas(&st, stdout);
    printf("  tempo de leitura ............. %.3f s (nao entra nas medicoes das arvores)\n\n", segundos(ini));
    if (c.n == 0) { fprintf(stderr, "Erro: nenhum registro valido no CSV.\n"); free(c.vet); return 3; }

    /* Ordem aleatoria reprodutivel (Fisher-Yates com PRNG proprio). */
    c.perm = malloc(c.n * sizeof *c.perm);
    if (!c.perm) { fprintf(stderr, "Erro: sem memoria.\n"); free(c.vet); return 3; }
    for (size_t i = 0; i < c.n; i++) c.perm[i] = &c.vet[i];
    unsigned long long sem = c.seed;
    for (size_t i = c.n - 1; i > 0; i--) {
        size_t j = (size_t)(splitmix(&sem) % (i + 1));
        const Registro *tmp = c.perm[i]; c.perm[i] = c.perm[j]; c.perm[j] = tmp;
    }
    /* Chaves repetidas no arquivo? (gbifID e unico no GBIF; se houver, avisa) */
    {
        const Registro **cp = malloc(c.n * sizeof *cp);
        if (cp) {
            memcpy(cp, c.perm, c.n * sizeof *cp);
            qsort(cp, c.n, sizeof *cp, cmp_ptr_registro);
            size_t d = 0;
            for (size_t i = 1; i < c.n; i++) if (registro_comparar(cp[i - 1], cp[i]) == 0) d++;
            free(cp);
            c.tem_dups = d > 0;
            if (d) printf("AVISO: %lu registro(s) com chave composta repetida no arquivo; insercoes repetidas serao rejeitadas\n"
                          "       e as verificacoes de contagem dos cenarios A e C ficam informativas.\n\n", UL(d));
        }
    }

    if (!saida_caminho) {
        snprintf(saida_padrao, sizeof saida_padrao, "resultados/%s_resultados.csv", ops->nome);
        saida_caminho = saida_padrao;
    }
    c.saida = fopen(saida_caminho, "w");
    if (!c.saida) {
        fprintf(stderr, "AVISO: nao foi possivel criar '%s' (%s); os resultados so aparecerao na tela.\n"
                        "       Rode a partir da raiz do projeto e confirme que a pasta existe, ou use --saida.\n\n",
                saida_caminho, strerror(errno));
    } else {
        fprintf(c.saida, "arvore,cenario,fase,n,execucoes_usadas,tempo_mediana_s,comparacoes,altura_final,rotacoes,"
                         "tamanho_final,ins_aceitas,ins_rejeitadas,buscas_ok,buscas_inesperadas,rem_ok,rem_inesperadas,validacao\n");
    }
    printf("Medicao: clock() (tempo de CPU) somente das operacoes na arvore; %d execucao(oes), 1a descartada, mediana das demais.\n"
           "Semente: %llu. Registros disponiveis: %lu.\n\n", c.reps, c.seed, UL(c.n));

    executar_cenarios(&c);

    if (c.saida) { fclose(c.saida); printf("Resultados gravados em %s\n", saida_caminho); }
    free(c.perm);
    free(c.vet);
    printf("Resultado final: %d verificacoes, %d falha(s)\n", g_checks, g_falhas);
    return g_falhas ? 1 : 0;
}
