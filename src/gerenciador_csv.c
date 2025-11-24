#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gerenciador_csv.h"
#include "auxiliar.h"

#define TAM_MAX_LINHA 1024

void liberar_registros(Registro **registros, int *num_registros) {
    if (registros != NULL && *registros != NULL) {
        free(*registros);
        *registros = NULL;
    }
    
    if (num_registros != NULL)
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

int processar_csv(FILE *arquivo, Registro **registros, int *num_registros){
    int numero_linhas, numero_colunas, linha_atual, coluna_atual, i;
    char linha[TAM_MAX_LINHA];
    char *p_linha, *campo, *longitude_dms, *latitude_dms;

    if (arquivo == NULL)
        return -1;

    numero_linhas = contar_numero_de_linhas(arquivo);

    *registros = (Registro *) malloc((numero_linhas - 1) * sizeof(Registro));
    if (*registros == NULL){
        liberar_registros(registros, num_registros);
        return -1;
    }

    rewind(arquivo);

    numero_colunas = contar_numero_de_colunas(arquivo);
    if (numero_colunas != NUMERO_COLUNAS_CSV){
        liberar_registros(registros, num_registros);
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

        (*registros)[i].chave = criar_coordenadas(longitude_dms, latitude_dms);
        (*registros)[i].linha_tabela = linha_atual;
        i++;
    }
    
    *num_registros = i;
    return 0;
}

int comparar_registros(const Registro *r1, const Registro *r2){
    if (r1 == NULL || r2 == NULL)
        return 0;
    
    return comparar_chaves(&r1->chave, &r2->chave);
}

int comparar_chaves(const tchave *c1, const tchave *c2){
    return comparar_coordenadas(c1, c2);
}

int ler_linha_csv(FILE *arquivo, Aerodromo *aerodromo, int linha_tabela){
    int linha_atual, coluna_atual;
    char linha[TAM_MAX_LINHA];
    char *p_linha, *campo, *longitude_dms, *latitude_dms;

    if (arquivo == NULL)
        return 0;
    
    rewind(arquivo);

    linha_atual = 0;
    while (fgets(linha, TAM_MAX_LINHA, arquivo) != NULL){
        if (linha_atual < linha_tabela){
            linha_atual++;
            continue;
        }
        p_linha = linha;

        for (coluna_atual = 0; coluna_atual < NUMERO_COLUNAS_CSV; coluna_atual++){
            campo = separar_string(&p_linha, SEPARADOR_CSV);

            switch (coluna_atual){
            case COLUNA_CODIGO_OACI:
                strncpy(aerodromo->codigo_oaci, campo, sizeof(aerodromo->codigo_oaci));
                break;
            case COLUNA_NOME:
                if (!campo_valido(campo))
                    return 0;
                strncpy(aerodromo->nome, campo, sizeof(aerodromo->nome));
                break;
            case COLUNA_MUNICIPIO:
                if (!campo_valido(campo))
                    return 0;
                strncpy(aerodromo->municipio, campo, sizeof(aerodromo->municipio));
                break;
            case COLUNA_UF:
                if (!campo_valido(campo))
                    return 0;
                strncpy(aerodromo->uf, campo, sizeof(aerodromo->uf));
                break;
            case COLUNA_LONGITUDE:
                if (!campo_valido(campo))
                    return 0;
                longitude_dms = campo;
                break;
            case COLUNA_LATITUDE:
                if (!campo_valido(campo))
                    return 0;
                latitude_dms = campo;
                break;
            case COLUNA_ALTITUDE:
                if (!campo_valido(campo))
                    return 0;
                aerodromo->altitude_em_metros = atoi(campo);
                break;
            case COLUNA_OPERACAO_D:
                aerodromo->operacao_diurna = criar_regra_de_voo(campo);
                break;
            case COLUNA_COMPRIMENTO:
                if (!campo_valido(campo))
                    return 0;
                aerodromo->comprimento_em_metros = atoi(campo);
                break;
            case COLUNA_LARGURA:
                if (!campo_valido(campo))
                    return 0;
                aerodromo->largura_em_metros = atoi(campo);
                break;
            default:
                break;
            }
        }
        aerodromo->coordenadas = criar_coordenadas(longitude_dms, latitude_dms);
        break;
    }
    return 1;
}