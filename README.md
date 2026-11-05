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

Inicialmente, será utilizado um conjunto de aproximadamente **500.000 registros**, podendo chegar a **879.726 registros** válidos e incluir os filtros de país e período.

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

O dataset foi escolhido por seu alto volume de registros e pela variedade de atributos, permitindo avaliar o comportamento das estruturas com diferentes tamanhos e distribuições de entrada. A chave composta (`scientificName`, `year`, `gbifID`) possibilitou implementar e testar um comparador personalizado, que envolve comparações de texto e de valores numéricos.

Nos testes realizados, foram utilizadas entradas em ordem aleatória e crescente, além de chaves repetidas artificialmente para verificar o tratamento de duplicidades. Também foram avaliadas as operações de inserção, busca e remoção. Para comparar o desempenho da BST e da AVL, foram observados o tempo de execução, a altura das árvores, o número de comparações e, no caso da AVL, o número de rotações.

---

# 2. 🌳 Estrutura(s) de Árvore Escolhida(s)

## 2.1 Estrutura(s)

Foram utilizadas duas estruturas de árvore para a organização dos registros do dataset:

- **Árvore Binária de Busca (BST — Binary Search Tree):** estrutura em que cada nó possui, no máximo, dois filhos. Os valores menores que a chave do nó ficam na subárvore esquerda, enquanto os maiores ficam na subárvore direita. Essa organização permite realizar operações de busca, inserção e remoção, mas seu desempenho depende da distribuição e da ordem de inserção dos dados. Em casos desfavoráveis, a árvore pode ficar desbalanceada.

- **Árvore AVL (Adelson-Velsky e Landis):** é uma árvore binária de busca autobalanceada. Para cada nó, o fator de balanceamento é calculado pela diferença entre as alturas das subárvores esquerda e direita. Esse fator deve permanecer entre -1 e 1. Quando uma inserção ou remoção provoca um desequilíbrio, a estrutura realiza rotações para recuperar o balanceamento. Existem quatro casos de rotação: **simples à esquerda, simples à direita, dupla esquerda-direita e dupla direita-esquerda**.

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

Foram implementadas as seguintes operações nas estruturas BST e AVL:

* [x] Inserção de registros usando chave composta;
* [x] Remoção de registros usando chave composta;
* [x] Busca de registros usando chave composta;
* [x] Percursos em pré-ordem, em ordem e em pós-ordem;
* [x] Cálculo da altura e contagem de comparações;
* [x] Na AVL, contagem de rotações e verificação do balanceamento;
* [x] Verificação de consistência: propriedade de busca (percurso em ordem) nas duas árvores e fator de balanceamento na AVL.

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

Os testes tiveram como objetivo verificar a corretude da BST e da AVL e comparar seu comportamento sob as mesmas entradas e operações. Para cada conjunto de dados, as duas árvores receberam os mesmos registros e foram submetidas à mesma sequência de operações. Todas as operações utilizaram a chave composta (`scientificName`, `year`, `gbifID`).

Foram considerados os seguintes objetivos:

- Verificar se cada chave composta inserida foi encontrada pela busca;
- Verificar a regra de duplicidade: uma chave repetida (os três componentes iguais) foi rejeitada sem criar um novo nó, enquanto chaves que diferiam em apenas um componente foram aceitas;
- Verificar se a remoção de uma chave existente manteve a árvore consistente e se a remoção de uma chave inexistente não alterou a árvore;
- Verificar automaticamente, após inserções e remoções, a propriedade de busca nas duas árvores (o percurso em ordem deveria produzir as chaves em ordem estritamente crescente segundo o comparador) e, na AVL, o fator de balanceamento entre -1 e 1 em todos os nós;
- Comparar a BST e a AVL sob as mesmas entradas e operações, com diferentes distribuições de dados;
- Observar a altura das estruturas, o número de comparações, o tempo de execução e, na AVL, o número de rotações;
- Avaliar casos extremos, como uma árvore com um único elemento.

Todas as medições foram realizadas no mesmo computador, recém-ligado e sem outros programas em execução, para reduzir a interferência de outros processos nos tempos registrados. Cada medição foi repetida cinco vezes. A primeira execução, considerada de aquecimento, foi descartada, e a mediana das quatro execuções restantes foi utilizada como resultado. A mesma regra foi aplicada à BST e à AVL.


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

