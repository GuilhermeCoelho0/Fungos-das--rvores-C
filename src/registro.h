/*
 * registro.h - Registro de ocorrencia (GBIF), chave composta e leitor de CSV.
 *
 * Chave composta: (scientificName, year, gbifID), comparada NESTA ordem.
 * A mesma funcao registro_comparar() e usada pela BST e pela AVL.
 */
#ifndef REGISTRO_H
#define REGISTRO_H

#include <stddef.h>
#include <stdio.h>

#define TAM_NOME_CIENTIFICO 160
#define TAM_TAXON            32
#define TAM_ESPECIE          80
#define TAM_PAIS             32

/* Modos de percurso (usados por bst_percorrer e avl_percorrer). */
#define MODO_PRE_ORDEM 0
#define MODO_EM_ORDEM  1
#define MODO_POS_ORDEM 2

typedef struct {
    long long gbifID;                         /* parte 3 da chave */
    char scientificName[TAM_NOME_CIENTIFICO]; /* parte 1 da chave */
    int  year;                                /* parte 2 da chave */
    char kingdom[TAM_TAXON];
    char phylum[TAM_TAXON];
    char classe[TAM_TAXON];                   /* coluna "class" do CSV  */
    char ordem[TAM_TAXON];                    /* coluna "order" do CSV  */
    char family[TAM_TAXON];
    char genus[TAM_TAXON];
    char species[TAM_ESPECIE];                /* opcional */
    char country[TAM_PAIS];                   /* opcional */
    double latitude;
    double longitude;
    int tem_coordenadas;                      /* 0 = ausente/invalida */
} Registro;

typedef void (*VisitaFn)(const Registro *r, void *usuario);

/* Comparador da chave composta: retorna -1, 0 ou 1. */
int registro_comparar(const Registro *a, const Registro *b);

/* Filtro EXPLICITO aplicado na leitura (opcional). */
typedef struct {
    char paises[64];  /* lista separada por virgula (ex.: "FI,NO,DK"); "" = sem filtro */
    int  ano_min;     /* 0 = sem limite */
    int  ano_max;     /* 0 = sem limite */
} FiltroCarga;

typedef struct {
    char   delimitador;
    size_t linhas_lidas;          /* linhas de dados (sem cabecalho, sem linhas em branco) */
    size_t aceitas;
    size_t desc_malformada;       /* numero de campos diferente do cabecalho / aspas abertas */
    size_t desc_sem_nome;         /* scientificName vazio */
    size_t desc_sem_ano;          /* year vazio */
    size_t desc_ano_invalido;     /* year nao numerico ou fora de 1..9999 */
    size_t desc_id_invalido;      /* gbifID vazio, nao numerico ou <= 0 */
    size_t desc_filtro_pais;      /* fora do filtro de paises */
    size_t desc_filtro_ano;       /* fora do intervalo de anos */
    size_t coord_ausentes;        /* aceitas, mas sem coordenadas validas */
    size_t campos_truncados;      /* strings maiores que o buffer (foram cortadas) */
    int    parou_no_limite;       /* leitura interrompida por --max */
} EstatCarga;

/* Le o CSV. Retorna 0 em sucesso, -1 em erro (mensagem em 'erro'). O vetor
 * *saida deve ser liberado com free(). Colunas obrigatorias: gbifID,
 * scientificName, year. max == 0 significa "sem limite". */
int registro_carregar_csv(const char *caminho, const FiltroCarga *filtro,
                          size_t max, Registro **saida, size_t *qtd,
                          EstatCarga *st, char *erro, size_t erro_tam);

void registro_imprimir_estatisticas(const EstatCarga *st, FILE *out);

#endif
