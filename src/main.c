#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aerodromo.h"
#include "gerenciador_csv.h"
#include "arvore-bb.h"

int main(void);

int main(void){
    Registro *registros = NULL;
    tnoe *raiz;
    int num_registros = 0, i;
    char longitude_dms[30], latitude_dms[30];

    criar_raiz(&raiz);

    if(processar_csv(ARQUIVO_CSV, &registros, &num_registros) != 0){
        printf("Erro ao processar o arquivo CSV.\n");
    }
    
    for (i = 0; i < 10; i++){
        inserir(&raiz, registros[i]);
        coordenada_decimal_para_dms(registros[i].chave.longitude, LONGITUDE, longitude_dms);
        coordenada_decimal_para_dms(registros[i].chave.latitude, LATITUDE, latitude_dms);

        printf("Registro %d: Linha tabela: %d, Longitude: %s, Longitude_d: %f, Latitude: %s, Latitude_d: %f\n", 
            i + 1, 
            registros[i].linha_tabela, 
            longitude_dms,
            registros[i].chave.longitude, 
            latitude_dms,
            registros[i].chave.latitude);
    }
    percorrer_em_ordem(raiz);
    printf("\n");
    liberar_registros(&registros, &num_registros);
    return 0;
}