Foram executados testes para verificar o comportamento da BST e da AVL em situações específicas, incluindo os seguintes casos extremos:

- 🌳 **Árvore com um único elemento:** foi inserido um único registro e, em seguida, realizadas operações de busca e remoção. Foi verificado se o registro era encontrado corretamente, se a remoção era concluída e se a árvore permanecia válida após ficar vazia.

- 🔁 **Dados duplicados:** foram inseridos registros com a mesma chave composta (`scientificName`, `year`, `gbifID`). Foi verificado se a estrutura rejeitava as duplicatas, sem criar novos nós ou alterar indevidamente a quantidade de registros.

- 🔀 **Chaves que diferem em um único componente:** foram inseridos registros com o mesmo `scientificName` e `year`, mas com valores diferentes de `gbifID`. Foi verificado se eram aceitos como chaves distintas e encontrados individualmente pelas operações de busca.

- ⬆️ **Dados em ordem crescente:** foram inseridos registros previamente ordenados pela chave composta. Na BST, esse cenário foi testado com 50 mil registros; na AVL, foi executado para os tamanhos de entrada previstos. Foram observadas a altura das árvores, a quantidade de comparações e, na AVL, as rotações realizadas para manter o balanceamento. A ordenação foi feita pela chave composta, pois ordenar apenas pelo `gbifID` não garante uma sequência crescente segundo o comparador, que começa por `scientificName`.

- 📦 **Grande volume de dados:** foi utilizado o maior conjunto disponível, com 879.726 registros em ordem aleatória. Foram observados o tempo de execução, a altura das árvores e o número de comparações, permitindo avaliar o comportamento das estruturas com uma grande quantidade de registros.

- 🔎 **Busca de elemento existente e inexistente:** foram realizadas buscas por chaves presentes e ausentes nas árvores. Foi verificado se os registros existentes eram encontrados e se as buscas por registros inexistentes retornavam corretamente a ausência do elemento.

- 🗑️ **Remoção de elemento existente e inexistente:** foram testadas remoções de nós existentes em diferentes situações: folha, nó com um filho e nó com dois filhos. Também foram realizadas tentativas de remoção de chaves inexistentes. Após as operações, foi verificada a consistência das árvores, incluindo a propriedade de busca nas duas estruturas e o balanceamento da AVL.

Os dados em ordem crescente foram especialmente relevantes para a BST, pois produziram uma árvore altamente desbalanceada, aproximando seu comportamento do pior caso, com complexidade O(n) para operações como busca, inserção e remoção. Na AVL, o mesmo cenário permitiu verificar a manutenção do balanceamento por meio das rotações.

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

Foram realizadas duas rodadas completas de testes, ambas em 03/11/2026, com o mesmo computador, o mesmo dataset e a mesma semente:

- **Rodada 1 (oficial):** computador recém-ligado e sem outros programas em execução, conforme a condição definida na seção 3.1.
- **Rodada 2 (experimento complementar):** computador ligado havia algum tempo e com o DaVinci Resolve renderizando um vídeo. Ela mede a sensibilidade das medições ao ambiente e **não substitui** os resultados oficiais.

Todas as tabelas de "Resultados oficiais" vêm da rodada 1. Os tempos da rodada 2 aparecem somente na seção "Experimento complementar", sempre identificados.

### Ambiente das medições

