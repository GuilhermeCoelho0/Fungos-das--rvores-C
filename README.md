# Fungos-das--rvores-C
# 🍄 Fungos das Árvores-C

**Trabalho da disciplina Estruturas de Dados II**
**Universidade Cidade de São Paulo (UNICID)**
**Professor:** Cid Rodrigues de Andrade

---

## 👥 Integrantes do Grupo

| Nome completo                      |        RGM |
| ---------------------------------- | ---------: |
| **Guilherme Coelho Silva**         |   42966515 |
| **Maria Luisa Assunção Novaes**    |   42802881 |
| **Miguel Targino Felix**           |   43079067 |
| **Thiago Salles de Almeida Silva** |   43461905 |

---

# 1. 📊 Dataset

## 1.1 Descrição

O conjunto de dados utilizado nos testes é composto por **registros de ocorrência de fungos** obtidos do **Global Biodiversity Information Facility (GBIF)**, uma infraestrutura internacional que disponibiliza dados sobre biodiversidade.

O domínio do dataset é a **biodiversidade de espécies de fungos**. Cada registro representa uma ocorrência de um organismo e contém informações relacionadas à sua **identificação taxonômica, localização geográfica e data da ocorrência**.

Os dados foram obtidos em formato estruturado, como **CSV**, e será utilizado um subconjunto dos registros disponíveis para adequação aos requisitos do projeto.

Inicialmente, será utilizado um conjunto de aproximadamente **500.000 registros**, podendo chegar a **879.726 registros** nos testes de maior escala.

---

## 1.2 Fonte

O dataset foi obtido por meio do **GBIF (Global Biodiversity Information Facility)**, utilizando registros de ocorrência classificados no grupo taxonômico **Fungi**.

🔗 **Fonte:**
https://www.gbif.org/pt/occurrence/search?taxonKey=F

---

## 1.3 Estrutura dos Dados

Foram utilizados os principais atributos disponíveis nos registros para representar cada ocorrência.

| Atributo         | Tipo    | Descrição                                     |
| ---------------- | ------- | --------------------------------------------- |
| `gbifID`         | Inteiro | Identificador único do registro de ocorrência |
| `scientificName` | String  | Nome científico do organismo                  |
| `kingdom`        | String  | Reino                                         |
| `phylum`         | String  | Filo                                          |
| `class`          | String  | Classe                                        |
| `order`          | String  | Ordem                                         |
| `family`         | String  | Família                                       |
| `genus`          | String  | Gênero                                        |
| `species`        | String  | Espécie, se disponível                        |
| `country`        | String  | País onde a ocorrência foi registrada         |
| `latitude`       | Float   | Latitude da ocorrência                        |
| `longitude`      | Float   | Longitude da ocorrência                       |
| `year`           | Inteiro | Ano do registro                               |

Para atender ao requisito de chave composta, as operações de inserção, busca e remoção utilizarão uma chave formada pela combinação dos campos (scientificName, year, gbifID). O comparador examinará os campos nesta ordem: primeiro scientificName (em ordem lexicográfica), depois year (em ordem numérica) e, por fim, gbifID (em ordem numérica). O resultado do comparador será negativo quando a primeira chave vier antes da segunda, zero quando todos os componentes forem iguais e positivo quando vier depois.

Registros sem scientificName ou sem year serão descartados na carga dos dados, pois ambos fazem parte da chave.

A mesma função de comparação será utilizada pela BST e pela AVL, garantindo que as duas estruturas apliquem exatamente a mesma regra. Uma chave será considerada repetida somente quando os três componentes forem iguais. O gbifID isolado poderá continuar armazenado como atributo do registro, mas não será usado sozinho como chave de comparação do projeto. Nos testes de duplicidade, serão criadas cópias artificiais de registros para forçar a repetição da mesma chave composta. A regra adotada será rejeitar a inserção quando a chave já existir, sem criar um segundo nó. Registros com mesmo scientificName e mesmo year, mas com outro gbifID, serão considerados chaves diferentes.


---

## 1.4 Justificativa da Escolha

