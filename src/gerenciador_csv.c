#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gerenciador_csv.h"
#include "auxiliar.h"

#define TAM_MAX_LINHA 1024

void limpar_recursos(FILE *arquivo, Registro **registros, int *num_registros){
    if (arquivo != NULL)
        fclose(arquivo);

    if (*registros != NULL){
        free(*registros);
        *registros = NULL;
    }
    
    *num_registros = 0;
}

int contar_numero_de_linhas(FILE *arquivo){
    int contador;
    char linha[TAM_MAX_LINHA];

    rewind(arquivo);

    contador = 0;
    while (fgets(linha, TAM_MAX_LINHA, arquivo) != NULL)
        contador++;

    return contador;
}

int contar_numero_de_colunas(FILE *arquivo){
    int numero_colunas;
    char linha[TAM_MAX_LINHA];
    char *p_linha;

    numero_colunas = 0;

    if(fgets(linha, TAM_MAX_LINHA, arquivo) == NULL)
        return -1;

    p_linha = linha;
    
    while (separar_string(&p_linha, SEPARADOR_CSV) != NULL)
        numero_colunas++;

    return numero_colunas - 1;
}

int campo_valido(const char *campo) {
    return (campo != NULL && strlen(campo) > 0);
}

int processar_csv(const char *nome_arquivo, Registro **registros, int *num_registros){
    int numero_linhas, numero_colunas, linha_atual, coluna_atual, i;
    char linha[TAM_MAX_LINHA];
    char *p_linha, *campo, *longitude_dms, *latitude_dms;
    FILE *arquivo;

    arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL){
        limpar_recursos(arquivo, registros, num_registros);
        return -1;
    }

    numero_linhas = contar_numero_de_linhas(arquivo);

    *registros = (Registro *) malloc((numero_linhas - 1) * sizeof(Registro));
    if (*registros == NULL){
        limpar_recursos(arquivo, registros, num_registros);
        return -1;
    }

    rewind(arquivo);

    numero_colunas = contar_numero_de_colunas(arquivo);
    if (numero_colunas != NUMERO_COLUNAS_CSV){
        limpar_recursos(arquivo, registros, num_registros);
        return -1;
    }

    i = 0;
    linha_atual = 0;
    while (fgets(linha, TAM_MAX_LINHA, arquivo) != NULL){
        linha_atual++;
        p_linha = linha;
        longitude_dms = NULL;
        latitude_dms = NULL;

        for (coluna_atual = 0; coluna_atual < NUMERO_COLUNAS_CSV; coluna_atual++){
            campo = separar_string(&p_linha, SEPARADOR_CSV);

            if (coluna_atual == COLUNA_LONGITUDE)
                longitude_dms = campo;

            if (coluna_atual == COLUNA_LATITUDE)
                latitude_dms = campo;
        }

        if (!campo_valido(longitude_dms) || !campo_valido(latitude_dms))
            continue;

        (*registros)[i].coordenadas = criar_coordenadas(longitude_dms, latitude_dms);
        (*registros)[i].linha_tabela = linha_atual;
        i++;
    }
    
    *num_registros = i;
    fclose(arquivo);
    return 0;
}
    
