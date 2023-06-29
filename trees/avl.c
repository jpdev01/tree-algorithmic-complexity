#include <stdlib.h>
#include <string.h>

long int avlCount = 0;

typedef struct noAVL {
    struct noAVL* pai;
    struct noAVL* esquerda;
    struct noAVL* direita;
    int valor;
} NoAVL;

typedef struct arvoreAVL {
    struct noAVL* raiz;
} ArvoreAVL;

ArvoreAVL* criarArvoreAVL() {
    ArvoreAVL *arvoreAVL = malloc(sizeof(ArvoreAVL));
    arvoreAVL->raiz = NULL;

    return arvoreAVL;
}

int vaziaAVL(ArvoreAVL* arvoreAVL) {
    return arvoreAVL->raiz == NULL;
}

int altura(NoAVL *noAVL) {
    int esquerda = 0, direita = 0;

    if (noAVL->esquerda != NULL) {
        esquerda = altura(noAVL->esquerda) + 1;
    }

    if (noAVL->direita != NULL) {
        direita = altura(noAVL->direita) + 1;
    }

    return esquerda > direita ? esquerda : direita;
}

int fb(NoAVL *noAVL) {
    avlCount++;
    int esquerda = 0, direita = 0;

    if (noAVL->esquerda != NULL) {
        esquerda = altura(noAVL->esquerda) + 1;
    }

    if (noAVL->direita != NULL) {
        direita = altura(noAVL->direita) + 1;
    }

    return esquerda - direita;
}

NoAVL *rse(ArvoreAVL *arvoreAVL, NoAVL *noAVL) {
    avlCount++;
    NoAVL *pai = noAVL->pai;
    NoAVL *direita = noAVL->direita;

    if (direita->esquerda != NULL) {
        direita->esquerda->pai = noAVL;
    }

    noAVL->direita = direita->esquerda;
    noAVL->pai = direita;

    direita->esquerda = noAVL;
    direita->pai = pai;

    if (pai == NULL) {
        arvoreAVL->raiz = direita;
    } else {
        if (pai->esquerda == noAVL) {
            pai->esquerda = direita;
        } else {
            pai->direita = direita;
        }
    }

    return direita;
}

NoAVL *rsd(ArvoreAVL *arvoreAVL, NoAVL *noAVL) {
    avlCount++;
    NoAVL *pai = noAVL->pai;
    NoAVL *esquerda = noAVL->esquerda;

    if (esquerda->direita != NULL) {
        esquerda->direita->pai = noAVL;
    }

    noAVL->esquerda = esquerda->direita;
    noAVL->pai = esquerda;

    esquerda->direita = noAVL;
    esquerda->pai = pai;

    if (pai == NULL) {
        arvoreAVL->raiz = esquerda;
    } else {
        if (pai->esquerda == noAVL) {
            pai->esquerda = esquerda;
        } else {
            pai->direita = esquerda;
        }
    }

    return esquerda;
}

NoAVL *rde(ArvoreAVL *arvoreAVL, NoAVL *noAVL) {
    noAVL->direita = rsd(arvoreAVL, noAVL->direita);
    return rse(arvoreAVL, noAVL);
}

NoAVL *rdd(ArvoreAVL *arvoreAVL, NoAVL *noAVL) {
    noAVL->esquerda = rse(arvoreAVL, noAVL->esquerda);
    return rsd(arvoreAVL, noAVL);
}

void balanceamento(ArvoreAVL *a, NoAVL *noAVL) {
    while (noAVL != NULL) {
        avlCount++;
        int fator = fb(noAVL);

        if (fator > 1) {
            if (fb(noAVL->esquerda) > 0) {
                rsd(a, noAVL);
            } else {
                rdd(a, noAVL);
            }
        } else if (fator < -1) {
            if (fb(noAVL->direita) < 0) {
                rse(a, noAVL);
            } else {
                rde(a, noAVL);
            }
        }

        noAVL = noAVL->pai;
    }
}

