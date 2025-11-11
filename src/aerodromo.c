#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "aerodromo.h"

void inicializar_aerodromo(Aerodromo* aerodromo) {
    if (aerodromo == NULL) {
        return;
    }
    
    aerodromo->coordenadas.longitude = 0.0;
    aerodromo->coordenadas.latitude = 0.0;
    aerodromo->codigo_oaci[0] = '\0';
    aerodromo->nome[0] = '\0';
    aerodromo->municipio[0] = '\0';
    aerodromo->uf[0] = '\0';
    aerodromo->altitude_em_metros = 0;
    aerodromo->comprimento_em_metros = 0;
    aerodromo->largura_em_metros = 0;
    aerodromo->regras_de_voo = DESCONHECIDO;
}

double dms_para_decimal(const char* dms) {
    int graus, minutos;
    double segundos, resultado;
    char direcao, *dms_aux, *p;

    dms_aux = malloc(strlen(dms) + 1);
    if (dms_aux == NULL) {
        return 0.0;
    }
    
    strcpy(dms_aux, dms);

    for (p = dms_aux; *p; ++p) {
        if (*p == ',') {
            *p = '.';
        }
    }

    sscanf(dms_aux, "\"%d°%d'%lf\"\"%c\"", &graus, &minutos, &segundos, &direcao);
    resultado = graus + (minutos / 60.0) + (segundos / 3600.0);

    direcao = toupper(direcao);
    if (direcao == 'S' || direcao == 'W') {
        resultado = -1 * resultado;
    }

    free(dms_aux);
    return resultado;
}

Coordenadas criar_coordenadas(const char* longitude_dms, const char* latitude_dms){
    Coordenadas coords;
    double longitude;
    double latitude;

    longitude = dms_para_decimal(longitude_dms);
    latitude = dms_para_decimal(latitude_dms);

    coords.longitude = longitude;
    coords.latitude = latitude;

    return coords;
}

RegrasDeVoo criar_regra_de_voo(const char* regra_de_voo_str){
    if (strcmp(regra_de_voo_str, "VFR") == 0) {
        return VFR;
    }
    
    if (strcmp(regra_de_voo_str, "IFR") == 0) {
        return IFR;
    }
    if (strcmp(regra_de_voo_str, "VFR/IFR") == 0) {
        return VFR_IFR;
    }

    return DESCONHECIDO;
}

int comparar_coordenadas(const Coordenadas* c1, const Coordenadas* c2){
    if (c1->latitude < c2->latitude)
        return -1;
    
    if (c1->latitude > c2->latitude)
        return 1;

    if (c1->longitude < c2->longitude)
        return -1;

    if (c1->longitude > c2->longitude)
        return 1;

    return 0;
}

int comparar_aerodromos(const Aerodromo* a1, const Aerodromo* a2){
    if (a1 == NULL || a2 == NULL) {
        return 0;
    }

    return comparar_coordenadas(&a1->coordenadas, &a2->coordenadas);
}

void coordenada_decimal_para_dms(double coordenada_decimal, int tipo, char* buffer){
    int graus, minutos;
    double minutos_d, segundos, valor;
    char direcao;

    if (tipo == LATITUDE) {
        if (coordenada_decimal >= 0)
            direcao = 'N';
        else
            direcao = 'S';
    } else {
        if (coordenada_decimal >= 0)
            direcao = 'E';
        else
            direcao = 'W';
    }

    valor = fabs(coordenada_decimal);
    
    graus = (int)floor(valor);
    minutos_d = (valor - graus) * 60;
    minutos = (int)minutos_d;
    segundos = (minutos_d - minutos) * 60;

    sprintf(buffer, "%02d°%02d'%.2f\"%c", graus, minutos, segundos, direcao);
}

void print_regra_de_voo(RegrasDeVoo regra_de_voo){
    switch (regra_de_voo) {
        case VFR:
            printf("VFR\n");
            break;
        case IFR:
            printf("IFR\n");
            break;
        case VFR_IFR:
            printf("VFR / IFR\n");
            break;
        default:
            printf("Desconhecido\n");
            break;
    }
}

void imprimir_aerodromo(const Aerodromo* aerodromo){
    char longitude_dms[30], latitude_dms[30];

    if (aerodromo == NULL) {
        printf("Aerodromo não existe\n");
        return;
    }

    coordenada_decimal_para_dms(aerodromo->coordenadas.longitude, LONGITUDE, longitude_dms);
    coordenada_decimal_para_dms(aerodromo->coordenadas.latitude, LATITUDE, latitude_dms);

    printf("Código OACI: %s\n", aerodromo->codigo_oaci);
    printf("Nome: %s\n", aerodromo->nome);
    printf("Município: %s\n", aerodromo->municipio);
    printf("UF: %s\n", aerodromo->uf);
    printf("Longitude: %s\n", longitude_dms);
    printf("Latitude: %s\n", latitude_dms);
    printf("Altitude: %dm\n", aerodromo->altitude_em_metros);
    printf("Regras de voo: ");
    print_regra_de_voo(aerodromo->regras_de_voo);
    printf("Comprimento: %dm\n", aerodromo->comprimento_em_metros);
    printf("Largura: %dm\n", aerodromo->largura_em_metros);
}