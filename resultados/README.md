# Resultados

Esta pasta recebe os arquivos gerados pelos programas de teste:

- `bst_resultados.csv` - gerado por `teste_bst`
- `avl_resultados.csv` - gerado por `teste_avl`

Cada linha e uma fase de um cenario (A, B, C ou D) para um tamanho `n`.
Colunas: `arvore, cenario, fase, n, execucoes_usadas, tempo_mediana_s,
comparacoes, altura_final, rotacoes, tamanho_final, ins_aceitas,
ins_rejeitadas, buscas_ok, buscas_inesperadas, rem_ok, rem_inesperadas,
validacao`.

- `tempo_mediana_s`: mediana (em segundos de CPU, via `clock()`) das execucoes
  restantes depois de descartar a primeira (aquecimento).
- `comparacoes`, `altura_final`, `rotacoes`: da ultima execucao (sao
  deterministicos para a mesma semente e os mesmos dados).
- `buscas_inesperadas` / `rem_inesperadas`: devem ser 0.
- `validacao`: `OK` (estrutura valida), `FALHOU` ou `NA`.

Registre aqui tambem: modelo do computador, sistema operacional, versao do GCC,
semente, arquivo de dados e comando usado em cada rodada.

---

## Execucao - bst

- **Data:** 02/11/2026
- **Horario:** 15:07:42
- **Dataset:** `dataset/ocorrencias.csv`
- **Registros carregados:** 384
- **Cenarios:** ABCD
- **Repeticoes:** 5
- **Semente:** 42
- **CSV gerado:** `resultados/bst_resultados.csv`

| Cenario | Fase | N | Tempo mediano (s) | Comparacoes | Altura | Rotacoes | Validacao |
|---|---|---:|---:|---:|---:|---:|---|
| A | insercao | 384 | 0.000000 | 3185 | 16 | 0 | OK |
| A | busca_todas | 384 | 0.000000 | 3569 | 16 | 0 | NA |
| B | insercao_ordenada | 384 | 0.000500 | 73536 | 383 | 0 | OK |
| B | busca_primeira_ultima | 384 | 0.000000 | 385 | 383 | 0 | NA |
| C | buscas_remocoes | 384 | 0.000000 | 1544 | 15 | 0 | OK |
| D | copias_exatas | 384 | 0.000000 | 27 | 16 | 0 | OK |
| D | chaves_distintas | 384 | 0.000000 | 70 | 16 | 0 | OK |

**Verificacoes:** 167  
**Falhas:** 0


---

## Execucao - avl

- **Data:** 02/11/2026
- **Horario:** 15:17:57
- **Dataset:** `dataset/ocorrencias.csv`
- **Registros carregados:** 384
- **Cenarios:** ABCD
- **Repeticoes:** 5
- **Semente:** 42
- **CSV gerado:** `resultados/avl_resultados.csv`

| Cenario | Fase | N | Tempo mediano (s) | Comparacoes | Altura | Rotacoes | Validacao |
|---|---|---:|---:|---:|---:|---:|---|
| A | insercao | 384 | 0.000000 | 2806 | 9 | 232 | OK |
| A | busca_todas | 384 | 0.000000 | 3000 | 9 | 0 | NA |
| B | insercao_ordenada | 384 | 0.000000 | 2945 | 8 | 375 | OK |
| B | busca_primeira_ultima | 384 | 0.000000 | 18 | 8 | 0 | NA |
| C | buscas_remocoes | 384 | 0.000000 | 1299 | 9 | 254 | OK |
| D | copias_exatas | 384 | 0.000000 | 21 | 9 | 232 | OK |
| D | chaves_distintas | 384 | 0.000000 | 74 | 9 | 239 | OK |

**Verificacoes:** 176  
**Falhas:** 0


---

## Execucao - avl

- **Data:** 02/11/2026
- **Horario:** 15:19:11
- **Dataset:** `dataset/ocorrencias.csv`
- **Registros carregados:** 384
- **Cenarios:** ABCD
- **Repeticoes:** 5
- **Semente:** 42
- **CSV gerado:** `resultados/avl_resultados.csv`

| Cenario | Fase | N | Tempo mediano (s) | Comparacoes | Altura | Rotacoes | Validacao |
|---|---|---:|---:|---:|---:|---:|---|
| A | insercao | 384 | 0.000000 | 2806 | 9 | 232 | OK |
| A | busca_todas | 384 | 0.000000 | 3000 | 9 | 0 | NA |
| B | insercao_ordenada | 384 | 0.000000 | 2945 | 8 | 375 | OK |
| B | busca_primeira_ultima | 384 | 0.000000 | 18 | 8 | 0 | NA |
| C | buscas_remocoes | 384 | 0.000000 | 1299 | 9 | 254 | OK |
| D | copias_exatas | 384 | 0.000000 | 21 | 9 | 232 | OK |
| D | chaves_distintas | 384 | 0.000000 | 74 | 9 | 239 | OK |

**Verificacoes:** 176  
**Falhas:** 0

