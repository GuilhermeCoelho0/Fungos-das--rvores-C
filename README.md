# Fungos-das--rvores-C
# 🍄 Fungos das Árvores-C

**Entrega 1 — Trabalho da disciplina Estruturas de Dados II**
**Universidade Cidade de São Paulo (UNICID)**
**Professor:** Cid Rodrigues de Andrade

---

## 👥 Integrantes do Grupo

| Nome completo                   |         RA |
| ------------------------------- | ---------: |
| **Guilherme Coelho**            |   42966515 |
| **Maria Luisa Assunção Novaes** |   42802881 |
| **Miguel Targino**              |   43079067 |
| **Thiago Salles**               |   43461905 |

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
| `kingdom`        | String  | Reino taxonômico                              |
| `phylum`         | String  | Filo                                          |
| `class`          | String  | Classe                                        |
| `order`          | String  | Ordem                                         |
| `family`         | String  | Família                                       |
| `genus`          | String  | Gênero                                        |
| `species`        | String  | Espécie, quando disponível                    |
| `country`        | String  | País onde a ocorrência foi registrada         |
| `latitude`       | Float   | Latitude da ocorrência                        |
| `longitude`      | Float   | Longitude da ocorrência                       |
| `year`           | Inteiro | Ano associado ao registro                     |

O campo **`gbifID`** será utilizado como **chave principal** para as operações de inserção, busca e remoção.

Também serão realizados testes utilizando atributos textuais, como **`scientificName`**, para avaliar comparações entre strings.

---

## 1.4 Justificativa da Escolha

O dataset de fungos foi escolhido por atender aos principais requisitos do projeto de **Estruturas de Dados II**.

O GBIF disponibiliza uma grande quantidade de registros de ocorrência, permitindo trabalhar com datasets entre **50 mil e 1 milhão de registros**, conforme solicitado no projeto.

Além do grande volume, os registros possuem diversos atributos e diferentes tipos de dados, como **inteiros, strings e valores de ponto flutuante**. Isso permite representar um cenário mais próximo de uma aplicação real e exige a implementação de comparadores para os registros.

A grande quantidade de registros também permite avaliar o comportamento das estruturas de árvore em diferentes tamanhos de entrada e cenários de teste, possibilitando medir:

* ⏱️ Tempo de inserção;
* 🔎 Tempo de busca;
* 🗑️ Tempo de remoção;
* 🌳 Altura da árvore;
* 🔢 Número de comparações;
* 🔄 Número de rotações, quando aplicável.

Dessa forma, o dataset permite comparar o comportamento das estruturas de árvore escolhidas pelo grupo em diferentes condições, incluindo **dados aleatórios, ordenados, duplicados e operações de inserção, busca e remoção**.

---

# 2. 🌳 Estrutura(s) de Árvore Escolhida(s)

## 2.1 Estrutura(s)

2.1 Estrutura(s)

Serão utilizadas duas estruturas de árvore para organização dos registros do dataset:

Árvore Binária de Busca (BST — Binary Search Tree);
Árvore AVL (Adelson-Velsky e Landis).


---

## 2.2 Justificativa Técnica

2.2 Justificativa Técnica

A escolha da BST e da AVL está relacionada principalmente ao volume do dataset e à necessidade de analisar o comportamento das árvores em diferentes condições de entrada.

A BST possui uma estrutura relativamente simples e organiza os registros de acordo com a chave gbifID. Quando a árvore apresenta uma distribuição equilibrada, as operações de busca, inserção e remoção podem apresentar complexidade média de O(log n). Entretanto, uma BST não balanceada pode atingir uma altura próxima de n, fazendo com que essas operações apresentem complexidade de O(n) no pior caso.

Esse comportamento é especialmente relevante para o projeto porque permite testar situações com registros organizados em ordem crescente ou decrescente, possibilitando observar o impacto da distribuição dos dados sobre a estrutura.

A AVL, por outro lado, mantém a árvore balanceada por meio de rotações após determinadas operações. Seu objetivo é manter a altura da árvore em ordem logarítmica em relação ao número de elementos, evitando que a estrutura se transforme em uma árvore linear.

A escolha conjunta das duas estruturas permite analisar a diferença entre:

uma árvore de busca que não possui balanceamento automático, representada pela BST;
uma árvore de busca auto-balanceada, representada pela AVL.

Essa comparação é adequada ao dataset porque o conjunto possui potencial para conter centenas de milhares de registros. Dessa forma, alterações na altura da árvore podem ter impacto significativo no número de comparações necessárias para localizar ou modificar um registro.

Além disso, a comparação entre BST e AVL possibilita relacionar os resultados dos testes práticos com suas respectivas complexidades assintóticas e observar como o balanceamento influencia o comportamento das operações.


---

## 2.3 Operações Implementadas

As operações previstas para a implementação são:

* [ ] Inserção
* [ ] Remoção
* [ ] Busca
* [ ] Percurso em pré-ordem
* [ ] Percurso em ordem
* [ ] Percurso em pós-ordem
* [ ] Balanceamento, se aplicável
* [ ] Outra: a definir

> **Observação:** operações referentes à **Entrega 2** serão preenchidas conforme a implementação do projeto.

---

## 2.4 Complexidade

2.4 Complexidade

A complexidade teórica das operações de inserção, busca e remoção depende da estrutura utilizada e, no caso da BST, principalmente da altura da árvore.