- **Computador:** Intel Core i7-13650HX, 16 GB DDR5, NVIDIA GeForce RTX 3050 6GB Laptop GPU (a GPU não é usada).
- **Sistema operacional:** Windows 11.
- **Compilador:** GCC 14.2.0, com `-std=c11 -Wall -Wextra -O2`.
- **Dataset:** dataset completo `dataset/ocorrencias.csv`, com 879.726 registros válidos (GBIF, grupo Fungi; Finlândia, Noruega e Dinamarca; 2020 a 2022). Esse arquivo foi usado apenas localmente e **não está no repositório**, que contém somente a amostra de demonstração descrita na seção 4.1.
- **Semente:** 42. **Repetições:** 5 (a primeira é descartada como aquecimento; reporta-se a mediana das 4 restantes).
- **Comando:** `.\teste_bst.exe dataset/ocorrencias.csv --tamanhos 50000,100000,250000,500000,879726` (idem para `teste_avl.exe`).
- **Rodada 1 (oficial):** 03/11/2026, BST às 11:12 e AVL às 11:17, com o computador recém-ligado e sem outros programas em execução.
- **Rodada 2 (complementar):** 03/11/2026, BST às 15:15 e AVL às 15:31, com o computador ligado havia algum tempo e o DaVinci Resolve renderizando um vídeo.
- **Convenções:** o tempo é o tempo decorrido medido com `clock()`, com resolução de cerca de 1 ms; tempos abaixo disso aparecem como 0,000 s. A altura é medida em arestas (árvore com um único nó tem altura 0). Nos CSVs, os cenários 1 a 4 aparecem como A a D.
- **Arquivos:**
  - `resultados/README.md`: registro bruto das duas rodadas, com todas as tabelas. É a **única fonte dos tempos oficiais** (rodada 1).
  - `resultados/bst_resultados.csv` e `resultados/avl_resultados.csv`: saída dos programas, que a cada execução sobrescreve o arquivo anterior. Os CSVs presentes no repositório correspondem à **rodada 2** (complementar). Por isso, os tempos deles diferem dos oficiais, enquanto comparações, alturas, rotações e contagens são idênticas nas duas rodadas.

### Resultados oficiais (rodada 1)

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
| BST | copias_exatas | 0 | 500 | 50.000 | 9.743 | 39 |
| BST | chaves_distintas | 1.000 | 0 | 51.000 | 21.150 | 39 |
| AVL | copias_exatas | 0 | 500 | 50.000 | 7.459 | 18 |
| AVL | chaves_distintas | 1.000 | 0 | 51.000 | 16.123 | 18 |

As 500 cópias exatas (1% dos 50.000 registros) foram rejeitadas sem criar nó, e o tamanho final continuou em 50.000. Os 1.000 registros com a mesma combinação (`scientificName`, `year`) e `gbifID` diferente foram aceitos, e o tamanho final passou para 51.000. Na AVL, a contagem de rotações é acumulada: em `copias_exatas` ela ficou em 34.861, igual à da inserção do cenário 1, ou seja, as cópias rejeitadas não provocaram nenhuma rotação. Em `chaves_distintas` ela subiu para 35.714 (853 rotações a mais).

**Verificações automáticas:** 327 na BST e 416 na AVL, com 0 falhas em ambas.

### Comparação com a complexidade teórica (Big-O)

**BST com dados ordenados: O(n²) confirmado.** Cada chave nova é maior que todas as anteriores e desce pela árvore inteira, que é uma lista. A k-ésima inserção faz k−1 comparações, e a soma é n(n−1)/2. Para n = 50.000, isso dá 1.249.975.000, exatamente o valor medido. A altura de 49.999 (= n−1) confirma a degeneração em lista. Extrapolando, (879.726 / 50.000)² ≈ 310 vezes mais trabalho levaria cerca de 13.500 s (≈ 3,7 horas) por execução. Isso justifica limitar a BST a 50.000 registros nesse cenário. É uma estimativa por escala, não uma medição.

**AVL com dados ordenados: O(log n) por operação.** A altura ficou em 15 a 19 (o mínimo possível, em arestas, é ⌊log₂ n⌋: 15 para n = 50.000 e 19 para n = 879.726), e a busca da primeira e da última chave custou apenas 32 a 40 comparações. O número de rotações é praticamente n (879.706 para 879.726 inserções), pois quase toda inserção ordenada desequilibra a árvore.

**Cenário aleatório: n log n nas duas árvores.** Ao dobrar n, as comparações crescem ≈2,15× (50 mil → 100 mil) e ≈2,13× (250 mil → 500 mil) na BST, e ≈2,14× e ≈2,12× na AVL. Para n log n o esperado é 2·(1 + 1/log₂ n) ≈ 2,11 a 2,13, portanto os dados são compatíveis. Em 879.726 registros, a BST faz em média ≈24,1 comparações por inserção e a AVL ≈18,7, ou seja, a AVL faz cerca de 22% menos comparações. Nas buscas e remoções (cenário 3) a diferença é parecida, ≈23%.

