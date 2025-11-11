#ifndef GERENCIADOR_CSV_H
#define GERENCIADOR_CSV_H

#include "aerodromo.h"

#define ARQUIVO_CSV "../data/G12.csv"
#define SEPARADOR_CSV ";\n"
#define NUMERO_COLUNAS_CSV 10

#define COLUNA_LONGITUDE 4
#define COLUNA_LATITUDE 5

typedef struct _registro{
    Coordenadas coordenadas;
    int linha_tabela;
} Registro;

int processar_csv(const char *nome_arquivo, Registro **registros, int *num_registros);

#endif