O dataset foi escolhido por seu alto volume e variedade de atributos, permitindo avaliar as estruturas com entradas de diferentes tamanhos e tipos de várias maneiras. A chave composta (scientificName, year, gbifID) também permite implementar e testar um comparador personalizado, exigindo comparação de texto e de valores numéricos. Os testes considerarão tanto entradas aleatórias e ordenadas, chaves repetidas artificialmente e todos os tipos de operações como inserção, busca e remoção. Serão observados o tempo de execução, a altura da árvore, o número de comparações e, na AVL, o número de rotações.

---

# 2. 🌳 Estrutura(s) de Árvore Escolhida(s)

## 2.1 Estrutura(s)

foram utilizadas duas estruturas de árvore para organização dos registros do dataset:

Árvore Binária de Busca (BST — Binary Search Tree);
Árvore AVL (Adelson-Velsky e Landis).


---

## 2.2 Justificativa Técnica

A escolha da BST e da AVL nos permite analisar o comportamento das árvores em diferentes condições de entrada.

A BST possui uma estrutura relativamente simples e organiza os registros pela chave composta (scientificName, year, gbifID), comparada pela função descrita na seção 1.3. Quando os registros chegam em uma ordem que mantém a árvore aproximadamente equilibrada (por exemplo, ordem aleatória), as operações de busca, inserção e remoção apresentam complexidade média de O(log n). Entretanto, uma BST sem balanceamento, ao receber registros já ordenados pela chave composta, pode atingir altura próxima de n, fazendo com que essas operações apresentem complexidade O(n) no pior caso.

A AVL, por outro lado, mantém a árvore balanceada por meio de rotações após inserções e remoções. Seu objetivo é manter a altura em ordem logarítmica em relação ao número de elementos, evitando que a estrutura se transforme em uma lista encadeada.

A escolha conjunta das duas estruturas permite analisar a diferença entre:

- uma árvore de busca sem balanceamento automático, representada pela BST;
- uma árvore de busca auto-balanceada, representada pela AVL.

Essa comparação é adequada ao dataset porque o conjunto possui centenas de milhares de registros e a chave começa por um texto (scientificName). Como cada comparação de chaves envolve comparação de strings, a altura da árvore tem impacto direto no custo de localizar ou modificar um registro. Além disso, o GBIF possui muitas ocorrências para o mesmo scientificName, de modo que as comparações frequentemente avançam até year e gbifID, exercitando o comparador completo.

---

## 2.3 Operações Implementadas

As operações previstas para a implementação são:

* [x] Inserção de registro usando chave composta;
* [x] Remoção de registros usando chave composta;
* [x] Busca de registro usando a chave composta;
* [x] Percurso em pré-ordem, em ordem e em pós-ordem;
* [x] Cálculo da altura e contagem de comparações;
* [x] Na AVL, contagem de rotações e verificação do balanceamento;
* [x] Verificação de consistência: propriedade de busca (percurso em ordem) nas duas árvores e fator de balanceamento na AVL;


---

## 2.4 Complexidade

A complexidade teórica das operações de inserção, busca e remoção depende da estrutura utilizada e, no caso da BST, principalmente da altura da árvore.

## BST

| Operação | Melhor caso | Caso médio	| Pior caso |
|---|---|---|---|
| Inserção	| O(1)	| O(log n)	| O(n) |
| Busca	| O(1)	| O(log n)	| O(n) |
| Remoção	| O(1)	| O(log n)	| O(n) |

Na BST, o melhor caso ocorre quando a operação encontra diretamente a posição necessária ou quando a árvore possui uma configuração favorável. No caso médio, considerando uma distribuição aproximadamente equilibrada, as operações apresentam comportamento O(log n). No pior caso, a árvore pode ficar degenerada, com altura próxima de n, levando as operações para O(n).

## AVL
| Operação | Melhor caso | Caso médio | Pior caso |
|---|---|---|---|
| Inserção | O(1) |	O(log n) | O(log n) |
| Busca | O(1) | O(log n) |	O(log n) |
| Remoção |	O(1) | O(log n) | O(log n) |

