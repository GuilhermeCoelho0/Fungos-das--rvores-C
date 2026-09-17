# Fungos-das--rvores-C
Fungos das Árvores-C
Entrega 1 de trabalho da disciplina Estruturas de Dados II — UNICID Prof. Cid Rodrigues de Andrade
👥 Integrantes do Grupo
Nome completo	RA
Guilherme Coelho 	42966515
Maria Luisa Assunção Novaes	42802881
Thiago Salles	
Miguel Targino	
	
________________________________________
1. Dataset
1.1 Descrição
O conjunto de dados utilizado nos testes será composto por registros de ocorrência de fungos obtidos do Global Biodiversity Information Facility (GBIF), uma infraestrutura internacional que disponibiliza dados sobre biodiversidade.

O domínio do dataset é biodiversidade de espécies de fungos. Cada registro representa uma ocorrência de um organismo, que contém informações sobre identificação taxonômica, localização geográfica e data da ocorrência.

Os dados serão obtidos em formato estruturado, como CSV, e será utilizado um subconjunto dos registros disponíveis para adequação aos requisitos do projeto, com aproximadamente 500.000 registros inicialmente, podendo chegar a 1.000.000 de registros nos testes de maior escala.

1.2 Fonte
O dataset será obtido por meio do GBIF (Global Biodiversity Information Facility), utilizando os registros de ocorrência classificados no grupo taxonômico Fungi.
Fonte: https://www.gbif.org/occurrence/search?taxonKey=5
1.3 Estrutura dos dados
Serão utilizados os principais atributos disponíveis nos registros para representar cada ocorrência, como por exemplo:

gbifID (inteiro) — identificador único do registro de ocorrência;
scientificName (string) — nome científico do organismo;
kingdom (string) — reino taxonômico;
phylum (string) — filo;
class (string) — classe;
order (string) — ordem;
family (string) — família;
genus (string) — gênero;
species (string) — espécie, quando disponível;
country (string) — país onde a ocorrência foi registrada;
latitude (float) — latitude da ocorrência;
longitude (float) — longitude da ocorrência;
year (inteiro) — ano associado ao registro.

O campo gbifID será utilizado como chave principal para operações de inserção, busca e remoção. Também serão realizados testes utilizando atributos textuais, como scientificName, para avaliar comparações entre strings.
1.4 Justificativa da escolha
Por que este dataset é adequado para testar a(s) estrutura(s) de árvore escolhida(s) (volume, distribuição dos dados, tipo de chave, etc.)?
________________________________________
2. Estrutura(s) de Árvore Escolhida(s)
2.1 Estrutura(s)
Liste a(s) árvore(s) implementada(s) (ex: BST, AVL, Rubro-Negra, B-Tree, etc.).
2.2 Justificativa técnica
Por que essa(s) estrutura(s) foi(ram) escolhida(s) para este dataset e problema? Considere complexidade, balanceamento, caso de uso.
2.3 Operações implementadas (Para Entrega 2)
	Inserção
	Remoção
	Busca
	Percursos (pré-ordem, em ordem, pós-ordem)
	Balanceamento (se aplicável)
	Outra: ______
2.4 Complexidade
Tabela com a complexidade assintótica (Big-O) teórica de cada operação implementada, no melhor, médio e pior caso.
Operação	Melhor caso	Caso médio	Pior caso
Inserção			
Busca			
Remoção			
________________________________________
3. Plano de Testes
3.1 Objetivo dos testes
O que o grupo pretende validar (corretude, desempenho, comportamento em casos extremos, etc.).
3.2 Cenários de teste
#	Cenário	Entrada	Resultado esperado	Status
1				☐
2				☐
3				☐
3.3 Casos extremos (edge cases)
Liste casos como: árvore vazia, único elemento, dados duplicados, dados em ordem crescente/decrescente (pior caso para BST), volume máximo do dataset, etc.
3.4 Testes de desempenho (Para Entrega 2)
Descreva como o grupo mediu tempo de execução e/ou uso de memória, e com quais tamanhos de entrada (ex: 100, 1.000, 10.000 registros).
3.5 Resultados obtidos (Para Entrega 2)
Resuma os resultados (tabelas, gráficos ou links para arquivos de saída na pasta /resultados) e compare-os com a complexidade assintótica (Big-O) teórica.
________________________________________
4. Como Executar
4.1 Pré-requisitos (Para Entrega 2)
Linguagem, versão e dependências necessárias.
4.2 Instruções (Para Entrega 2)
# Exemplo
git clone <link-do-repositorio>
cd <pasta>
# comandos de compilação/execução
4.3 Estrutura do repositório (já com pastas para a Entrega 2)
/src         → código-fonte
/dataset     → dataset utilizado
/testes      → scripts e casos de teste
/resultados  → saídas e relatórios de desempenho
README.md
________________________________________
5. Referências
Bibliografia, artigos ou materiais consultados.
