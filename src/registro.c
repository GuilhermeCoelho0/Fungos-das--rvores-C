/* registro.c - comparador da chave composta e leitor de CSV (GBIF). */
#include "registro.h"

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


/* Comparador da chave composta (scientificName, year, gbifID) */

/* E ESTA FUNCAO que define o que e "menor", "maior" e "igual" para as duas
 * arvores. Compara em cascata (desempate), como ordenar nomes de uma lista:
 *   scientificName (ordem alfabetica, via strcmp)
 *   se o nome empatar, year (numerico)
 *   se o ano tambem empatar, gbifID (numerico)
 * So retorna 0 quando os TRES campos sao iguais e so ai a arvore considera a chave duplicada e rejeita a insercao.
 * Retorna sempre -1, 0 ou 1 (e nao a diferenca crua) para evitar overflow
 * ao subtrair numeros grandes como o gbifID (long long)*/

int registro_comparar(const Registro *a, const Registro *b)
{
    int c = strcmp(a->scientificName, b->scientificName);
    if (c != 0) return c < 0 ? -1 : 1;
    if (a->year != b->year) return a->year < b->year ? -1 : 1;
    if (a->gbifID != b->gbifID) return a->gbifID < b->gbifID ? -1 : 1;
    return 0;
}


/* Utilitarios do leitor*/

