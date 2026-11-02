/*
 * teste_comum.h - Infraestrutura de testes compartilhada pela BST e pela AVL.
 *
 * Cada arvore e "plugada" por uma tabela de funcoes (OpsArvore). Assim os
 * cenarios A-D e os casos extremos sao escritos UMA vez e executados de forma
 * identica nas duas estruturas (mesmos registros, mesma semente, mesmas
 * operacoes).
 */
#ifndef TESTE_COMUM_H
#define TESTE_COMUM_H

#include <stddef.h>
#include "../src/registro.h"

typedef struct OpsArvore {
    const char *nome;               /* "bst" ou "avl" (vai para o CSV de resultados) */
    int balanceada;                 /* 1 para AVL */
    size_t limite_ordenado;         /* maior n permitido no cenario B (0 = sem limite) */
    void *(*criar)(void);
    void (*destruir)(void *);
    int (*inserir)(void *, const Registro *);
    const Registro *(*buscar)(void *, const Registro *);
    int (*remover)(void *, const Registro *);
    int (*altura)(const void *);
    size_t (*tamanho)(const void *);
    int (*validar)(const void *);
    int (*percorrer)(const void *, int, VisitaFn, void *);
    unsigned long long (*comparacoes)(const void *);
    void (*zerar_comparacoes)(void *);
    unsigned long long (*rotacoes)(const void *);
} OpsArvore;

/* Ponto de entrada comum: analisa argumentos, roda casos extremos e cenarios.
 * Retorna: 0 = tudo certo, 1 = algum teste falhou, 2 = uso incorreto,
 * 3 = erro de execucao (arquivo, memoria...). */
int teste_principal(const OpsArvore *ops, int argc, char **argv);

#endif
