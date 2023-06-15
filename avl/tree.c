#include <stdlib.h>
#include <string.h>

typedef struct no {
    struct no* pai;
    struct no* esquerda;
    struct no* direita;
    int valor;
} No;

typedef struct arvore {
    struct no* raiz;
} Arvore;

Arvore* cria() {
    Arvore *arvore = malloc(sizeof(Arvore));
    arvore->raiz = NULL;

    return arvore;
}

int vazia(Arvore* arvore) {
    return arvore->raiz == NULL;
}

void removeNo(Arvore* arvore, No* no) {
    if (no->esquerda != NULL)
        removeNo(arvore, no->esquerda);

    if (no->direita != NULL)
        removeNo(arvore, no->direita);

    if (no->pai == NULL) {
        arvore->raiz = NULL;
    } else {
        if (no->pai->esquerda == no)
            no->pai->esquerda = NULL;
        else
            no->pai->direita = NULL;
    }
    free(no);
}