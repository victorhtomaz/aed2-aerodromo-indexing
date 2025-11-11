#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aerodromo.h"
#include "gerenciador_csv.h"

int main(void);

int main(void){
    Registro *registros = NULL;
    int num_registros = 0, i;
    char longitude_dms[30], latitude_dms[30];
    /*Aerodromo aerodromo1, aerodromo2;
    
    inicializar_aerodromo(&aerodromo1);
    inicializar_aerodromo(&aerodromo2);

    snprintf(aerodromo1.codigo_oaci, sizeof(aerodromo1.codigo_oaci), "%s", "SBGP");
    snprintf(aerodromo1.nome, sizeof(aerodromo1.nome), "%s", "EMBRAER - Unidade Gavião Peixoto");
    snprintf(aerodromo1.municipio, sizeof(aerodromo1.municipio), "%s", "GAVIÃO PEIXOTO");
    snprintf(aerodromo1.uf, sizeof(aerodromo1.uf), "%s", "SP");
    aerodromo1.altitude_em_metros = 609;
    aerodromo1.coordenadas = criar_coordenadas("\"48°24'17\"\"W\"", "\"21°45'52\"\"S\"");
    aerodromo1.regras_de_voo = criar_regra_de_voo("VFR");
    aerodromo1.comprimento_em_metros = 1800;
    aerodromo1.largura_em_metros = 30;

    imprimir_aerodromo(&aerodromo1);*/

    if(processar_csv(ARQUIVO_CSV, &registros, &num_registros) != 0){
        printf("Erro ao processar o arquivo CSV.\n");
    }
    
    for (i = 0; i < num_registros; i++){

        coordenada_decimal_para_dms(registros[i].coordenadas.longitude, LONGITUDE, longitude_dms);
        coordenada_decimal_para_dms(registros[i].coordenadas.latitude, LATITUDE, latitude_dms);

        printf("Registro %d: Linha tabela: %d, Longitude: %s, Longitude_d: %f, Latitude: %s, Latitude_d: %f\n", 
            i + 1, 
            registros[i].linha_tabela, 
            longitude_dms,
            registros[i].coordenadas.longitude, 
            latitude_dms,
            registros[i].coordenadas.latitude);
    }
    
    free(registros);
    return 0;
}
