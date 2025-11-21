#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aerodromo.h"
#include "gerenciador_csv.h"
#include "arvore_bb.h"
#include "arvore_b_mais.h"

void imprimir_hierarquia_aux(tnoe_b_mais *no, int nivel) {
    int i;
    if (no != NULL) {
        for (i = 0; i < nivel; i++) {
            printf("    ");
        }

        printf("[N%d|%s|Qtd:%d]: ", nivel, no->e_folha ? "FOLHA" : "INTERNO", no->num_chaves);

        for (i = 0; i < no->num_chaves; i++) {
            printf("(%.2f) ", no->chaves[i].latitude); 
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
    Registro *registros = NULL;
    tnoe *raiz;
    tnoe_b_mais *raiz_bm;
    int num_registros = 0, i;
    char longitude_dms[30], latitude_dms[30];

    criar_raiz(&raiz);
    criar_raiz_bm(&raiz_bm);

    if(processar_csv(ARQUIVO_CSV, &registros, &num_registros) != 0){
        printf("Erro ao processar o arquivo CSV.\n");
    }
    
    for (i = 0; i < 40; i++){
        inserir(&raiz, registros[i]);
        inserir_bm(&raiz_bm, registros[i]);
        imprimir_arvore(raiz_bm);
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
    imprimir_arvore(raiz_bm);
    liberar_arvore(&raiz);
    liberar_arvore_bm(&raiz_bm);
    liberar_registros(&registros, &num_registros);
    return 0;
}
