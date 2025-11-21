#include <stdio.h>
#include <stdlib.h>
#include "arvore_bb.h"

void criar_raiz(tnoe **raiz){
    *raiz = NULL;
}   

tnoe* cria_no(tno dados){
    tnoe *no;

    no = (tnoe*) malloc(sizeof(tnoe));
    if (no == NULL)
        exit(1);

    no->chave = dados.chave;
    no->dados = dados;
    no->esq = NULL;
    no->dir = NULL;

    return no;
}

int arvore_vazia(tnoe *arv){
    return (arv == NULL);
}

int inserir(tnoe **no_atual, tno dados){
    int comparacao;
    tnoe *aux;

    if (arvore_vazia(*no_atual)){
        *no_atual = cria_no(dados);
    }
    else{
        aux = *no_atual;
        comparacao = comparar_chaves(&dados.chave, &aux->dados.chave);
        if (comparacao == 0)
            return -1;

        if (comparacao > 0){
            if (aux->dir == NULL){
                aux->dir = cria_no(dados);
            }
            else{
                aux = aux->dir;
                inserir(&aux, dados);
            }
        }
        else{
            if (aux->esq == NULL){
                aux->esq = cria_no(dados);
            }
            else{
                aux = aux->esq;
                inserir(&aux, dados);
            }
        }
    }

    return 1;
}

int remover(tnoe *raiz, tnoe **no_atual, tchave chave){
    tnoe *aux, *sucessor, *ant_sucessor;
    int comparacao;

    aux = *no_atual;
    comparacao = comparar_chaves(&chave, &aux->chave);

    if (aux == NULL)
        return 0;
    else if (comparacao < 0)
        remover(raiz, &(aux->esq), chave);
    else if (comparacao > 0)
        remover(raiz, &(aux->dir), chave);
    else{
        /*Apagar sem filhos*/
        if (aux->esq == NULL && aux->dir == NULL)
            *no_atual = NULL;
        /*Apagar com um filho*/
        else if (aux->esq == NULL)
            *no_atual = aux->dir;
        else if (aux->dir == NULL)
            *no_atual = aux->esq;
        /*Apagar com dois filhos*/
        else{
            ant_sucessor = aux;
            sucessor = aux->dir;

            while(sucessor->esq != NULL){
                ant_sucessor = sucessor;
                sucessor = sucessor->esq;
            }

            sucessor->esq = aux->esq;

            if (ant_sucessor != aux){
                ant_sucessor->esq = sucessor->dir;
                sucessor->dir = aux->dir;
            }

            *no_atual = sucessor;
        }
        free(aux);
    }

    return 1;
}

void print_chave(tchave chave){
    printf("Latitude: %f", chave.latitude);
    printf(" | ");
    printf("Longitude: %f", chave.longitude);
}

void liberar_arvore(tnoe **raiz) {
    if (raiz == NULL || *raiz == NULL)
        return;
    
    liberar_arvore(&(*raiz)->esq);
    liberar_arvore(&(*raiz)->dir);
    
    free(*raiz);
    *raiz = NULL;
}

void percorrer_em_ordem(tnoe *no){
    if (no == NULL)
        return;

    percorrer_em_ordem(no->esq);
    print_chave(no->chave);
    printf(";\n");
    percorrer_em_ordem(no->dir);
}