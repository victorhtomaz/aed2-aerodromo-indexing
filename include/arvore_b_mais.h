#ifndef ARVORE_B_MAIS_H
#define ARVORE_B_MAIS_H

#define GRAU_MIN 5 // Se refere ao número mínimo de chaves por nó

#include "gerenciador_csv.h"
#include "auxiliar.h"

typedef Registro tno_b_mais;

typedef struct _tnoe_b_mais{
    int num_chaves;
    boolean e_folha;
    tchave chaves[2 * GRAU_MIN];
    struct _tnoe_b_mais *filhos[2 * GRAU_MIN + 1];
    /* Caso seja folha */
    tno_b_mais dados[2 * GRAU_MIN];
    struct _tnoe_b_mais *proximo;
} tnoe_b_mais;

void criar_raiz(tnoe_b_mais **raiz);
int arvore_vazia(tnoe_b_mais *arv);
void liberar_arvore(tnoe_b_mais **raiz);
int inserir(tnoe_b_mais **raiz, tno_b_mais dados);
tnoe_b_mais* buscar(tnoe_b_mais *raiz, tchave chave);

#endif