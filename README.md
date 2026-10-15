# Fungos-das--rvores-C
# 🍄 Fungos das Árvores-C

**Entrega 1 — Trabalho da disciplina Estruturas de Dados II**
**Universidade Cidade de São Paulo (UNICID)**
**Professor:** Cid Rodrigues de Andrade

---

## 👥 Integrantes do Grupo

| Nome completo                      |         RA |
| ---------------------------------- | ---------: |
| **Guilherme Coelho Silva**         |   42966515 |
| **Maria Luisa Assunção Novaes**    |   42802881 |
| **Miguel Targino Felix**          |   43079067 |
| **Thiago Salles de Almeida Silva** |   43461905 |

---

# 1. 📊 Dataset

## 1.1 Descrição

O conjunto de dados utilizado nos testes será composto por **registros de ocorrência de fungos** obtidos do **Global Biodiversity Information Facility (GBIF)**, uma infraestrutura internacional que disponibiliza dados sobre biodiversidade.

O domínio do dataset é a **biodiversidade de espécies de fungos**. Cada registro representa uma ocorrência de um organismo e contém informações relacionadas à sua **identificação taxonômica, localização geográfica e data da ocorrência**.

Os dados serão obtidos em formato estruturado, como **CSV**, e será utilizado um subconjunto dos registros disponíveis para adequação aos requisitos do projeto.

Inicialmente, será utilizado um conjunto de aproximadamente **500.000 registros**, podendo chegar a **1.000.000 de registros** nos testes de maior escala.

---

## 1.2 Fonte

O dataset será obtido por meio do **GBIF (Global Biodiversity Information Facility)**, utilizando registros de ocorrência classificados no grupo taxonômico **Fungi**.

🔗 **Fonte:**
https://www.gbif.org/occurrence/search?taxonKey=5

---

## 1.3 Estrutura dos Dados

Serão utilizados os principais atributos disponíveis nos registros para representar cada ocorrência.

| Atributo         | Tipo    | Descrição                                     |
| ---------------- | ------- | --------------------------------------------- |
| `gbifID`         | Inteiro | Identificador único do registro de ocorrência |
| `scientificName` | String  | Nome científico do organismo                  |
| `kingdom`        | String  | Classificação taxonômica                      |
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

Serão utilizadas duas estruturas de árvore para organização dos registros do dataset:

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

* [ ] Inserção de registro usando chave composta;
* [ ] Remoção de registros usando chave composta;
* [ ] Busca de registro usando a chave composta;
* [ ] Percurso em pré-ordem, em ordem e em pós-ordem;
* [ ] Cálculo da altura e contagem de comparações;
* [ ] Na AVL, contagem de rotações e verificação do balanceamento;
* [ ] Outra: a definir;
* [ ] Verificação de consistência: propriedade de busca (percurso em ordem) nas duas árvores e fator de balanceamento na AVL;

> **Observação:** operações referentes à **Entrega 2** serão preenchidas conforme a implementação do projeto.

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

Complexidade	Ideia simples <br>
O(1)	Não depende do tamanho dos dados <br>
O(log n)	Cresce muito lentamente <br>
O(n)	Cresce proporcionalmente aos dados <br>


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
- Avaliar casos extremos, como árvore vazia e árvore com um único elemento.

Todas as medições serão feitas no mesmo computador, recém-ligado e sem outros programas em execução, para que o tempo medido reflita o comportamento das árvores e não o ruído da máquina. Cada medição será repetida cinco vezes; a primeira execução (aquecimento) será descartada e será reportada a mediana das quatro restantes. A mesma regra vale para a BST e para a AVL.


---

## 3.2 Cenários de Teste

Em todos os cenários, os mesmos registros e a mesma sequência de operações são aplicados à BST e à AVL.

| # | Cenário | Entrada | Operações | Resultado esperado | Status |
|---|---------|---------|-----------|--------------------|--------|
| 1 | Inserção em ordem aleatória | Registros embaralhados; 50 mil, 100 mil, 250 mil, 500 mil e 1 milhão | Inserção de todos os registros; busca de todas as chaves inseridas | BST com altura da ordem de 2 a 3 vezes log2(n); AVL com altura entre log2(n) e 1,44·log2(n); todas as buscas encontram a chave | ☐ |
| 2 | Inserção em ordem crescente | Registros ordenados pela chave composta (scientificName, year, gbifID) antes da inserção; AVL com todos os tamanhos e BST com 10 mil, 20 mil e 50 mil | Inserção de todos os registros; busca da primeira e da última chave | BST degenera em lista (altura igual a n, busca O(n)); AVL mantém altura logarítmica com número de rotações da ordem de n | ☐ |
| 3 | Buscas e remoções intercaladas | Árvore montada com inserção aleatória (cenário 1) em 100 mil e 500 mil registros | Buscas de 10% de chaves existentes e 10% de inexistentes, intercaladas com a remoção de 10% das chaves | Buscas de chaves existentes encontram o registro e as de inexistentes não; árvores consistentes após as remoções; AVL com menos comparações por operação que a BST e com fator de balanceamento entre -1 e 1 | ☐ |
| 4 | Duplicatas forçadas | Dataset do cenário 1 (100 mil registros) mais cópias artificiais de 1% dos registros, com a mesma chave composta | Tentativa de inserir as cópias; inserção de registros com mesmo scientificName e year, mas gbifID diferente | Cópias rejeitadas, sem criar nó e sem alterar o número de elementos; registros que diferem em um componente da chave aceitos | ☐ |