**Alturas.** Em 879.726 registros, log₂ n ≈ 19,75. A AVL aleatória ficou em 23, dentro do limite de 1,44·log₂ n ≈ 28,4. A BST aleatória ficou em 49 (≈2,5·log₂ n), dentro da faixa esperada de 2 a 3 vezes log₂ n. Já a BST ordenada degenera, com altura de n−1.

**Tempo.** Na rodada oficial, a AVL foi mais rápida que a BST no cenário aleatório (1,08 s contra 1,61 s em 879.726), apesar de executar 613.706 rotações. Uma explicação plausível é que o custo dominante é a comparação de strings (`scientificName`), e a AVL faz menos comparações; porém isso não foi medido separadamente. Na AVL, a inserção ordenada (0,20 s) foi ≈5× mais rápida que a aleatória (1,08 s) com número de comparações semelhante. Uma hipótese é a melhor localidade de cache no acesso sequencial, que também não foi testada.

**Resolução do relógio.** Tempos iguais a 0,000 s indicam valores abaixo da resolução do `clock()` (≈1 ms) e não tempo zero. Como a mediana de 4 valores é a média dos dois centrais, os tempos variam em passos de 0,5 ms.

### Experimento complementar: sensibilidade ao ambiente (rodada 2)

Para avaliar a influência de outros programas em execução sobre os tempos medidos, foi realizada uma segunda rodada completa de testes em 03/11/2026. Nessa rodada, o computador já estava ligado havia algum tempo e o **DaVinci Resolve estava renderizando um vídeo**. Os testes da BST foram executados às **15:15**, enquanto os testes da AVL foram executados às **15:31**.

Essa rodada teve caráter exclusivamente complementar e **não substituiu os resultados oficiais**, pois as condições de execução foram diferentes das estabelecidas na seção 3.1. Os resultados foram utilizados para observar a sensibilidade dos tempos de execução ao ambiente da máquina.

**Comparação dos tempos no cenário A — inserção em ordem aleatória (879.726 registros):**

| Estrutura | Rodada oficial | Rodada com vídeo | Aumento |
|---|---:|---:|---:|
| BST | 1,6055 s | 5,5620 s | 3,5× |
| AVL | 1,0845 s | 4,2610 s | 3,9× |

Os resultados mostram que os tempos de execução aumentaram consideravelmente na segunda rodada, mesmo com as mesmas entradas e configurações de teste.

As comparações, alturas, rotações e contagens de inserções, rejeições e remoções permaneceram idênticas às da rodada oficial. Isso demonstra que essas métricas não foram afetadas pelo estado da máquina nas condições testadas.

Portanto, os resultados da segunda rodada reforçam a importância de manter condições controladas durante as medições de desempenho. Por esse motivo, a comparação principal entre BST e AVL neste trabalho utiliza exclusivamente os resultados da rodada oficial.

O que foi observado:

- **Comparações, alturas, rotações e contagens de inserções, rejeições e remoções foram idênticas** às da rodada oficial em todas as fases. Isso confirma que são determinísticas (mesma semente, mesmos dados) e não dependem do estado da máquina.
- **Os tempos aumentaram de ≈1,2× a ≈7×**, considerando as fases com tempo oficial de pelo menos 10 ms (abaixo disso a resolução do relógio impede a comparação). O menor aumento foi o da BST no cenário 2 e no cenário 3 (≈1,2×) e o maior foi o da AVL na busca de todas as chaves com n = 50.000 (de 0,023 s para 0,162 s).
- **A ordem de desempenho entre as estruturas dependeu do cenário e do estado da máquina.** Dividindo o tempo da AVL pelo da BST (valores menores que 1 indicam AVL mais rápida):
  - na inserção aleatória (cenário 1), a AVL foi mais rápida nas duas rodadas (0,63 a 0,74 na rodada 1; 0,62 a 0,82 na rodada 2);
  - na busca de todas as chaves do cenário 1, a vantagem da AVL (0,71 a 0,80 na rodada 1) praticamente desapareceu na rodada 2 (0,90 a 1,05), com a AVL ligeiramente mais lenta em n = 50.000 e n = 500.000;
  - no cenário 3, a AVL foi mais rápida na rodada 1 (0,74 a 0,84) e **mais lenta** na rodada 2 (2,70 a 3,09), embora o número de comparações fosse o mesmo. Os dados não permitem explicar essa inversão e ela não foi investigada; tomada isoladamente, a rodada 2 não é uma base confiável para comparar o tempo das duas árvores.

