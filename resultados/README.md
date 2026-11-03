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
- **Configurações do PC:** Intel Core i7-13650HX, 16GB DDR5, NVIDIA GeForce RTX 3050 6GB Laptop GPU

- **Situação:** ROdando com o PC recem ligado
## Execucao - bst


- **Data:** 03/11/2026
- **Horario:** 11:12:15
- **Dataset:** `dataset/ocorrencias.csv`
- **Registros carregados:** 879726
- **Cenarios:** ABCD
- **Repeticoes:** 5
- **Semente:** 42
- **CSV gerado:** `resultados/bst_resultados.csv`

| Cenario | Fase | N | Tempo mediano (s) | Comparacoes | Altura | Rotacoes | Validacao |
|---|---|---:|---:|---:|---:|---:|---|
| A | insercao | 50000 | 0.038000 | 916401 | 39 | 0 | OK |
| A | busca_todas | 50000 | 0.032500 | 966401 | 39 | 0 | NA |
| A | insercao | 100000 | 0.090500 | 1971210 | 42 | 0 | OK |
| A | busca_todas | 100000 | 0.082000 | 2071210 | 42 | 0 | NA |
| A | insercao | 250000 | 0.308500 | 5388165 | 44 | 0 | OK |
| A | busca_todas | 250000 | 0.296500 | 5638165 | 44 | 0 | NA |
| A | insercao | 500000 | 0.844500 | 11467364 | 46 | 0 | OK |
| A | busca_todas | 500000 | 0.722500 | 11967364 | 46 | 0 | NA |
| A | insercao | 879726 | 1.605500 | 21168198 | 49 | 0 | OK |
| A | busca_todas | 879726 | 1.551000 | 22047924 | 49 | 0 | NA |
| B | insercao_ordenada | 50000 | 43.573500 | 1249975000 | 49999 | 0 | OK |
| B | busca_primeira_ultima | 50000 | 0.002000 | 50001 | 49999 | 0 | NA |
| C | buscas_remocoes | 50000 | 0.013500 | 391702 | 37 | 0 | OK |
| C | buscas_remocoes | 100000 | 0.036500 | 840531 | 41 | 0 | OK |
| C | buscas_remocoes | 250000 | 0.111500 | 2283871 | 44 | 0 | OK |
| C | buscas_remocoes | 500000 | 0.281000 | 4845657 | 46 | 0 | OK |
| C | buscas_remocoes | 879726 | 0.572000 | 8914820 | 49 | 0 | OK |
| D | copias_exatas | 50000 | 0.000500 | 9743 | 39 | 0 | OK |
| D | chaves_distintas | 50000 | 0.001000 | 21150 | 39 | 0 | OK |

**Verificacoes:** 327  
**Falhas:** 0


---

## Execucao - avl

- **Data:** 03/11/2026
- **Horario:** 11:17:05
- **Dataset:** `dataset/ocorrencias.csv`
- **Registros carregados:** 879726
- **Cenarios:** ABCD
- **Repeticoes:** 5
- **Semente:** 42
- **CSV gerado:** `resultados/avl_resultados.csv`

