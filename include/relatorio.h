#ifndef RELATORIO_H
#define RELATORIO_H

#include "arvore_b_mais.h"
#include <stdio.h>

void imprimir_relatorio(FILE *arquivo_csv, tnoe_b_mais *raiz_bm);
void imprimir_relatorio_paginado(FILE *arquivo_csv, tnoe_b_mais *raiz_bm, int numero_da_pagina);

#endif