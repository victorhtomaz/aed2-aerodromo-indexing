#ifndef AERODROMO_H
#define AERODROMO_H

typedef enum _tipo_coordenada{
    LONGITUDE,
    LATITUDE
} TipoCoordenada;

typedef enum _regras_de_voo{
    DESCONHECIDO,
    VFR,
    IFR,
    VFR_IFR
} RegrasDeVoo;

typedef struct _coordenadas{
    double longitude;
    double latitude;
} Coordenadas;

typedef struct _aerodromo{
    Coordenadas coordenadas;
    char codigo_oaci[5];
    char nome[255];
    char municipio[255];
    char uf[3];
    int altitude_em_metros;
    RegrasDeVoo regras_de_voo;
    int comprimento_em_metros;
    int largura_em_metros;
} Aerodromo;

void inicializar_aerodromo(Aerodromo* aerodromo);

Coordenadas criar_coordenadas(const char *longitude_dms, const char *latitude_dms);
RegrasDeVoo criar_regra_de_voo(const char *regra_de_voo_str);

int comparar_coordenadas(const Coordenadas *c1, const Coordenadas *c2); /*Retorna -1 se menor, 0 se igual, 1 se maior*/
int comparar_aerodromos(const Aerodromo *a1, const Aerodromo *a2); /*Retorna -1 se menor, 0 se igual, 1 se maior*/
void imprimir_aerodromo(const Aerodromo *aerodromo);
void coordenada_decimal_para_dms(double coordenada_decimal, int tipo, char *buffer);

#endif