| Cenario | Fase | N | Tempo mediano (s) | Comparacoes | Altura | Rotacoes | Validacao |
|---|---|---:|---:|---:|---:|---:|---|
| A | insercao | 50000 | 0.028000 | 725137 | 18 | 34861 | OK |
| A | busca_todas | 50000 | 0.023000 | 747998 | 18 | 0 | NA |
| A | insercao | 100000 | 0.064000 | 1551560 | 19 | 70083 | OK |
| A | busca_todas | 100000 | 0.063000 | 1597702 | 19 | 0 | NA |
| A | insercao | 250000 | 0.219500 | 4214920 | 21 | 174326 | OK |
| A | busca_todas | 250000 | 0.219500 | 4331070 | 21 | 0 | NA |
| A | insercao | 500000 | 0.529500 | 8935299 | 22 | 348405 | OK |
| A | busca_todas | 500000 | 0.579000 | 9167291 | 22 | 0 | NA |
| A | insercao | 879726 | 1.084500 | 16447833 | 23 | 613706 | OK |
| A | busca_todas | 879726 | 1.170000 | 16856244 | 23 | 0 | NA |
| B | insercao_ordenada | 50000 | 0.010000 | 734465 | 15 | 49984 | OK |
| B | busca_primeira_ultima | 50000 | 0.000000 | 32 | 15 | 0 | NA |
| B | insercao_ordenada | 100000 | 0.020000 | 1568929 | 16 | 99983 | OK |
| B | busca_primeira_ultima | 100000 | 0.000000 | 34 | 16 | 0 | NA |
| B | insercao_ordenada | 250000 | 0.050500 | 4237857 | 17 | 249982 | OK |
| B | busca_primeira_ultima | 250000 | 0.000000 | 36 | 17 | 0 | NA |
| B | insercao_ordenada | 500000 | 0.113500 | 8975713 | 18 | 499981 | OK |
| B | busca_primeira_ultima | 500000 | 0.000000 | 38 | 18 | 0 | NA |
| B | insercao_ordenada | 879726 | 0.203000 | 16545945 | 19 | 879706 | OK |
| B | busca_primeira_ultima | 879726 | 0.000000 | 40 | 19 | 0 | NA |
| C | buscas_remocoes | 50000 | 0.011000 | 307068 | 18 | 36716 | OK |
| C | buscas_remocoes | 100000 | 0.027000 | 654125 | 19 | 73715 | OK |
| C | buscas_remocoes | 250000 | 0.091000 | 1766591 | 21 | 183594 | OK |
| C | buscas_remocoes | 500000 | 0.236500 | 3729460 | 22 | 367203 | OK |
| C | buscas_remocoes | 879726 | 0.456000 | 6877068 | 23 | 646748 | OK |
| D | copias_exatas | 50000 | 0.000000 | 7459 | 18 | 34861 | OK |
| D | chaves_distintas | 50000 | 0.001000 | 16123 | 18 | 35714 | OK |

**Verificacoes:** 416  
**Falhas:** 0


---
- **Situação:** Rodando com os PC ligado depois de um tempo e renderizando vídeo

## Execucao - bst

- **Data:** 03/11/2026
- **Horario:** 15:15:15
- **Dataset:** `dataset/ocorrencias.csv`
- **Registros carregados:** 879726
- **Cenarios:** ABCD
- **Repeticoes:** 5
- **Semente:** 42
- **CSV gerado:** `resultados/bst_resultados.csv`

| Cenario | Fase | N | Tempo mediano (s) | Comparacoes | Altura | Rotacoes | Validacao |
|---|---|---:|---:|---:|---:|---:|---|
| A | insercao | 50000 | 0.178500 | 916401 | 39 | 0 | OK |
| A | busca_todas | 50000 | 0.154000 | 966401 | 39 | 0 | NA |
| A | insercao | 100000 | 0.483000 | 1971210 | 42 | 0 | OK |
| A | busca_todas | 100000 | 0.423500 | 2071210 | 42 | 0 | NA |
| A | insercao | 250000 | 1.305000 | 5388165 | 44 | 0 | OK |
| A | busca_todas | 250000 | 1.215500 | 5638165 | 44 | 0 | NA |
| A | insercao | 500000 | 2.799000 | 11467364 | 46 | 0 | OK |
| A | busca_todas | 500000 | 2.609000 | 11967364 | 46 | 0 | NA |
| A | insercao | 879726 | 5.562000 | 21168198 | 49 | 0 | OK |
| A | busca_todas | 879726 | 5.455000 | 22047924 | 49 | 0 | NA |
| B | insercao_ordenada | 50000 | 52.960500 | 1249975000 | 49999 | 0 | OK |
| B | busca_primeira_ultima | 50000 | 0.002000 | 50001 | 49999 | 0 | NA |
| C | buscas_remocoes | 50000 | 0.016500 | 391702 | 37 | 0 | OK |
| C | buscas_remocoes | 100000 | 0.041000 | 840531 | 41 | 0 | OK |
| C | buscas_remocoes | 250000 | 0.139000 | 2283871 | 44 | 0 | OK |
| C | buscas_remocoes | 500000 | 0.333000 | 4845657 | 46 | 0 | OK |
| C | buscas_remocoes | 879726 | 0.692000 | 8914820 | 49 | 0 | OK |
| D | copias_exatas | 50000 | 0.000000 | 9743 | 39 | 0 | OK |
| D | chaves_distintas | 50000 | 0.001000 | 21150 | 39 | 0 | OK |

