#ifndef RELATORIO_H
#define RELATORIO_H

#include "arvore_b_mais.h"
#include <stdio.h>

#define REGISTROS_POR_PAGINA 2 * GRAU_MIN

void imprimir_relatorio(FILE *arquivo_csv, tnoe_b_mais *raiz_bm);
void imprimir_relatorio_paginado(FILE *arquivo_csv, tnoe_b_mais *raiz_bm, int numero_da_pagina);

#endif