Na AVL, o balanceamento mantém a altura da árvore em ordem O(log n). Dessa forma, as operações de busca, inserção e remoção possuem limite assintótico O(log n) no pior caso.

| Complexidade	| Ideia simples |
|---|---|
| O(1) | Não depende do tamanho dos dados | 
| O(log n) | Cresce muito lentamente |
| O(n) | Cresce proporcionalmente aos dados |


---

# 3. 🧪 Plano de Testes

## 3.1 Objetivo dos Testes

Os testes terão como objetivo verificar a corretude da BST e da AVL e comparar seu comportamento sob as mesmas entradas e operações. Para cada conjunto de dados, as duas árvores receberão os mesmos registros e serão submetidas à mesma sequência de operações. Todas as operações usam a chave composta (scientificName, year, gbifID).

Serão considerados os seguintes objetivos:

- Verificar se cada chave composta inserida é encontrada pela busca;
- Verificar a regra de duplicidade: uma chave repetida (os três componentes iguais) é rejeitada sem criar novo nó, e chaves que diferem em apenas um componente são aceitas;
- Verificar se a remoção de uma chave existente mantém a árvore consistente e se a remoção de uma chave inexistente não altera a árvore;
- Verificar automaticamente, após inserções e remoções, a propriedade de busca nas duas árvores (o percurso em ordem deve produzir as chaves em ordem estritamente crescente segundo o comparador) e, na AVL, o fator de balanceamento entre -1 e 1 em todos os nós;
- Comparar a BST e a AVL sob as mesmas entradas e operações, com diferentes distribuições de dados;
- Observar a altura das estruturas, o número de comparações, o tempo de execução e, na AVL, o número de rotações;
- Avaliar casos extremos, como árvore com um único elemento.

Todas as medições serão feitas no mesmo computador, recém-ligado e sem outros programas em execução, para que o tempo medido reflita o comportamento das árvores e não o ruído da máquina. Cada medição será repetida cinco vezes; a primeira execução (aquecimento) será descartada e será reportada a mediana das quatro restantes. A mesma regra vale para a BST e para a AVL.


---

## 3.2 Cenários de Teste

Em todos os cenários, os mesmos registros e a mesma sequência de operações são aplicados à BST e à AVL.

| # | Cenário | Entrada | Operações | Resultado esperado | Status |
|---|---------|---------|-----------|--------------------|--------|
| 1=A | Inserção em ordem aleatória | Registros embaralhados; 50 mil, 100 mil, 250 mil, 500 mil e 879.726 | Inserção de todos os registros; busca de todas as chaves inseridas | BST com altura da ordem de 2 a 3 vezes log₂n; AVL com altura entre ⌊log₂ n⌋ e 1,44·log₂n; todas as buscas encontram a chave | ☑ |
| 2=B | Inserção em ordem crescente | Registros ordenados pela chave composta (scientificName, year, gbifID) antes da inserção; AVL com todos os tamanhos e BST com 50 mil | Inserção de todos os registros; busca da primeira e da última chave | BST degenera em lista (altura igual a n-1, busca O(n)); AVL mantém altura logarítmica com número de rotações da ordem de n | ☑ |
| 3=C | Buscas e remoções intercaladas | Árvore montada com inserção aleatória (cenário 1) em 50 mil, 100 mil, 250 mil, 500 mil e 879.726 registros | Buscas de 10% de chaves existentes e 10% de inexistentes, intercaladas com a remoção de 10% das chaves | Buscas de chaves existentes encontram o registro e as de inexistentes não; árvores consistentes após as remoções; AVL com menos comparações por operação que a BST e com fator de balanceamento entre -1 e 1 | ☑ |
| 4=D | Duplicatas forçadas | Dataset do cenário 1 (50 mil registros) mais cópias artificiais de 1% dos registros, com a mesma chave composta | Tentativa de inserir as cópias; inserção de registros com mesmo scientificName e year, mas gbifID diferente | Cópias rejeitadas, sem criar nó e sem alterar o número de elementos; registros que diferem apenas no gbifID aceitos | ☑ |