**Verificacoes:** 327  
**Falhas:** 0


---

## Execucao - avl

- **Data:** 03/11/2026
- **Horario:** 15:31:32
- **Dataset:** `dataset/ocorrencias.csv`
- **Registros carregados:** 879726
- **Cenarios:** ABCD
- **Repeticoes:** 5
- **Semente:** 42
- **CSV gerado:** `resultados/avl_resultados.csv`

| Cenario | Fase | N | Tempo mediano (s) | Comparacoes | Altura | Rotacoes | Validacao |
|---|---|---:|---:|---:|---:|---:|---|
| A | insercao | 50000 | 0.125500 | 725137 | 18 | 34861 | OK |
| A | busca_todas | 50000 | 0.162000 | 747998 | 18 | 0 | NA |
| A | insercao | 100000 | 0.362500 | 1551560 | 19 | 70083 | OK |
| A | busca_todas | 100000 | 0.380500 | 1597702 | 19 | 0 | NA |
| A | insercao | 250000 | 1.074000 | 4214920 | 21 | 174326 | OK |
| A | busca_todas | 250000 | 1.138000 | 4331070 | 21 | 0 | NA |
| A | insercao | 500000 | 1.728000 | 8935299 | 22 | 348405 | OK |
| A | busca_todas | 500000 | 2.616500 | 9167291 | 22 | 0 | NA |
| A | insercao | 879726 | 4.261000 | 16447833 | 23 | 613706 | OK |
| A | busca_todas | 879726 | 4.996500 | 16856244 | 23 | 0 | NA |
| B | insercao_ordenada | 50000 | 0.031500 | 734465 | 15 | 49984 | OK |
| B | busca_primeira_ultima | 50000 | 0.000000 | 32 | 15 | 0 | NA |
| B | insercao_ordenada | 100000 | 0.072000 | 1568929 | 16 | 99983 | OK |
| B | busca_primeira_ultima | 100000 | 0.000000 | 34 | 16 | 0 | NA |
| B | insercao_ordenada | 250000 | 0.154000 | 4237857 | 17 | 249982 | OK |
| B | busca_primeira_ultima | 250000 | 0.000000 | 36 | 17 | 0 | NA |
| B | insercao_ordenada | 500000 | 0.334000 | 8975713 | 18 | 499981 | OK |
| B | busca_primeira_ultima | 500000 | 0.000000 | 38 | 18 | 0 | NA |
| B | insercao_ordenada | 879726 | 0.670500 | 16545945 | 19 | 879706 | OK |
| B | busca_primeira_ultima | 879726 | 0.000000 | 40 | 19 | 0 | NA |
| C | buscas_remocoes | 50000 | 0.051000 | 307068 | 18 | 36716 | OK |
| C | buscas_remocoes | 100000 | 0.125000 | 654125 | 19 | 73715 | OK |
| C | buscas_remocoes | 250000 | 0.375000 | 1766591 | 21 | 183594 | OK |
| C | buscas_remocoes | 500000 | 0.921500 | 3729460 | 22 | 367203 | OK |
| C | buscas_remocoes | 879726 | 1.953000 | 6877068 | 23 | 646748 | OK |
| D | copias_exatas | 50000 | 0.001000 | 7459 | 18 | 34861 | OK |
| D | chaves_distintas | 50000 | 0.003000 | 16123 | 18 | 35714 | OK |

**Verificacoes:** 416  
**Falhas:** 0