BST
Operação	Melhor caso	Caso médio	Pior caso
Inserção	O(1)	O(log n)	O(n)
Busca	O(1)	O(log n)	O(n)
Remoção	O(1)	O(log n)	O(n)

Na BST, o melhor caso ocorre quando a operação encontra diretamente a posição necessária ou quando a árvore possui uma configuração favorável. No caso médio, considerando uma distribuição aproximadamente equilibrada, as operações apresentam comportamento O(log n). No pior caso, a árvore pode ficar degenerada, com altura próxima de n, levando as operações para O(n).

AVL
Operação	Melhor caso	Caso médio	Pior caso
Inserção	O(1)	O(log n)	O(log n)
Busca	O(1)	O(log n)	O(log n)
Remoção	O(1)	O(log n)	O(log n)

Na AVL, o balanceamento mantém a altura da árvore em ordem O(log n). Dessa forma, as operações de busca, inserção e remoção possuem limite assintótico O(log n) no pior caso.

Complexidade	Ideia simples
O(1)	Não depende do tamanho dos dados
O(log n)	Cresce muito lentamente
O(n)	Cresce proporcionalmente aos dados


---

# 3. 🧪 Plano de Testes

## 3.1 Objetivo dos Testes

3.1 Objetivo dos Testes

Os testes terão como objetivo verificar a corretude das estruturas de árvore e analisar seu comportamento diante de diferentes formas de entrada.

Serão considerados os seguintes objetivos:

Verificar se os registros são inseridos corretamente;
Verificar se registros podem ser localizados pela chave gbifID;
Verificar o comportamento das operações de remoção;
Avaliar o comportamento das árvores com diferentes distribuições de dados;
Verificar o comportamento com dados em ordem crescente e decrescente;
Verificar o tratamento de registros duplicados;
Avaliar casos extremos, como árvores vazias e árvores contendo apenas um elemento;
Observar a altura das estruturas;
Contabilizar o número de comparações realizadas;
Verificar, na AVL, o comportamento relacionado ao balanceamento e às rotações.

Os testes também permitirão posteriormente relacionar o comportamento observado nas execuções com as complexidades assintóticas teóricas das estruturas BST e AVL.


---

## 3.2 Cenários de Teste

3.2 Cenários de Teste

Além dos diferentes tipos e volumes de dados, serão considerados três cenários relacionados às condições de utilização do computador durante a execução dos testes. O objetivo é observar se a carga de trabalho do sistema influencia o tempo de execução das operações realizadas pelas estruturas de árvore.

|  #  | Cenário                            | Entrada / Condição do sistema                                                                                                                                                                                                 | Resultado esperado                                                                                                      | Status |
| :-: | :--------------------------------- | :---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | :---------------------------------------------------------------------------------------------------------------------- | :----: |
|  1  | **Baixa utilização do sistema**    | Execução do teste logo após a inicialização do computador, com o mínimo possível de programas e processos adicionais em execução.                                                                                             | Obter uma referência de desempenho com baixa utilização dos recursos do computador.                                     |    ☐   |
|  2  | **Utilização moderada do sistema** | Execução do teste após o computador ter sido utilizado normalmente por algum período, com programas e processos comuns em execução.                                                                                           | Verificar a variação do desempenho das operações em uma condição de utilização moderada dos recursos.                   |    ☐   |
|  3  | **Alta utilização do sistema**     | Execução do teste enquanto o computador estiver executando diversas tarefas simultaneamente, como várias abas do navegador, programas em segundo plano ou processos de maior consumo de recursos, como renderização de vídeo. | Verificar o comportamento e a variação do tempo de execução das operações sob maior utilização dos recursos do sistema. |    ☐   |




---

## 3.3 Casos Extremos (Edge Cases)

3.3 Casos Extremos (Edge Cases)

Serão considerados os seguintes casos extremos:

🌱 Árvore vazia: verificar o comportamento das operações quando nenhum registro foi inserido;
1️⃣ Árvore com um único elemento: verificar inserção, busca e remoção de um único registro;
🔁 Dados duplicados: verificar o comportamento quando existem registros com a mesma chave gbifID;
⬆️ Dados em ordem crescente: inserir registros com gbifID em ordem crescente para observar o comportamento da BST e o balanceamento da AVL;
⬇️ Dados em ordem decrescente: inserir registros com gbifID em ordem decrescente para observar o comportamento da BST e o balanceamento da AVL;
📦 Grande volume de dados: utilizar o maior conjunto de registros previsto para verificar o comportamento das estruturas em grande escala;
🔎 Busca de elemento existente: verificar se um registro presente é encontrado corretamente;
❌ Busca de elemento inexistente: verificar se a estrutura identifica corretamente a ausência do registro;
🗑️ Remoção de elemento existente: verificar a remoção de registros presentes na árvore;
❌ Remoção de elemento inexistente: verificar se a estrutura permanece consistente quando se tenta remover uma chave inexistente.

Os dados em ordem crescente e decrescente são especialmente relevantes para a BST, pois podem produzir uma árvore altamente desbalanceada e aproximar seu comportamento do pior caso O(n).

Na AVL, esses mesmos cenários permitem verificar a capacidade de manutenção do balanceamento por meio das rotações.



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
Fungos-das-Arvores-C/
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