Observação: a inserção da BST será implementada de forma iterativa, e o cenário 2 limita a BST a tamanhos menores, pois a inserção de n chaves ordenadas custa O(n²) comparações e a recursão em uma árvore degenerada pode estourar a pilha.


---

## 3.3 Casos Extremos (Edge Cases)

Serão considerados os seguintes casos extremos:

- 1️⃣ Árvore com um único elemento: verificar inserção, busca e remoção de um único registro;
- 🔁 Dados duplicados: verificar a rejeição de registros com a mesma chave composta (scientificName, year, gbifID);
- 🔀 Chaves que diferem em um único componente: registros com mesmo scientificName e year, mas com gbifID diferente devem ser aceitos como chaves distintas;
- ⬆️ Dados em ordem crescente: inserir registros previamente ordenados pela chave composta, pois a ordem do gbifID sozinha não produz uma sequência ordenada para o comparador, que começa por scientificName;
- 📦 Grande volume de dados: utilizar o maior conjunto previsto (879.726 registros) em ordem aleatória, para verificar o comportamento das estruturas em grande escala;
- 🔎 Busca de elemento existente e inexistente: verificar se um registro presente é encontrado e se a ausência de um registro é identificada corretamente;
- 🗑️ Remoção de elemento existente e inexistente: verificar a remoção de registros presentes (folha, nó com um filho e nó com dois filhos) e se a árvore permanece consistente ao tentar remover uma chave inexistente.

Os dados em ordem crescente são especialmente relevantes para a BST, pois produzem uma árvore altamente desbalanceada e aproximam seu comportamento do pior caso O(n). Na AVL, esse mesmo cenário permite verificar a manutenção do balanceamento por meio das rotações.

---

## 3.4 Testes de Desempenho

Os testes de desempenho serão realizados utilizando diferentes tamanhos de entrada, inicialmente considerando:

* **50.000 registros**
* **100.000 registros**
* **250.000 registros**
* **500.000 registros**
* **879.726 registros**

Serão avaliados tempo de execução, número de comparações, altura da árvore e, quando aplicável, número de rotações. A altura é medida em arestas (uma árvore com um único nó tem altura 0)
Observação: os tamanhos acima valem para os cenários 1 e 3 nas duas árvores e para o cenário 2 na AVL. No cenário 2, a BST será testada apenas com 50.000 registros assim como o cenário 4, conforme a seção 3.2.

---

## 3.5 Resultados Obtidos

### Ambiente das medições

- **Computador:** Intel Core i7-13650HX, 16 GB DDR5, NVIDIA GeForce RTX 3050 6GB Laptop GPU (a GPU não é usada).
- **Sistema operacional:** Windows 11.
- **Compilador:** GCC 14.2.0, com `-std=c11 -Wall -Wextra -O2`.
- **Dataset:** `dataset/ocorrencias.csv`, 879.726 registros válidos.
- **Semente:** 42. **Repetições:** 5 (a primeira é descartada como aquecimento; reporta-se a mediana das 4 restantes).
- **Comando:** `.\teste_bst.exe dataset/ocorrencias.csv --tamanhos 50000,100000,250000,500000,879726` (idem para `teste_avl.exe`).
- **Condição:** computador recém-ligado e sem outros programas em execução (rodada oficial, 03/11/2026).
- **Convenções:** o tempo é o tempo decorrido medido com `clock()`, com resolução de cerca de 1 ms; por isso valores abaixo de 1 ms são reportados como "<1 ms". A altura é medida em arestas (árvore com um único nó tem altura 0). Nos CSVs, os cenários 1 a 4 aparecem como A a D.
- **Arquivos:** `resultados/bst_resultados.csv`, `resultados/avl_resultados.csv` e o registro bruto em `resultados/README.md`.

### Resultados oficiais

**Cenário 1 (A): inserção em ordem aleatória**

