#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "arvore_b_mais.h"

void criar_raiz_bm(tnoe_b_mais **raiz){
    *raiz = NULL;
}

tnoe_b_mais* cria_no_bm(boolean e_folha){
    int i;
    tnoe_b_mais *no;

    no = (tnoe_b_mais*) malloc(sizeof(tnoe_b_mais));
    if (no == NULL)
        exit(1);

    no->e_folha = e_folha;
    no->num_chaves = 0;
    no->proximo = NULL;

    for (i = 0; i < (2 * GRAU_MIN + 1); i++)
        no->filhos[i] = NULL;

    return no;
}

int arvore_vazia_bm(tnoe_b_mais *arv){
    return (arv == NULL);
}

void liberar_arvore_bm(tnoe_b_mais **raiz){
    int i;
    tnoe_b_mais *aux;

    if (raiz == NULL || *raiz == NULL)
        return;

    aux = *raiz;
    i = 0;

    if (aux->e_folha == FALSE){
        for (i = 0; i <= aux->num_chaves; i++){
            liberar_arvore_bm(&(aux->filhos[i]));
        }
    }

    free(aux);
    *raiz = NULL;
}

void split_folha(tnoe_b_mais *no, tno_b_mais dados, tchave *chave_promovida, tnoe_b_mais **novo_no){
    int i;
    tnoe_b_mais *nova_folha;
    tchave chave_aux[2 * GRAU_MIN + 1];
    tno_b_mais dados_aux[2 * GRAU_MIN + 1];

    nova_folha = cria_no_bm(TRUE);

    memcpy(chave_aux, no->chaves, no->num_chaves * sizeof(tchave));
    memcpy(dados_aux, no->dados, no->num_chaves * sizeof(tno_b_mais));
    
    i = 2 * GRAU_MIN;
    while (i > 0 && comparar_chaves(&dados.chave, &chave_aux[i - 1]) < 0){
        chave_aux[i] = chave_aux[i - 1];
        dados_aux[i] = dados_aux[i - 1];
        i--;
    }

    chave_aux[i] = dados.chave;
    dados_aux[i] = dados;

    no->num_chaves = GRAU_MIN;
    nova_folha->num_chaves = GRAU_MIN + 1;

    memcpy(no->chaves, chave_aux, no->num_chaves * sizeof(tchave));
    memcpy(no->dados, dados_aux, no->num_chaves * sizeof(tno_b_mais));

    memcpy(nova_folha->chaves, &chave_aux[GRAU_MIN], nova_folha->num_chaves * sizeof(tchave));
    memcpy(nova_folha->dados, &dados_aux[GRAU_MIN], nova_folha->num_chaves * sizeof(tno_b_mais));

    nova_folha->proximo = no->proximo;
    no->proximo = nova_folha;

    *chave_promovida = nova_folha->chaves[0];
    *novo_no = nova_folha;
}

void split_interno(tnoe_b_mais *no, tchave chave_split, tnoe_b_mais *novo_filho, tchave *chave_promovida, tnoe_b_mais **novo_no){
    int i;
    tnoe_b_mais *novo_interno;
    tchave chave_aux[2 * GRAU_MIN + 1];
    tnoe_b_mais *filhos_aux[2 * GRAU_MIN + 2];

    novo_interno = cria_no_bm(FALSE);

    memcpy(chave_aux, no->chaves, no->num_chaves * sizeof(tchave));
    memcpy(filhos_aux, no->filhos, (no->num_chaves + 1) * sizeof(tnoe_b_mais*));

    i = 2 * GRAU_MIN;
    while (i > 0 && comparar_chaves(&chave_split, &chave_aux[i - 1]) < 0){
        chave_aux[i] = chave_aux[i - 1];
        filhos_aux[i + 1] = filhos_aux[i];
        i--;
    }

    chave_aux[i] = chave_split;
    filhos_aux[i + 1] = novo_filho;

    *chave_promovida = chave_aux[GRAU_MIN];
    
    no->num_chaves = GRAU_MIN;
    novo_interno->num_chaves = GRAU_MIN;

    memcpy(no->chaves, chave_aux, no->num_chaves * sizeof(tchave));
    memcpy(no->filhos, filhos_aux, (no->num_chaves + 1) * sizeof(tnoe_b_mais*));

    memcpy(novo_interno->chaves, &chave_aux[GRAU_MIN + 1], novo_interno->num_chaves * sizeof(tchave));
    memcpy(novo_interno->filhos, &filhos_aux[GRAU_MIN + 1], (novo_interno->num_chaves + 1) * sizeof(tnoe_b_mais*));

    *novo_no = novo_interno;
}

