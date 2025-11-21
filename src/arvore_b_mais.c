#include <stdio.h>
#include <stdlib.h>
#include "arvore_b_mais.h"

void criar_raiz(tnoe_b_mais **raiz){
    *raiz = NULL;
}

tnoe_b_mais* cria_no(boolean e_folha){
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

int arvore_vazia(tnoe_b_mais *arv){
    return (arv == NULL);
}

void liberar_arvore(tnoe_b_mais **raiz){
    int i;
    tnoe_b_mais *aux;

    if (raiz == NULL || *raiz == NULL)
        return;

    aux = *raiz;
    i = 0;

    if (aux->e_folha == FALSE){
        for (i = 0; i <= aux->num_chaves; i++){
            liberar_arvore(&(aux->filhos[i]));
        }
    }

    free(aux);
    *raiz = NULL;
}

void inserir_nao_cheio(tnoe_b_mais *no, tno_b_mais dados){
    int i_vazio, comparacao;

    i_vazio = no->num_chaves;

    if (no->e_folha){

        while (i_vazio > 0 && comparar_chaves(&dados.chave, &no->chaves[i_vazio - 1]) < 0){
            no->chaves[i_vazio] = no->chaves[i_vazio - 1];
            no->dados[i_vazio] = no->dados[i_vazio - 1];
            
            i_vazio--;
        } 

        no->chaves[i_vazio] = dados.chave;
        no->dados[i_vazio] = dados;
        no->num_chaves++;
    }
    else{

    }
}

int inserir_em_no(); // Implementar

int inserir(tnoe_b_mais **raiz, tno_b_mais dados){
    int comparacao, indice_filho;
    tnoe_b_mais *aux, *nova_raiz;

    indice_filho = 0;
    aux = *raiz;

    if (arvore_vazia(aux)){
        aux = cria_no(TRUE);
        aux->chaves[0] = dados.chave;
        aux->dados[0] = dados;
        aux->num_chaves = 1;

        *raiz = aux;
        return 1;
    }
    
    if (aux->num_chaves == (2 * GRAU_MIN)){
        nova_raiz = cria_no(FALSE);

        *raiz = nova_raiz;
        nova_raiz->filhos[0] = aux;

        split();

        comparacao = comparar_chaves(&dados.chave, &nova_raiz->chaves[0]);
        if (comparacao > 0)
            indice_filho++;

        inserir_nao_cheio(nova_raiz->filhos[indice_filho], dados);
    }
    else{
        inserir_nao_cheio(aux, dados);
    }

    return 1;
}