| n | Tempo BST (s) | Tempo AVL (s) | Comp. BST | Comp. AVL | Altura BST | Altura AVL | Rotações AVL |
|---:|---:|---:|---:|---:|---:|---:|---:|
| 50.000 | 0,0380 | 0,0280 | 916.401 | 725.137 | 39 | 18 | 34.861 |
| 100.000 | 0,0905 | 0,0640 | 1.971.210 | 1.551.560 | 42 | 19 | 70.083 |
| 250.000 | 0,3085 | 0,2195 | 5.388.165 | 4.214.920 | 44 | 21 | 174.326 |
| 500.000 | 0,8445 | 0,5295 | 11.467.364 | 8.935.299 | 46 | 22 | 348.405 |
| 879.726 | 1,6055 | 1,0845 | 21.168.198 | 16.447.833 | 49 | 23 | 613.706 |

Todas as buscas encontraram suas chaves (0 buscas inesperadas) e a validação foi OK em todos os tamanhos.

**Cenário 2 (B): inserção em ordem crescente**

| Árvore | n | Tempo (s) | Comparações | Altura | Rotações |
|---|---:|---:|---:|---:|---:|
| BST | 50.000 | 43,5735 | 1.249.975.000 | 49.999 | 0 |
| AVL | 50.000 | 0,0100 | 734.465 | 15 | 49.984 |
| AVL | 100.000 | 0,0200 | 1.568.929 | 16 | 99.983 |
| AVL | 250.000 | 0,0505 | 4.237.857 | 17 | 249.982 |
| AVL | 500.000 | 0,1135 | 8.975.713 | 18 | 499.981 |
| AVL | 879.726 | 0,2030 | 16.545.945 | 19 | 879.706 |

A busca da primeira e da última chave custou 50.001 comparações na BST e apenas 32 a 40 na AVL.

**Cenário 3 (C): buscas e remoções intercaladas**

| n | Tempo BST (s) | Tempo AVL (s) | Comp. BST | Comp. AVL | Altura BST | Altura AVL |
|---:|---:|---:|---:|---:|---:|---:|
| 50.000 | 0,0135 | 0,0110 | 391.702 | 307.068 | 37 | 18 |
| 100.000 | 0,0365 | 0,0270 | 840.531 | 654.125 | 41 | 19 |
| 250.000 | 0,1115 | 0,0910 | 2.283.871 | 1.766.591 | 44 | 21 |
| 500.000 | 0,2810 | 0,2365 | 4.845.657 | 3.729.460 | 46 | 22 |
| 879.726 | 0,5720 | 0,4560 | 8.914.820 | 6.877.068 | 49 | 23 |

Houve 0 buscas inesperadas e 0 remoções inesperadas, e a validação foi OK em todas as fases. Em ambas as árvores, a propriedade de busca foi mantida e, na AVL, o fator de balanceamento ficou entre -1 e 1.

**Cenário 4 (D): duplicatas forçadas (n = 50.000)**

| Árvore | Fase | Inseridas | Rejeitadas | Tamanho final | Comparações | Altura |
|---|---|---:|---:|---:|---:|---:|
| BST | copias_exatas | `<preencher>` | `<preencher>` | `<preencher>` | 9.743 | 39 |
| BST | chaves_distintas | `<preencher>` | `<preencher>` | `<preencher>` | 21.150 | 39 |
| AVL | copias_exatas | `<preencher>` | `<preencher>` | `<preencher>` | 7.459 | 18 |
| AVL | chaves_distintas | `<preencher>` | `<preencher>` | `<preencher>` | 16.123 | 18 |

As cópias exatas foram rejeitadas sem criar nó (o tamanho final não mudou), e os registros com a mesma combinação (`scientificName`, `year`) e `gbifID` diferente foram aceitos. Na AVL, a contagem de rotações é acumulada: em `copias_exatas` ela ficou em 34.861, igual à da inserção do cenário 1, ou seja, as cópias rejeitadas não provocaram nenhuma rotação. Em `chaves_distintas` ela subiu para 35.714 (853 rotações a mais).

**Verificações automáticas:** 327 na BST e 416 na AVL, com 0 falhas em ambas.

### Comparação com a complexidade teórica (Big-O)