Observação: a inserção da BST será implementada de forma iterativa, e o cenário 2 limita a BST a tamanhos menores, pois a inserção de n chaves ordenadas custa O(n²) comparações e a recursão em uma árvore degenerada pode estourar a pilha.


---

## 3.3 Casos Extremos (Edge Cases)

Serão considerados os seguintes casos extremos:

- 🌱 Árvore vazia: verificar o comportamento de busca, remoção e percursos quando nenhum registro foi inserido;
- 1️⃣ Árvore com um único elemento: verificar inserção, busca e remoção de um único registro;
- 🔁 Dados duplicados: verificar a rejeição de registros com a mesma chave composta (scientificName, year, gbifID);
- 🔀 Chaves que diferem em um único componente: registros com mesmo scientificName e year e gbifID diferente, ou mesmo scientificName e gbifID e year diferente, devem ser aceitos como chaves distintas;
- ⬆️ Dados em ordem crescente: inserir registros previamente ordenados pela chave composta, pois a ordem do gbifID sozinha não produz uma sequência ordenada para o comparador, que começa por scientificName;
- ⬇️ Dados em ordem decrescente: inserir os registros na ordem inversa da chave composta, para observar a degeneração da BST (limitada aos mesmos tamanhos do cenário 2) e as rotações da AVL;
- 📦 Grande volume de dados: utilizar o maior conjunto previsto (1 milhão de registros) em ordem aleatória, para verificar o comportamento das estruturas em grande escala;
- 🔎 Busca de elemento existente e inexistente: verificar se um registro presente é encontrado e se a ausência de um registro é identificada corretamente;
- 🗑️ Remoção de elemento existente e inexistente: verificar a remoção de registros presentes (folha, nó com um filho e nó com dois filhos) e se a árvore permanece consistente ao tentar remover uma chave inexistente.

Os dados em ordem crescente e decrescente são especialmente relevantes para a BST, pois produzem uma árvore altamente desbalanceada e aproximam seu comportamento do pior caso O(n). Na AVL, esses mesmos cenários permitem verificar a manutenção do balanceamento por meio das rotações.

---

## 3.4 Testes de Desempenho

> **Entrega 2**

Os testes de desempenho serão realizados utilizando diferentes tamanhos de entrada, inicialmente considerando:

* **50.000 registros**
* **100.000 registros**
* **250.000 registros**
* **500.000 registros**
* **1.000.000 de registros**

Serão avaliados fatores como tempo de execução, número de comparações, altura da árvore e, quando aplicável, número de rotações.
Observação: os tamanhos acima valem para os cenários 1, 3 e 4 nas duas árvores e para o cenário 2 na AVL. No cenário 2 (e no caso extremo de ordem decrescente), a BST será testada apenas com 10.000, 20.000 e 50.000 registros, conforme a seção 3.2.

---

## 3.5 Resultados Obtidos

> **Entrega 2**

Os resultados dos testes serão apresentados posteriormente por meio de:

* Tabelas;
* Gráficos;
* Resultados de execução;
* Comparação entre as estruturas;
* Comparação dos resultados observados com a complexidade assintótica (**Big-O**) teórica.

Os arquivos de resultados serão armazenados na pasta `/resultados`.

---

# 4. ⚙️ Como Executar

## 4.1 Pré-requisitos

> **Entrega 2**

Serão informados nesta seção:

* Linguagem utilizada;
* Versão do compilador;
* Dependências necessárias;
* Outros requisitos para execução.

---

## 4.2 Instruções

> **Entrega 2**

Após a implementação, serão adicionadas as instruções completas para compilação e execução do projeto.

Exemplo:

```bash
git clone <link-do-repositorio>
cd <pasta-do-projeto>
```

Os comandos específicos de compilação e execução serão adicionados posteriormente.

---

## 4.3 Estrutura do Repositório

A estrutura prevista para o projeto é:

```text
Fungos-das--rvores-C/
│
├── src/
│   └── código-fonte
│
├── dataset/
│   └── dataset utilizado
│
├── testes/
│   └── scripts e casos de teste
│
├── resultados/
│   └── saídas e relatórios de desempenho
│
└── README.md
```

---

# 5. 📚 Referências

* **GBIF — Global Biodiversity Information Facility**
  Registros de ocorrência de organismos do grupo taxonômico Fungi.

  https://www.gbif.org/occurrence/search?taxonKey=5

* **Materiais fornecidos pelo professor Cid Rodrigues de Andrade**
  Disciplina de Estruturas de Dados II — UNICID.

---

> 🍄 **Fungos das Árvores-C**
> Trabalho desenvolvido para a disciplina de **Estruturas de Dados II — UNICID**.

