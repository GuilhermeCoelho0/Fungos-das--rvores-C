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

As estruturas de árvore implementadas pelo grupo serão definidas nesta seção.

> **Status:** 🚧 A definir / implementação para a próxima etapa.

---

## 2.2 Justificativa Técnica

A justificativa técnica das estruturas escolhidas será apresentada considerando:

* Complexidade assintótica;
* Balanceamento;
* Volume de dados;
* Características do dataset;
* Casos de uso;
* Comportamento nos diferentes cenários de teste.

> **Status:** 🚧 A definir.

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

A tabela abaixo será preenchida de acordo com as estruturas efetivamente implementadas.

| Operação | Melhor caso | Caso médio | Pior caso |
| -------- | ----------: | ---------: | --------: |
| Inserção |           — |          — |         — |
| Busca    |           — |          — |         — |
| Remoção  |           — |          — |         — |

> **Status:** 🚧 A preencher após a definição das estruturas.

---

# 3. 🧪 Plano de Testes

## 3.1 Objetivo dos Testes

Os testes terão como objetivo validar:

* A **corretude** das operações das árvores;
* O comportamento das estruturas com diferentes volumes de dados;
* O desempenho das operações;
* O comportamento em casos extremos;
* A quantidade de comparações realizadas;
* A altura das árvores;
* As rotações, quando aplicável;
* O comportamento das estruturas em diferentes cenários de entrada.

---

## 3.2 Cenários de Teste

|  # | Cenário              | Entrada                                   | Resultado esperado                             | Status |
| -: | -------------------- | ----------------------------------------- | ---------------------------------------------- | :----: |
|  1 | Dados aleatórios     | Registros de fungos em ordem aleatória    | Estrutura construída corretamente              |    ☐   |
|  2 | Dados ordenados      | Registros organizados por chave           | Avaliar comportamento da árvore                |    ☐   |
|  3 | Dados duplicados     | Registros com chaves repetidas            | Validar comportamento definido para duplicatas |    ☐   |
|  4 | Operações misturadas | Inserções, buscas e remoções intercaladas | Operações executadas corretamente              |    ☐   |

---

## 3.3 Casos Extremos (Edge Cases)

Serão considerados casos como:

* 🌱 Árvore vazia;
* 1️⃣ Árvore contendo apenas um elemento;
* 🔁 Dados duplicados;
* ⬆️ Dados em ordem crescente;
* ⬇️ Dados em ordem decrescente;
* 📦 Volume máximo do dataset;
* 🔎 Busca por elemento existente;
* ❌ Busca por elemento inexistente;
* 🗑️ Remoção de elemento existente;
* ❌ Tentativa de remoção de elemento inexistente.

Os dados em ordem crescente ou decrescente serão especialmente importantes para avaliar o **pior caso de uma BST não balanceada**.

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