**BST com dados ordenados: O(n²) confirmado.** Cada chave nova é maior que todas as anteriores e desce pela árvore inteira, que é uma lista. A k-ésima inserção faz k−1 comparações, e a soma é n(n−1)/2. Para n = 50.000, isso dá 1.249.975.000, exatamente o valor medido. A altura de 49.999 (= n−1) confirma a degeneração em lista. Extrapolando, (879.726 / 50.000)² ≈ 310 vezes mais trabalho levaria cerca de 13.500 s (≈ 3,7 horas) por execução. Isso justifica limitar a BST a 50.000 registros nesse cenário. É uma estimativa por escala, não uma medição.

**AVL com dados ordenados: O(log n) por operação.** A altura ficou em 15 a 19 (o mínimo possível, em arestas, é ⌊log₂ n⌋: 15 para n = 50.000 e 19 para n = 879.726), e a busca da primeira e da última chave custou apenas 32 a 40 comparações. O número de rotações é praticamente n (879.706 para 879.726 inserções), pois quase toda inserção ordenada desequilibra a árvore.

**Cenário aleatório: n log n nas duas árvores.** Ao dobrar n, as comparações crescem ≈2,15× (50 mil → 100 mil) e ≈2,13× (250 mil → 500 mil) na BST, e ≈2,14× e ≈2,12× na AVL. Para n log n o esperado é 2·(1 + 1/log₂ n) ≈ 2,11 a 2,13, portanto os dados são compatíveis. Em 879.726 registros, a BST faz em média ≈24,1 comparações por inserção e a AVL ≈18,7, ou seja, a AVL faz cerca de 22% menos comparações. Nas buscas e remoções (cenário 3) a diferença é parecida, ≈23%.

**Alturas.** Em 879.726 registros, log₂ n ≈ 19,75. A AVL aleatória ficou em 23, dentro do limite de 1,44·log₂ n ≈ 28,4. A BST aleatória ficou em 49 (≈2,5·log₂ n), dentro da faixa esperada de 2 a 3 vezes log₂ n. Já a BST ordenada degenera, com altura de n−1.

**Tempo.** A AVL foi mais rápida que a BST no cenário aleatório (1,08 s contra 1,61 s em 879.726), apesar de executar 613.706 rotações. Uma explicação plausível é que o custo dominante é a comparação de strings (`scientificName`), e a AVL faz menos comparações; porém isso não foi medido separadamente. Na AVL, a inserção ordenada (0,20 s) foi ≈5× mais rápida que a aleatória (1,08 s) com número de comparações semelhante. Uma hipótese é a melhor localidade de cache no acesso sequencial, que também não foi testada.

**Resolução do relógio.** Tempos iguais a 0,000 s indicam valores abaixo da resolução do `clock()` (≈1 ms) e não tempo zero. Como a mediana de 4 valores é a média dos dois centrais, os tempos variam em passos de 0,5 ms.

### Experimento extra: sensibilidade ao ruído

Para avaliar a influência de outros programas, repetimos as execuções com o computador ligado há algum tempo e **renderizando um vídeo** (03/11/2026, 15:15 e 15:31). Esta rodada **não faz parte dos resultados oficiais**, pois viola a condição da seção 3.1.

- **Comparações, alturas e rotações foram idênticas** às da rodada oficial em todas as fases. Isso confirma que são determinísticos (mesma semente, mesmos dados) e não dependem da máquina.
- **Os tempos aumentaram de ≈1,2× a ≈4,7×.** Por exemplo, na inserção aleatória com 879.726 registros, a BST passou de 1,61 s para 5,56 s e a AVL de 1,08 s para 4,26 s. Já o cenário 2 da BST (43,57 s → 52,96 s) variou só ≈1,2×.
- **A ordem entre as estruturas se manteve** no cenário aleatório (AVL mais rápida que a BST), mas os valores absolutos dependem muito do estado da máquina.

Conclusão: comparações, alturas e rotações são as medidas robustas; o tempo só é comparável entre execuções feitas nas mesmas condições.

### Limitações