int inserir_em_no(tnoe_b_mais *no, tno_b_mais dados, tchave *chave_promovida, tnoe_b_mais **novo_no){
    int i_vazio, i, j, split_realizado;
    tchave chave_split;
    tnoe_b_mais *novo_filho;

    novo_filho = NULL;
    i_vazio = no->num_chaves;

    if (no->e_folha){
        i = 0;
        
        while (i < no->num_chaves && comparar_chaves(&dados.chave, &no->chaves[i]) > 0)
            i++;
        

        if (i < no->num_chaves && comparar_chaves(&dados.chave, &no->chaves[i]) == 0) 
            return -1;
        

        /* Split nó folha se estiver cheio */
        if (no->num_chaves == 2 * GRAU_MIN){
            split_folha(no, dados, chave_promovida, novo_no);
            return 1;
        }

        while (i_vazio > 0 && comparar_chaves(&dados.chave, &no->chaves[i_vazio - 1]) < 0){
            no->chaves[i_vazio] = no->chaves[i_vazio - 1];
            no->dados[i_vazio] = no->dados[i_vazio - 1];
            
            i_vazio--;
        } 

        no->chaves[i_vazio] = dados.chave;
        no->dados[i_vazio] = dados;
        no->num_chaves++;

        return 0;
    }
    else{
        i = 0;
        j = no->num_chaves;

        while (i < no->num_chaves && comparar_chaves(&dados.chave, &no->chaves[i]) > 0)
            i++;
        
        
        split_realizado = inserir_em_no(no->filhos[i], dados, &chave_split, &novo_filho);

        if (split_realizado == -1)
            return -1;

        if (split_realizado == 0)
            return 0;
        
        /* Split nó interno se estiver cheio */
        if (no->num_chaves == 2 * GRAU_MIN){
            split_interno(no, chave_split, novo_filho, chave_promovida, novo_no);
            return 1;
        }

        while (j > i){
            no->chaves[j] = no->chaves[j - 1];
            no->filhos[j + 1] = no->filhos[j];
            j--;
        }

        no->chaves[i] = chave_split;
        no->filhos[i + 1] = novo_filho;

        no->num_chaves++;
        return 0;
    }
}

int inserir_bm(tnoe_b_mais **raiz, tno_b_mais dados){
    int split_realizado;
    tnoe_b_mais *aux, *nova_raiz, *novo_filho;
    tchave chave_split;

    aux = *raiz;

    if (arvore_vazia_bm(aux)){
        aux = cria_no_bm(TRUE);
        aux->chaves[0] = dados.chave;
        aux->dados[0] = dados;
        aux->num_chaves = 1;

        *raiz = aux;
        return 1;
    }
    
    split_realizado = inserir_em_no(aux, dados, &chave_split, &novo_filho);

    if (split_realizado == -1)
        return 0;

    if (split_realizado == 1){
        nova_raiz = cria_no_bm(FALSE);

        nova_raiz->num_chaves = 1;
        nova_raiz->chaves[0] = chave_split;
        nova_raiz->filhos[0] = aux;
        nova_raiz->filhos[1] = novo_filho;

        *raiz = nova_raiz;
    }

    return 1;
}

tno_b_mais* buscar_bm(tnoe_b_mais *no, tchave chave){
    int i, comparacao;

    if (no == NULL)
        return NULL;

    i = 0;

    if (no->e_folha == FALSE){
        while (i < no->num_chaves && comparar_chaves(&chave, &no->chaves[i]) >= 0)
            i++;

        printf("Nó interno: 1° chave:  Lat: %.6f, Long: %.6f\n", no->chaves[0].latitude, no->chaves[0].longitude);
        return buscar_bm(no->filhos[i], chave);
    }

    while (i < no->num_chaves){
        printf("Nó folha: %d° chave:  Lat: %.6f, Long: %.6f\n", i+1, no->chaves[i].latitude, no->chaves[i].longitude);
        comparacao = comparar_chaves(&chave, &no->chaves[i]);

        if (comparacao < 0)
            return NULL;
        
        if (comparacao == 0){
            printf("Chave encontrada: Lat: %.6f, Long: %.6f\n", no->chaves[i].latitude, no->chaves[i].longitude);
            return &no->dados[i];
        }
   
        i++;
    }
    
    return NULL;
}

void percorrer_em_ordem_bm(tnoe_b_mais *raiz, tno_b_mais **registros){
    int i;
    tnoe_b_mais *aux;

    aux = raiz;

    if (aux == NULL)
        return;

    while (aux->e_folha == FALSE) {
        aux = aux->filhos[0];
    }

    while (aux != NULL) {
        for (i = 0; i < aux->num_chaves; i++) {
            (*registros)->chave = aux->chaves[i];
            (*registros)->linha_tabela = aux->dados[i].linha_tabela;
            (*registros)++;
        }
        aux = aux->proximo;
    }
}

int contar_chaves_bm(tnoe_b_mais *raiz){
    tnoe_b_mais *aux = raiz;
    int total_dados = 0;

    if (aux == NULL) 
        return 0;

    while (aux->e_folha == FALSE) {
        aux = aux->filhos[0];
    }

    while (aux != NULL) {
        total_dados += aux->num_chaves;
        aux = aux->proximo;
    }

    return total_dados;
}