Conclusão: comparações, alturas e rotações são as medidas robustas; o tempo só é comparável entre execuções feitas nas mesmas condições, e por isso a comparação entre BST e AVL neste trabalho se apoia nos resultados oficiais.

### Limitações

- O tempo de execução foi medido com `clock()`, com resolução de aproximadamente 1 ms, sendo sensível à execução de outros processos, como demonstrado na rodada 2.
- Foi realizada uma única rodada oficial e uma rodada complementar. Portanto, os tempos oficiais possuem repetições dentro de uma mesma sessão, mas não foram comparados entre diferentes sessões oficiais.
- No cenário 2, a BST foi testada apenas com 50.000 registros devido ao custo computacional O(n²) da inserção ordenada. Os tempos para tamanhos maiores foram estimados por extrapolação, e não medidos diretamente.
- Não foram realizadas medições de uso de memória nem separados os custos das comparações de strings e da manipulação de ponteiros. Portanto, as explicações sobre as diferenças de tempo entre as estruturas são hipóteses.
- O dataset completo utilizado nos experimentos é um subconjunto de dados do GBIF, referente ao grupo Fungi, com registros da Finlândia, Noruega e Dinamarca, entre 2020 e 2022. Os resultados podem variar com outras distribuições de nomes científicos e anos.
- O repositório disponibiliza apenas uma amostra de demonstração, com aproximadamente 385 registros. Dessa forma, executar os programas diretamente com os dados disponíveis no GitHub **não reproduz os resultados apresentados neste relatório**, que foram obtidos com 879.726 registros. Para reproduzir os experimentos, é necessário gerar o dataset completo diretamente no GBIF, utilizando os mesmos filtros de seleção descritos na seção 4.1.

Os arquivos de resultados estão disponíveis na pasta `resultados/`.

---

# 4. ⚙️ Como Executar

## 4.1 Pré-requisitos

* Linguagem: C, padrão C11 (-std=c11).
* Compilador: GCC 14.2.0.
* Flags de compilação: -std=c11 -Wall -Wextra -O2.
* Dependências: apenas a biblioteca padrão de C.
* Sistema operacional testado: Windows 11.
* Hardware usado nos testes: Intel Core i7-13650HX, 16 GB DDR5, NVIDIA GeForce RTX 3050 6GB Laptop GPU (a GPU não é usada pelo programa).
* Dataset de demonstração (incluído no repositório): `dataset/ocorrencias.csv`, com aproximadamente 385 registros de fungos (grupo Fungi) do GBIF, filtrados para a Islândia em 2021. É uma amostra pequena, disponibilizada apenas para que qualquer pessoa possa clonar o repositório e executar os programas sem baixar o dataset completo. **Os resultados de desempenho deste README não foram obtidos com ela.**
* Dataset completo (não incluído no repositório): `dataset/ocorrencias.csv` com 879.726 registros válidos de fungos (grupo Fungi) do GBIF, filtrados para Finlândia, Noruega e Dinamarca no período de 2020 a 2022. Esse arquivo ficou disponível apenas localmente e foi o utilizado em todas as medições da seção 3.5. Para reproduzir os resultados, é preciso gerá-lo no GBIF com os mesmos filtros e colocá-lo em `dataset/` no lugar da amostra.

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
│   ├── teste_comum.c / teste_comum.h
│
├── dataset/
│   └── ocorrencias.csv # amostra de demonstração (384 registros), o CSV completo não é versionado
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
## 4.4 Configuração do ambiente
Vídeo demonstrando como configurar o ambiente e executar os comandos necessários.

https://github.com/user-attachments/assets/d8a3bdb7-eb87-4b8a-997c-73f51a037b72

---
### 4.5 Execução dos testes da BST
Vídeo curto mostrando a execução dos testes da Árvore Binária de Busca (BST).

https://github.com/user-attachments/assets/a70f498f-ef7e-4a14-8b19-4c72b47b6bec


---

## 4.6 Execução dos testes da AVL
Vídeo curto mostrando a execução dos testes da Árvore AVL.


https://github.com/user-attachments/assets/5c67018b-4fb4-4eb0-9b59-a852473c5d05

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





