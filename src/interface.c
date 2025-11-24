#include <stdio.h>
#include <stdlib.h>
#include "interface.h"
#include "relatorio.h"

void limpar(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void limpar_tela(){
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

void pausar_tela(){
    int c;
    printf("\nPressione ENTER para continuar...");
    
    c = getchar();
    
    if (c != '\n' && c != EOF)
        limpar();
}

void carregar_arquivo_csv(FILE *arquivo_csv, tnoe **raiz_bb, tnoe_b_mais **raiz_b_mais){
    Registro *registros;
    int num_registros, resultado, i;
    char resposta;

    limpar_tela();

    if ((*raiz_b_mais != NULL) || (*raiz_bb != NULL)){
        printf("As árvores já foram carregadas. Deseja recarregar? (S/N)\n");
        scanf(" %c", &resposta);
        limpar();

        if (resposta != 'S' && resposta != 's'){
            printf("Operação de carregamento cancelada.\n");
            return;
        }

        liberar_arvore(raiz_bb);
        liberar_arvore_bm(raiz_b_mais);
    }

    resultado = processar_csv(arquivo_csv, &registros, &num_registros);
    if (resultado != 0){
        printf("Erro ao processar o arquivo CSV.\n");
        return;
    }

    criar_raiz(raiz_bb);
    criar_raiz_bm(raiz_b_mais);
 
    for (i = 0; i < num_registros; i++){
        inserir(raiz_bb, registros[i]);
        inserir_bm(raiz_b_mais, registros[i]);
    }

    printf("Arquivo CSV carregado com sucesso. %d registros inseridos nas árvores.\n", num_registros);
    liberar_registros(&registros, &num_registros);
}

void buscar_aerodromo_bb(FILE *arquivo_csv, tnoe *raiz_bb){
    char entrada_latitude_dms[50], entrada_longitude_dms[50];
    Aerodromo aerodromo;
    Registro *registro_encontrado;
    tchave chave_busca;
    int i;

    limpar_tela();

    if (raiz_bb == NULL){
        printf("A árvore binária de busca está vazia. Garanta que carregou o arquivo CSV.\n");
        return;
    }

    inicializar_aerodromo(&aerodromo);

    i = 0;
    do
    {
        limpar_tela();

        printf("Digite as chaves latitude e longitude no formato DMS (23°23'23.3\"S).\n");
        printf("Latitude: ");
        fgets(entrada_latitude_dms, sizeof(entrada_latitude_dms), stdin);
        printf("Longitude: ");
        fgets(entrada_longitude_dms, sizeof(entrada_longitude_dms), stdin);
        chave_busca = criar_coordenadas(entrada_longitude_dms, entrada_latitude_dms);
        i = chave_busca.latitude == 0 || chave_busca.longitude == 0;
        if (i){
            printf("Chave inválida. Tente novamente.\n");
            pausar_tela();
        }
    } while (i);
    
    printf("Buscando: \n");

    registro_encontrado = buscar(raiz_bb, chave_busca);

    if (registro_encontrado == NULL){
        printf("Registro não encontrado na árvore binária de busca.\n");
        return;
    }
    ler_linha_csv(arquivo_csv, &aerodromo, registro_encontrado->linha_tabela);
    
    printf("==================================================\n");
    printf("Aeródromo: \n");
    imprimir_aerodromo(&aerodromo);
}

void buscar_aerodromo_b_mais(FILE *arquivo_csv, tnoe_b_mais *raiz_b_mais){
    char entrada_latitude_dms[50], entrada_longitude_dms[50];
    Aerodromo aerodromo;
    Registro *registro_encontrado;
    tchave chave_busca;
    int i;

    limpar_tela();

    if (raiz_b_mais == NULL){
        printf("A árvore B+ está vazia. Garanta que carregou o arquivo CSV.\n");
        return;
    }

    inicializar_aerodromo(&aerodromo);

    i = 0;
    do
    {
        limpar_tela();

        printf("Digite as chaves latitude e longitude no formato DMS (23°23'23.3\"S).\n");
        printf("Latitude: ");
        fgets(entrada_latitude_dms, sizeof(entrada_latitude_dms), stdin);
        printf("Longitude: ");
        fgets(entrada_longitude_dms, sizeof(entrada_longitude_dms), stdin);
        chave_busca = criar_coordenadas(entrada_longitude_dms, entrada_latitude_dms);
        i = chave_busca.latitude == 0 || chave_busca.longitude == 0;
        if (i){
            printf("Chave inválida. Tente novamente.\n");
            pausar_tela();
        }
    } while (i);
    
    printf("Buscando chave: Lat: %.6f, Long: %.6f\n", chave_busca.latitude, chave_busca.longitude);

    registro_encontrado = buscar_bm(raiz_b_mais, chave_busca);

    if (registro_encontrado == NULL){
        printf("Registro não encontrado na árvore B+.\n");
        return;
    }
    ler_linha_csv(arquivo_csv, &aerodromo, registro_encontrado->linha_tabela);
    
    printf("==================================================\n");
    printf("Aeródromo: \n");
    imprimir_aerodromo(&aerodromo);
}

void gerar_relatorio(FILE *arquivo_csv, tnoe_b_mais *raiz_b_mais){
    limpar_tela();

    if (raiz_b_mais == NULL){
        printf("A árvore B+ está vazia, não é possível gerar o relátorio. Garanta que carregou o arquivo CSV.\n");
        return;
    }

    printf("Relátorio com todos os aeródromos: \n");
    imprimir_relatorio(arquivo_csv, raiz_b_mais);
}

void gerar_relatorio_paginado(FILE *arquivo_csv, tnoe_b_mais *raiz_b_mais){
    int numero_da_pagina, i, quantidade_chaves, quantidade_paginas;
    char escolha;

    limpar_tela();

    if (raiz_b_mais == NULL){
        printf("A árvore B+ está vazia, não é possível gerar o relátorio. Garanta que carregou o arquivo CSV.\n");
        return;
    }

    quantidade_chaves = contar_chaves_bm(raiz_b_mais);
    quantidade_paginas = (quantidade_chaves + (REGISTROS_POR_PAGINA) - 1) / (REGISTROS_POR_PAGINA);

    numero_da_pagina = 1;
    while (i){
        limpar_tela();
        printf("==================================================\n");
        printf("Relatório paginado - Página %d (Total: %d)\n", numero_da_pagina, quantidade_paginas);
        imprimir_relatorio_paginado(arquivo_csv, raiz_b_mais, numero_da_pagina);

        printf("==================================================\n");
        printf("Digite [>] para próxima página, [<] para página anterior ou 0 para sair: \n");
        
        scanf(" %c", &escolha);
        limpar();

        switch (escolha)
        {
        case '>':
            if (numero_da_pagina < quantidade_paginas)
                numero_da_pagina++;
            else{
                printf("Essa é a última página.\n");
                pausar_tela();
            }
            break;
        case '<':
            if (numero_da_pagina > 1)
                numero_da_pagina--;
            else{
                printf("Essa é a primeira página.\n");
                pausar_tela();
            }
            break;
        case '0':
            i = 0;
            break;
        default:
            printf("Opção inválida. Tente novamente.\n");
            pausar_tela();
            break;
        }
    }
    
}

void gerar_relatorio_paginado_busca(FILE *arquivo_csv, tnoe_b_mais *raiz_b_mais){
    int numero_da_pagina, i, quantidade_chaves, quantidade_paginas;

    limpar_tela();

    if (raiz_b_mais == NULL){
        printf("A árvore B+ está vazia, não é possível gerar o relatório. Garanta que carregou o arquivo CSV.\n");
        return;
    }

    quantidade_chaves = contar_chaves_bm(raiz_b_mais);
    quantidade_paginas = (quantidade_chaves + (REGISTROS_POR_PAGINA) - 1) / (REGISTROS_POR_PAGINA);

    i = 1;
    do {
        limpar_tela();

        printf("Digite o número da página do relátorio que deseja visualizar: (Qntd: %d)\n", quantidade_paginas == 0 ? 1 : quantidade_paginas);
        scanf(" %d", &numero_da_pagina);
        limpar();

        if (numero_da_pagina <= 0 || numero_da_pagina > quantidade_paginas){
            printf("Número de página inválido. Tente novamente: ");
            pausar_tela();
        }
        else
            i = 0;
    } while(i);

    printf("Relatório paginado - Página %d: \n", numero_da_pagina);
    imprimir_relatorio_paginado(arquivo_csv, raiz_b_mais, numero_da_pagina);
}

void executar_menu(){
    FILE *arquivo_csv;
    tnoe *raiz_bb;
    tnoe_b_mais *raiz_b_mais;
    int opcao;

    opcao = -1;

    arquivo_csv = fopen(ARQUIVO_CSV, "r");
    if (arquivo_csv == NULL){
        printf("Erro ao abrir o arquivo CSV: %s\n", ARQUIVO_CSV);
        return;
    }

    do{
        limpar_tela();
        exibir_opcoes();

        if (scanf("%d", &opcao) != 1) {

            opcao = -1;
        }
        limpar();

        switch (opcao) {
            case 1:
                printf("Carregar arquivo CSV.\n");
                carregar_arquivo_csv(arquivo_csv, &raiz_bb, &raiz_b_mais);
                break;
            case 2:
                buscar_aerodromo_bb(arquivo_csv, raiz_bb);
                break;
            case 3:
                buscar_aerodromo_b_mais(arquivo_csv, raiz_b_mais);
                break;
            case 4:
                gerar_relatorio(arquivo_csv, raiz_b_mais);
                break;
            case 5:
                gerar_relatorio_paginado(arquivo_csv, raiz_b_mais);
                break;
            case 6:
                gerar_relatorio_paginado_busca(arquivo_csv, raiz_b_mais);
                break;
            case 0:
                printf("\nSaindo do programa.\n");
                break;
            default:
                printf("Opcao invalida. Por favor, tente novamente.\n");
                break;
        }

        pausar_tela();
    } while (opcao != 0);
    
    liberar_arvore(&raiz_bb);
    liberar_arvore_bm(&raiz_b_mais);
    fclose(arquivo_csv);
}

void exibir_opcoes(){
    printf("==================================================\n");
    printf("                  AERODROMOS                      \n");
    printf("==================================================\n");

    printf("\nSelecione uma opcao: (Digite o número)\n\n");
    printf("1 - Carregar arquivo CSV\n");
    printf("2 - Busca por Aerodromo (árvore binária de busca)\n");
    printf("3 - Busca por Aerodromo (árvore B+)\n");
    printf("4 - Gerar relatório (árvore B+)\n");
    printf("5 - Gerar relatório paginado (árvore B+)\n");
    printf("6 - Gerar relatório - buscar página (árvore B+)\n");
    printf("0 - Sair\n\n");
    printf("Opção: ");
}