- O tempo é o tempo decorrido (relógio de parede) medido com `clock()`, com resolução de ≈1 ms, e é sensível a outros processos.
- No cenário 2, a BST foi testada apenas com 50.000 registros, por causa do custo O(n²); o tempo para tamanhos maiores é extrapolado.
- Não foi medido o uso de memória nem foram separados os custos de comparação de strings e de manipulação de ponteiros; as explicações de tempo são hipóteses.
- O dataset é um subconjunto do GBIF (grupo Fungi), e os resultados podem diferir com outra distribuição de nomes e anos.

Os arquivos de resultados estão na pasta /resultados.

---

# 4. ⚙️ Como Executar

## 4.1 Pré-requisitos

* Linguagem: C, padrão C11 (-std=c11).
* Compilador: GCC 14.2.0.
* Flags de compilação: -std=c11 -Wall -Wextra -O2.
* Dependências: apenas a biblioteca padrão de C.
* Sistema operacional testado: Windows 11.
* Hardware usado nos testes: Intel Core i7-13650HX, 16 GB DDR5, NVIDIA GeForce RTX 3050 6GB Laptop GPU (a GPU não é usada pelo programa).
* Dataset: dataset/ocorrencias.csv, com 879.726 registros válidos extraídos do GBIF (grupo Fungi) já incluído no repositório.

---

## 4.2 Instruções

### 1. Clonar o repositório

```bash
git clone https://github.com/GuilhermeCoelho0/Fungos-das--rvores-C.git
cd Fungos-das--rvores-C
```

### 2. Compilar

```bash
gcc -std=c11 -Wall -Wextra -O2 -Isrc -Itestes -o teste_bst.exe testes/teste_bst.c testes/teste_comum.c src/bst.c src/registro.c
gcc -std=c11 -Wall -Wextra -O2 -Isrc -Itestes -o teste_avl.exe testes/teste_avl.c testes/teste_comum.c src/avl.c src/registro.c
```

Cada executável junta o programa de teste, o código comum de teste (teste_comum.c), a árvore correspondente e o módulo de registro/comparador. As opções -Isrc -Itestes permitem que os #include "bst.h" e "teste_comum.h" sejam encontrados.


### 3. Executar

```bash
.\teste_bst.exe dataset/ocorrencias.csv --tamanhos 50000,100000,250000,500000,879726
.\teste_avl.exe dataset/ocorrencias.csv --tamanhos 50000,100000,250000,500000,879726
```

Argumentos

| Argumento |	Descrição |
|---|---|
| dataset/ocorrencias.csv |	Caminho do arquivo de dados |
| --tamanhos |	Lista de tamanhos de entrada, separados por vírgula |


Sem opções adicionais, o programa roda os cenários A, B, C e D, com 5 repetições (a primeira é descartada como aquecimento) e semente 42.

Saídas: resultados/bst_resultados.csv e resultados/avl_resultados.csv.

Tempo esperado: teste_avl leva cerca de 1 a 2 minutos. teste_bst leva cerca de 5 minutos, quase todos no cenário B (inserção ordenada, O(n²)).

Para reproduzir as medições oficiais: rode com o computador recém-ligado e sem outros programas abertos, como descrito na seção 3.1.

---

## 4.3 Estrutura do Repositório

A estrutura do projeto é:

```text
Fungos-das--rvores-C/
│
├── src/
│   ├── bst.c / bst.h
│   ├── avl.c / avl.h
│   └── registro.c / registro.h
│
├── testes/
│   ├── teste_bst.c
│   ├── teste_avl.c
│   ├── teste_comum.c / .h
│
├── dataset/
│   └── ocorrencias.csv
│
├── resultados/
│   ├── bst_resultados.csv
│   ├── avl_resultados.csv
│   └── README.md
│
├── .gitignore
└── README.md
```

---

# 5. 📚 Referências

* **GBIF — Global Biodiversity Information Facility**
  Registros de ocorrência de organismos do grupo taxonômico Fungi.

  https://www.gbif.org/pt/occurrence/search?taxonKey=F

* **Materiais fornecidos pelo professor Cid Rodrigues de Andrade**
  Disciplina de Estruturas de Dados II — UNICID.

---

> 🍄 **Fungos das Árvores-C**
> Trabalho desenvolvido para a disciplina de **Estruturas de Dados II — UNICID**.