NoAVL* criarNo(int valor) {
    NoAVL* noAVL = malloc(sizeof(NoAVL));
    noAVL->valor = valor;
    noAVL->pai = NULL;
    noAVL->esquerda = NULL;
    noAVL->direita = NULL;
    return noAVL;
}

NoAVL* adicionarNo(NoAVL* noAVL, int valor) {
    avlCount++;
    if (valor > noAVL->valor) {
        if (noAVL->direita == NULL) {
            NoAVL* novo = criarNo(valor);
            novo->pai = noAVL;
            noAVL->direita = novo;

            return novo;
        } else {
            return adicionarNo(noAVL->direita, valor);
        }
    } else {
        if (noAVL->esquerda == NULL) {
            NoAVL* novo = criarNo(valor);
            novo->pai = noAVL;
            noAVL->esquerda = novo;

            return novo;
        } else {
            return adicionarNo(noAVL->esquerda, valor);
        }
    }
}

NoAVL* adicionarValorAVL(ArvoreAVL* arvoreAVL, int valor) {
    avlCount++;
    if (arvoreAVL->raiz == NULL) {
        NoAVL* novo = criarNo(valor);
        arvoreAVL->raiz = novo;

        return novo;
    } else {
        avlCount++;
        NoAVL* noAVL = adicionarNo(arvoreAVL->raiz, valor);
        balanceamento(arvoreAVL, noAVL);
        return noAVL;
    }
}

void removerNoAVL(ArvoreAVL* arvoreAVL, NoAVL* noAVL) {
    if (noAVL->esquerda != NULL)
        removerNoAVL(arvoreAVL, noAVL->esquerda);

    if (noAVL->direita != NULL)
        removerNoAVL(arvoreAVL, noAVL->direita);

    if (noAVL->pai == NULL) {
        arvoreAVL->raiz = NULL;
    } else {
        if (noAVL->pai->esquerda == noAVL)
            noAVL->pai->esquerda = NULL;
        else
            noAVL->pai->direita = NULL;
    }
    free(noAVL);
}

NoAVL* localizar(NoAVL* noAVL, int valor) {
    avlCount++;
    if (noAVL->valor == valor) {
        return noAVL;
    } else {
        if (valor < noAVL->valor) {
            if (noAVL->esquerda != NULL) {
                return localizar(noAVL->esquerda, valor);
            }
        } else {
            if (noAVL->direita != NULL) {
                return localizar(noAVL->direita, valor);
            }
        }
    }

    return NULL;
}

void removerValorAVL(ArvoreAVL* arvoreAVL, int valor) {
    NoAVL* noAVL = localizar(arvoreAVL->raiz, valor);

    while (noAVL != NULL) {
        if (noAVL->esquerda == NULL && noAVL->direita == NULL) {
            if (noAVL->pai == NULL) {
                arvoreAVL->raiz = NULL;
            } else {
                if (noAVL->pai->esquerda == noAVL)
                    noAVL->pai->esquerda = NULL;
                else
                    noAVL->pai->direita = NULL;
            }
            free(noAVL);
            break;
        } else if (noAVL->esquerda != NULL && noAVL->direita != NULL) {
            NoAVL* sucessor = noAVL->direita;
            while (sucessor->esquerda != NULL) {
                avlCount++;
                sucessor = sucessor->esquerda;
            }

            noAVL->valor = sucessor->valor;
            noAVL = sucessor;
        } else {
            NoAVL* filho = (noAVL->esquerda != NULL) ? noAVL->esquerda : noAVL->direita;
            filho->pai = noAVL->pai;

            if (noAVL->pai == NULL) {
                arvoreAVL->raiz = filho;
            } else {
                if (noAVL->pai->esquerda == noAVL)
                    noAVL->pai->esquerda = filho;
                else
                    noAVL->pai->direita = filho;
            }

            free(noAVL);
            break;
        }
    }

    if (noAVL != NULL) {
        balanceamento(arvoreAVL, arvoreAVL->raiz);
    }
}
