#include <stdio.h>
#include <stdlib.h>
#include "relatorio.h"
#include "arvore_b_mais.h"
#include "aerodromo.h"
#include "gerenciador_csv.h"

void imprimir_relatorio(FILE *arquivo_csv, tnoe_b_mais *raiz_bm){
    tno_b_mais *registros, *aux;
    Aerodromo aerodromo;
    tnoe_b_mais *atual;
    int i, quantidade_chaves;

    if (arquivo_csv == NULL)
        printf("Arquivo CSV invalido.\n");

    atual = raiz_bm;

    quantidade_chaves = contar_chaves_bm(raiz_bm);
    registros = (Registro*) malloc(quantidade_chaves * sizeof(Registro));
    if (registros == NULL){
        printf("Erro ao alocar memoria para os registros.\n");
        return;
    }

    aux = registros;

    percorrer_em_ordem_bm(raiz_bm, &aux);

    for (i = 0; i < quantidade_chaves; i++){
        inicializar_aerodromo(&aerodromo);

        if (ler_linha_csv(arquivo_csv, &aerodromo, registros[i].linha_tabela) == 0){
            printf("Erro ao ler a linha %d do arquivo CSV.\n", registros[i].linha_tabela);
            continue;
        }

        printf("-------------------------------\n");
        printf("Aerodromo %d:\n", i + 1);
        imprimir_aerodromo(&aerodromo);
        printf("-------------------------------\n");
    }
}

void imprimir_relatorio_paginado(FILE *arquivo_csv, tnoe_b_mais *raiz_bm, int numero_da_pagina){

}