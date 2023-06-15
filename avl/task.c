#include <stdio.h>
#include "tree.c"
#include "noOperations.c"

Arvore* build() {
    Arvore* arvore = cria();
    adicionar(arvore, 4);
    adicionar(arvore, 2);
    adicionar(arvore, 8);
    adicionar(arvore, 1);
    adicionar(arvore, 3);
    adicionar(arvore, 6);
    adicionar(arvore, 9);
    adicionar(arvore, 5);
    adicionar(arvore, 7);

    return arvore;
}

void task() {
    Arvore* arvore = build();

    printf("---Q1----\n");
    printf("Fator de balanceamento: %i", fb(arvore->raiz));

    printf("\n---Q2----\n");
    printf("Remover nó 6:\n");
    removeNo(arvore, arvore->raiz->direita->esquerda);
    printf("Fator de balanceamento removendo o nó 6: %i", fb(arvore->raiz));
    removeNo(arvore, arvore->raiz->direita);
    printf("\nFator de balanceamento removendo o nó 8: %i", fb(arvore->raiz));
}