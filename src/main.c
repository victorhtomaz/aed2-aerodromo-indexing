#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aerodromo.h"
#include "gerenciador_csv.h"
#include "arvore_bb.h"
#include "arvore_b_mais.h"
#include "relatorio.h"

void imprimir_hierarquia_aux(tnoe_b_mais *no, int nivel) {
    int i;
    if (no != NULL) {
        for (i = 0; i < nivel; i++) {
            printf("    ");
        }

        printf("[N%d|%s|Qtd:%d]: ", nivel, no->e_folha ? "FOLHA" : "INTERNO", no->num_chaves);

        for (i = 0; i < no->num_chaves; i++) {
            printf("(%.6f) ", no->chaves[i].latitude); 
        }
        printf("\n");

        if (!no->e_folha) {
            for (i = 0; i <= no->num_chaves; i++) {
                imprimir_hierarquia_aux(no->filhos[i], nivel + 1);
            }
        }
    }
}

void imprimir_arvore(tnoe_b_mais *raiz) {
    printf("\n--- VISUALIZACAO HIERARQUICA ---\n");
    if (raiz == NULL) {
        printf("Arvore Vazia\n");
    } else {
        imprimir_hierarquia_aux(raiz, 0);
    }
    printf("--------------------------------\n");
}

int main(void);

int main(void){
    FILE *arquivo_csv;
    Aerodromo aerodromo;
    Registro *registros = NULL;
    tnoe *raiz;
    tnoe_b_mais *raiz_bm;
    Coordenadas chave_teste;
    int num_registros = 0, i;
    char longitude_dms[30], latitude_dms[30];

    criar_raiz(&raiz);
    criar_raiz_bm(&raiz_bm);

    arquivo_csv = fopen(ARQUIVO_CSV, "r");

    if(processar_csv(arquivo_csv, &registros, &num_registros) != 0){
        printf("Erro ao processar o arquivo CSV.\n");
    }
    
    for (i = 0; i < num_registros; i++){
        inserir(&raiz, registros[i]);
        inserir_bm(&raiz_bm, registros[i]);
        coordenada_decimal_para_dms(registros[i].chave.longitude, LONGITUDE, longitude_dms);
        coordenada_decimal_para_dms(registros[i].chave.latitude, LATITUDE, latitude_dms);

        /*printf("Registro %d: Linha tabela: %d, Longitude: %s, Longitude_d: %f, Latitude: %s, Latitude_d: %f\n", 
            i + 1, 
            registros[i].linha_tabela, 
            longitude_dms,
            registros[i].chave.longitude, 
            latitude_dms,
            registros[i].chave.latitude);
        */
    }

    /*chave_teste = raiz_bm->filhos[1]->filhos[1]->chaves[1];

    tno *resultado = buscar(raiz, chave_teste);
    if (resultado != NULL) {
        printf("Registro encontrado na arvore BB: Linha tabela: %d, Longitude: %.6f, Latitude: %.6f\n", 
            resultado->linha_tabela,
            resultado->chave.longitude,
            resultado->chave.latitude);
    } else {
        printf("Registro nao encontrado na arvore BB.\n");
    }   
    tno_b_mais *resultado_busca = buscar_bm(raiz_bm, chave_teste);
    if (resultado_busca != NULL) {
        printf("Registro encontrado na arvore B+: Linha tabela: %d, Longitude: %.6f, Latitude: %.6f\n", 
            resultado_busca->linha_tabela,
            resultado_busca->chave.longitude,
            resultado_busca->chave.latitude);
    } else {
        printf("Registro nao encontrado na arvore B+.\n");
    }*/

    /*inicializar_aerodromo(&aerodromo);
    if (ler_linha_csv(ARQUIVO_CSV, &aerodromo, resultado->linha_tabela)) {
        printf("\n--- DADOS DO AERODROMO ENCONTRADO ---\n");
        imprimir_aerodromo(&aerodromo);
        printf("---------------------------------------\n");
    }*/
    imprimir_relatorio(arquivo_csv, raiz_bm);
    fclose(arquivo_csv);

    liberar_registros(&registros, &num_registros);
    /*imprimir_arvore(raiz_bm);*/
    liberar_arvore(&raiz);
    liberar_arvore_bm(&raiz_bm);
    return 0;
}
