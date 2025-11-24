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
    aerodromo->operacao_diurna = DESCONHECIDO;
}

void normalizar_dms(const char *entrada, char *saida) {
    size_t i, j, inicio, fim, len;
    char c;
    
    len = strlen(entrada);
    j = 0;
    inicio = 0;
    fim = len;

    if (len >= 2 && entrada[0] == '\"' && entrada[len-1] == '\"') {
        inicio = 1;
        fim = len - 1;
    }

    for (i = inicio; i < fim; i++) {
        c = entrada[i];

        if (c == '\"') {
            if (i + 1 < fim && entrada[i+1] == '\"') {
                saida[j++] = '\"';
                i++;
            } else {
                saida[j++] = '\"';
            }
        } 
        
        else if (c == ',') {
            saida[j++] = '.';
        }
        
        else {
            if (isdigit((unsigned char)c)) {
                saida[j++] = c;
            }
            else if (c == '.' || c == '\'' || c == '-') {
                saida[j++] = c;
            }
            else if (strchr("NSEWnsew", c)) {
                saida[j++] = toupper(c);
            }
            
            else if (!isalnum((unsigned char)c) && !isspace((unsigned char)c)) {
                saida[j++] = c;
            }
            
        }
    }
    saida[j] = '\0';
}

void normalizar_regra_de_voo(const char* entrada, char* saida) {
    size_t i, j, len;
    
    len = strlen(entrada);
    j = 0;
    for (i = 0; i < len; i++) 
        if (!isspace(entrada[i])) 
            saida[j++] = entrada[i];
        
    saida[j] = '\0';
}

double dms_para_decimal(const char* dms) {
    int graus, minutos;
    double segundos, resultado;
    char direcao, *dms_aux;

    graus = 0;
    minutos = 0;
    segundos = 0.0;

    dms_aux = malloc(strlen(dms) + 1);
    if (dms_aux == NULL) {
        return 0.0;
    }
    
    normalizar_dms(dms, dms_aux);

    sscanf(dms_aux, "%d°%d'%lf\"%c", &graus, &minutos, &segundos, &direcao);
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
    char *regra_aux;

    regra_aux = malloc(strlen(regra_de_voo_str) + 1);

    normalizar_regra_de_voo(regra_de_voo_str, regra_aux);

    if (strcmp(regra_aux, "VFR") == 0) {
        return VFR;
    }
    
    if (strcmp(regra_aux, "IFR") == 0) {
        return IFR;
    }
    if (strcmp(regra_aux, "VFR/IFR") == 0) {
        return VFR_IFR;
    }

    free(regra_aux);
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

    printf("Latitude: %s\n", latitude_dms);
    printf("Longitude: %s\n", longitude_dms);
    printf("Código OACI: %s\n", aerodromo->codigo_oaci);
    printf("Nome: %s\n", aerodromo->nome);
    printf("Município: %s\n", aerodromo->municipio);
    printf("UF: %s\n", aerodromo->uf);
    printf("Altitude: %dm\n", aerodromo->altitude_em_metros);
    printf("Operação diurna: ");
    print_regra_de_voo(aerodromo->operacao_diurna);
    printf("Comprimento: %dm\n", aerodromo->comprimento_em_metros);
    printf("Largura: %dm\n", aerodromo->largura_em_metros);
}