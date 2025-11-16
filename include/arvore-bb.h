#ifndef ARVORE_BINARIA_BUSCA_H
#define ARVORE_BINARIA_BUSCA_H

#include "gerenciador_csv.h"

typedef Registro* tno;

typedef struct _tnoe{
    tchave chave;
    tno dados;
    struct _tnoe *esq, *dir;
} tnoe;

void criar_raiz(tnoe **raiz);
int arvore_vazia(tnoe *arv);
int inserir(tnoe **no_atual, tno dados);
int remover(tnoe *raiz, tnoe **no_atual, tchave chave); /*mudar para tchave*/
void print_chave(tchave chave);
void liberar_arvore(tnoe **raiz);
void percorrer_em_ordem(tnoe *no);

#endif