#ifndef GERENCIADOR_CSV_H
#define GERENCIADOR_CSV_H

#include "aerodromo.h"

#define ARQUIVO_CSV "data/G12.csv"
#define SEPARADOR_CSV ";\n"
#define NUMERO_COLUNAS_CSV 10

typedef enum {
    COLUNA_CODIGO_OACI = 0,
    COLUNA_NOME = 1,
    COLUNA_MUNICIPIO = 2,
    COLUNA_UF = 3,
    COLUNA_LONGITUDE = 4,
    COLUNA_LATITUDE = 5,
    COLUNA_ALTITUDE = 6,
    COLUNA_OPERACAO_D = 7,
    COLUNA_COMPRIMENTO = 8,
    COLUNA_LARGURA = 9
} ColunasCSV;

typedef Coordenadas tchave;

typedef struct _registro{
    tchave chave;
    int linha_tabela;
} Registro;

int processar_csv(FILE *arquivo, Registro **registros, int *num_registros);
int comparar_registros(const Registro *r1, const Registro *r2);
int comparar_chaves(const tchave *c1, const tchave *c2);
void liberar_registros(Registro **registros, int *num_registros);
int ler_linha_csv(FILE *arquivo, Aerodromo *aerodromo, int linha_tabela);

#endif