/* A string e vazia ou so tem espacos/tabs? (usado para detectar campo ausente) */
static int vazio(const char *s)
{
    while (*s) {
        if (!isspace((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}

/* Copia com truncamento seguro: nunca escreve alem de 'tam' bytes.
 * Retorna 1 se precisou cortar o texto (para contarmos nas estatisticas)*/
static int copiar(char *dst, size_t tam, const char *src)
{
    size_t l = strlen(src);
    if (l >= tam) {
        memcpy(dst, src, tam - 1);
        dst[tam - 1] = '\0';
        return 1;
    }
    memcpy(dst, src, l + 1);
    return 0;
}


/* Converte texto em inteiro (long long) VALIDANDO: rejeita vazio, lixo no fim
 * ("12abc") e estouro de faixa. Retorna 1 se ok. Nao usamos atoi() porque ele
 * devolve 0 para texto invalido e nao da para distinguir de um "0" de verdade */
static int parse_ll(const char *s, long long *out)
{
    char *fim;
    errno = 0;
    long long v = strtoll(s, &fim, 10);
    if (fim == s || errno != 0) return 0;
    while (isspace((unsigned char)*fim)) fim++;
    if (*fim != '\0') return 0;
    *out = v;
    return 1;
}

/* Mesma ideia para numeros decimais (latitude/longitude). Tambem rejeita
 * NaN e infinito (isfinite)*/
static int parse_double(const char *s, double *out)
{
    char *fim;
    errno = 0;
    double v = strtod(s, &fim);
    if (fim == s || errno != 0 || !isfinite(v)) return 0;
    while (isspace((unsigned char)*fim)) fim++;
    if (*fim != '\0') return 0;
    *out = v;
    return 1;
}

/* Compara duas strings ignorando maiuscula/minuscula ("fi" == "FI") */
static int igual_ci(const char *a, const char *b)
{
    while (*a && *b) {
        if (toupper((unsigned char)*a) != toupper((unsigned char)*b)) return 0;
        a++; b++;
    }
    return *a == *b;
}

/* O pais do registro esta na lista "FI,NO,DK" (sem diferenciar maiusculas)
 * Percorre a lista cortando em cada virgula e comparando cada pedaco */
static int pais_permitido(const char *lista, const char *pais)
{
    char token[64];
    const char *p = lista;
    while (*p) {
        size_t n = 0;
        while (p[n] && p[n] != ',') n++; /* n = tamanho do pedaco atual */
        if (n > 0 && n < sizeof token) {
            memcpy(token, p, n);
            token[n] = '\0';
            if (igual_ci(token, pais)) return 1;
        }
        p += n;
        if (*p == ',') p++;
    }
    return 0;
}

/* Le uma linha inteira (qualquer tamanho). Retorna tamanho ou -1 no EOF
 * (-2 = faltou memoria.) Usamos buffer que cresce porque linhas do GBIF podem
 * ser MUITO longas e um fgets com tamanho fixo cortaria a linha no meio */
static long ler_linha(FILE *f, char **buf, size_t *cap)
{
    size_t len = 0;
    for (;;) {
        if (*cap - len < 2) { /* sem espaco: dobra o buffer */
            size_t nc = *cap ? *cap * 2 : 4096;
            char *nb = realloc(*buf, nc);
            if (!nb) return -2;
            *buf = nb;
            *cap = nc;
        }
        if (!fgets(*buf + len, (int)(*cap - len), f)) break;
        len += strlen(*buf + len);
        if (len > 0 && (*buf)[len - 1] == '\n') break; /* chegou ao fim da linha */
    }
    if (len == 0) return -1;
    while (len > 0 && ((*buf)[len - 1] == '\n' || (*buf)[len - 1] == '\r'))
        (*buf)[--len] = '\0'; /* tira \n e \r (arquivos de Windows) */
    return (long)len;
}

/* Divide a linha em campos, in-place. Com aspas habilitadas, segue o padrao
 * RFC 4180 (campo entre aspas, "" = aspas literal). Retorna o numero de
 * campos, ou -1 se houver aspas sem fechar. Para ao atingir 'max' campos
 * Limitacao: campos entre aspas com quebra de linha NAO sao suportados
 *
 * "In-place" = nao copia nada: troca cada delimitador por '\0' dentro da
 * propria linha e guarda em campos[i] o ponteiro para o inicio de cada pedaco.
 * Depois disso, cada campos[i] ja e uma string C normal */
static int dividir(char *linha, char delim, int aspas, char **campos, int max)
{
    int n = 0;
    char *p = linha;
    for (;;) {
        if (n >= max) return n;
        if (aspas && *p == '"') {
            /* campo entre aspas: r le, w escreve (vai "desescapando" as aspas) */
            char *r = p + 1, *w = p;
            campos[n++] = p;
            for (;;) {
                if (*r == '"') {
                    if (r[1] == '"') { *w++ = '"'; r += 2; } /* "" vira uma aspa literal */
                    else { r++; break; } /* aspa de fechamento */
                } else if (*r == '\0') {
                    return -1; /* aspas nunca fechadas */
                } else {
                    *w++ = *r++;
                }
            }
            if (*r == delim) { *w = '\0'; p = r + 1; continue; }
            if (*r == '\0')  { *w = '\0'; return n; }
            return -1; /* lixo depois das aspas */
        }
        /* campo normal: anda ate o proximo delimitador */
        campos[n++] = p;
        while (*p && *p != delim) p++;
        if (*p == delim) { *p = '\0'; p++; continue; }
        return n;
    }
}

/* Posicao (indice) de cada coluna que nos interessa dentro do CSV
 * -1 significa "essa coluna nao existe neste arquivo" */
typedef struct {
    int id, nome, ano, kingdom, phylum, classe, ordem, family, genus,
        species, pais, lat, lon;
} Colunas;

/* Le o cabecalho e descobre em que posicao esta cada coluna, PELO NOME.
 * Assim o programa nao depende da ordem das colunas do arquivo */
static void mapear(Colunas *c, char **nomes, int n)
{
    memset(c, -1, sizeof *c); /* todos os int = -1 */
    for (int i = 0; i < n; i++) {
        const char *s = nomes[i];
        if (!strcmp(s, "gbifID")) c->id = i;
        else if (!strcmp(s, "scientificName")) c->nome = i;
        else if (!strcmp(s, "year")) c->ano = i;
        else if (!strcmp(s, "kingdom")) c->kingdom = i;
        else if (!strcmp(s, "phylum")) c->phylum = i;
        else if (!strcmp(s, "class")) c->classe = i;
        else if (!strcmp(s, "order")) c->ordem = i;
        else if (!strcmp(s, "family")) c->family = i;
        else if (!strcmp(s, "genus")) c->genus = i;
        else if (!strcmp(s, "species")) c->species = i;
        else if (!strcmp(s, "country") || !strcmp(s, "countryCode")) c->pais = i;
        else if (!strcmp(s, "latitude") || !strcmp(s, "decimalLatitude")) c->lat = i;
        else if (!strcmp(s, "longitude") || !strcmp(s, "decimalLongitude")) c->lon = i;
    }
}
/* Devolve o texto da coluna idx, ou "" se a coluna nao existe no arquivo
 * (assim as colunas opcionais nunca dao erro) */
static const char *campo(char **campos, int idx)
{
    return idx >= 0 ? campos[idx] : "";
}


/* Carga do CSV*/

/* Funcao principal do leitor. Resumo do que ela faz:
 *   1. abre o arquivo e le o cabecalho
 *   2. descobre o delimitador (TAB, ';' ou ',') e mapeia as colunas
 *   3. para cada linha: divide em campos, valida, aplica filtros e, se estiver
 *      tudo certo, guarda um Registro no vetor (que cresce dobrando)
 *   4. conta (em 'st') cada linha aceita e o MOTIVO de cada descarte
 * Uma linha ruim e apenas descartada e contada, ela nao derruba a leitura */

int registro_carregar_csv(const char *caminho, const FiltroCarga *filtro,
                          size_t max, Registro **saida, size_t *qtd,
                          EstatCarga *st, char *erro, size_t erro_tam)
{
    FILE *f = NULL;
    char *buf = NULL, *copia_cab = NULL, **campos = NULL;
    size_t cap = 0, cap_vet = 0, n = 0;
    Registro *vet = NULL;
    int rc = -1; /* comeca como "erro", so vira 0 no final se tudo der certo */

    memset(st, 0, sizeof *st);
    *saida = NULL;
    *qtd = 0;

    f = fopen(caminho, "r");
    if (!f) {
        snprintf(erro, erro_tam, "nao foi possivel abrir '%s': %s", caminho, strerror(errno));
        return -1;
    }

    long len = ler_linha(f, &buf, &cap);
    if (len < 0) {
        snprintf(erro, erro_tam, "arquivo vazio ou erro ao ler o cabecalho");
        goto fim;
    }
    char *cab = buf;
    if ((unsigned char)cab[0] == 0xEF && (unsigned char)cab[1] == 0xBB && (unsigned char)cab[2] == 0xBF)
        cab += 3; /* BOM UTF-8 (3 bytes invisiveis que alguns programas colocam no inicio do arquivo) */

    /* Delimitador: o mais frequente entre TAB, ';' e ',' no cabecalho
     * (O download "Simple" do GBIF vem separado por TAB; o Excel costuma salvar com ';'.) */
    size_t ntab = 0, nsc = 0, nvir = 0;
    for (const char *p = cab; *p; p++) {
        if (*p == '\t') ntab++;
        else if (*p == ';') nsc++;
        else if (*p == ',') nvir++;
    }
    char delim = '\t';
    size_t melhor = ntab;
    if (nsc > melhor)  { delim = ';'; melhor = nsc; }
    if (nvir > melhor) { delim = ',';  melhor = nvir; }
    if (melhor == 0) {
        snprintf(erro, erro_tam, "nao foi possivel identificar o delimitador no cabecalho");
        goto fim;
    }
    st->delimitador = delim;
    int aspas = (delim != '\t'); /* TSV do GBIF nao usa aspas: elas sao literais */

    int ncols = (int)melhor + 1; /* numero de colunas = numero de delimitadores + 1 */
    copia_cab = malloc(strlen(cab) + 1);
    campos = malloc((size_t)(ncols + 2) * sizeof *campos);
    if (!copia_cab || !campos) { snprintf(erro, erro_tam, "sem memoria"); goto fim; }
    strcpy(copia_cab, cab); /* trabalhamos numa copia, pois dividir() altera o texto */

    int nc = dividir(copia_cab, delim, aspas, campos, ncols + 1);
    if (nc != ncols) {
        snprintf(erro, erro_tam, "cabecalho malformado (%d campos lidos, %d esperados)", nc, ncols);
        goto fim;
    }
    Colunas col;
    mapear(&col, campos, ncols);
    if (col.id < 0 || col.nome < 0 || col.ano < 0) { /* as 3 colunas da chave sao obrigatorias */
        snprintf(erro, erro_tam, "colunas obrigatorias ausentes no cabecalho:%s%s%s",
                 col.id < 0 ? " gbifID" : "", col.nome < 0 ? " scientificName" : "",
                 col.ano < 0 ? " year" : "");
        goto fim;
    }
/*Laco principal: uma iteracao por linha de dados */
    for (;;) {
        len = ler_linha(f, &buf, &cap);
        if (len == -2) { snprintf(erro, erro_tam, "sem memoria ao ler linha"); goto fim; }
        if (len < 0) break; /* fim do arquivo */
        if (vazio(buf)) continue;  /* ignora linhas em branco */
        st->linhas_lidas++;

        /* limite --max: para quando ja tem registros suficientes */
        if (max > 0 && n >= max) { st->parou_no_limite = 1; st->linhas_lidas--; break; }

        int k = dividir(buf, delim, aspas, campos, ncols + 1);
        if (k != ncols) { st->desc_malformada++; continue; } /* numero de colunas errado */

        /* Validacao dos campos da chave: cada falha tem seu proprio contador */
        const char *s_nome = campo(campos, col.nome);
        const char *s_ano  = campo(campos, col.ano);
        const char *s_id   = campo(campos, col.id);
        if (vazio(s_nome)) { st->desc_sem_nome++; continue; }
        if (vazio(s_ano))  { st->desc_sem_ano++;  continue; }
        long long ano_ll, id;
        if (!parse_ll(s_ano, &ano_ll) || ano_ll < 1 || ano_ll > 9999) { st->desc_ano_invalido++; continue; }
        if (!parse_ll(s_id, &id) || id <= 0) { st->desc_id_invalido++; continue; }
        /* Filtros EXPLICITOS (so agem se o usuario passou --paises / --anos) */
        const char *s_pais = campo(campos, col.pais);
        if (filtro && filtro->paises[0] && !pais_permitido(filtro->paises, s_pais)) {
            st->desc_filtro_pais++; continue;
        }
        if (filtro && ((filtro->ano_min && ano_ll < filtro->ano_min) ||
                       (filtro->ano_max && ano_ll > filtro->ano_max))) {
            st->desc_filtro_ano++; continue;
        }

        /* Linha aprovada: garante espaco no vetor (dobra a capacidade quando enche) */
        if (n == cap_vet) {
            size_t nova = cap_vet ? cap_vet * 2 : 1024;
            Registro *nv = realloc(vet, nova * sizeof *vet);
            if (!nv) { snprintf(erro, erro_tam, "sem memoria (%lu registros ja lidos)", (unsigned long)n); goto fim; }
            vet = nv;
            cap_vet = nova;
        }
        Registro *r = &vet[n];
        memset(r, 0, sizeof *r); /* zera tudo: campos opcionais ausentes ficam "" / 0 */
        r->gbifID = id;
        r->year = (int)ano_ll;
        st->campos_truncados += (size_t)copiar(r->scientificName, sizeof r->scientificName, s_nome);
        st->campos_truncados += (size_t)copiar(r->kingdom, sizeof r->kingdom, campo(campos, col.kingdom));
        st->campos_truncados += (size_t)copiar(r->phylum, sizeof r->phylum, campo(campos, col.phylum));
        st->campos_truncados += (size_t)copiar(r->classe, sizeof r->classe, campo(campos, col.classe));
        st->campos_truncados += (size_t)copiar(r->ordem, sizeof r->ordem, campo(campos, col.ordem));
        st->campos_truncados += (size_t)copiar(r->family, sizeof r->family, campo(campos, col.family));
        st->campos_truncados += (size_t)copiar(r->genus, sizeof r->genus, campo(campos, col.genus));
        st->campos_truncados += (size_t)copiar(r->species, sizeof r->species, campo(campos, col.species));
        st->campos_truncados += (size_t)copiar(r->country, sizeof r->country, s_pais);

        /* Coordenadas sao opcionais: se faltarem ou forem absurdas (fora do
         * intervalo valido), o registro e aceito mesmo assim, so marcado como
         * "sem coordenadas" nao entram na chave, entao nao justificam descartar */
        double la, lo;
        if (parse_double(campo(campos, col.lat), &la) && parse_double(campo(campos, col.lon), &lo) &&
            la >= -90.0 && la <= 90.0 && lo >= -180.0 && lo <= 180.0) {
            r->latitude = la;
            r->longitude = lo;
            r->tem_coordenadas = 1;
        } else {
            st->coord_ausentes++;
        }
        n++;
        st->aceitas++;
    }
    if (ferror(f)) { snprintf(erro, erro_tam, "erro de leitura em '%s'", caminho); goto fim; }

    /* Sucesso: entrega o vetor ao chamador (que passa a ser o dono e deve dar free) */
    *saida = vet;
    *qtd = n;
    vet = NULL; /* evita o free(vet) logo abaixo apagar o que acabamos de entregar */
    rc = 0;
fim: /* ponto unico de limpeza: roda tanto no sucesso quanto em qualquer erro */
    free(vet);
    free(buf);
    free(copia_cab);
    free(campos);
    if (f) fclose(f);
    return rc;
}

/* Imprime o "relatorio da carga": quantas linhas entraram e por que as outras
 * foram descartadas. Util para colocar no README/relatorio do trabalho */
void registro_imprimir_estatisticas(const EstatCarga *st, FILE *out)
{
    fprintf(out, "Leitura do CSV (delimitador: %s)\n",
            st->delimitador == '\t' ? "TAB" : (st->delimitador == ',' ? "virgula" : "ponto e virgula"));
    fprintf(out, "  linhas de dados lidas ........ %lu\n", (unsigned long)st->linhas_lidas);
    fprintf(out, "  registros aceitos ............ %lu\n", (unsigned long)st->aceitas);
    fprintf(out, "  descartados: malformados ..... %lu\n", (unsigned long)st->desc_malformada);
    fprintf(out, "  descartados: sem scientificName %lu\n", (unsigned long)st->desc_sem_nome);
    fprintf(out, "  descartados: sem year ........ %lu\n", (unsigned long)st->desc_sem_ano);
    fprintf(out, "  descartados: year invalido ... %lu\n", (unsigned long)st->desc_ano_invalido);
    fprintf(out, "  descartados: gbifID invalido . %lu\n", (unsigned long)st->desc_id_invalido);
    fprintf(out, "  descartados: filtro de pais .. %lu\n", (unsigned long)st->desc_filtro_pais);
    fprintf(out, "  descartados: filtro de ano ... %lu\n", (unsigned long)st->desc_filtro_ano);
    fprintf(out, "  aceitos sem coordenadas ...... %lu\n", (unsigned long)st->coord_ausentes);
    fprintf(out, "  campos de texto truncados .... %lu\n", (unsigned long)st->campos_truncados);
    if (st->parou_no_limite) fprintf(out, "  (leitura interrompida pelo limite --max)\n");
}
