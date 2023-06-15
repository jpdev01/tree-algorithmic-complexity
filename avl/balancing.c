int altura(No *no) {
    int esquerda = 0, direita = 0;

    if (no->esquerda != NULL) {
        esquerda = altura(no->esquerda) + 1;
    }

    if (no->direita != NULL) {
        direita = altura(no->direita) + 1;
    }

    return esquerda > direita ? esquerda : direita; //max(esquerda,direita)
}

int fb(No *no) {
    int esquerda = 0, direita = 0;

    if (no->esquerda != NULL) {
        esquerda = altura(no->esquerda) + 1;
    }

    if (no->direita != NULL) {
        direita = altura(no->direita) + 1;
    }

    return esquerda - direita;
}

No *rse(Arvore *arvore, No *no) {
    No *pai = no->pai;
    No *direita = no->direita;

    if (direita->esquerda != NULL) {
        direita->esquerda->pai = no;
    }

    no->direita = direita->esquerda;
    no->pai = direita;

    direita->esquerda = no;
    direita->pai = pai;

    if (pai == NULL) {
        arvore->raiz = direita;
    } else {
        if (pai->esquerda == no) {
            pai->esquerda = direita;
        } else {
            pai->direita = direita;
        }
    }

    return direita;
}

No *rsd(Arvore *arvore, No *no) {
    No *pai = no->pai;
    No *esquerda = no->esquerda;

    if (esquerda->direita != NULL) {
        esquerda->direita->pai = no;
    }

    no->esquerda = esquerda->direita;
    no->pai = esquerda;

    esquerda->direita = no;
    esquerda->pai = pai;

    if (pai == NULL) {
        arvore->raiz = esquerda;
    } else {
        if (pai->esquerda == no) {
            pai->esquerda = esquerda;
        } else {
            pai->direita = esquerda;
        }
    }

    return esquerda;
}

No *rde(Arvore *arvore, No *no) {
    no->direita = rsd(arvore, no->direita);
    return rse(arvore, no);
}

No *rdd(Arvore *arvore, No *no) {
    no->esquerda = rse(arvore, no->esquerda);
    return rsd(arvore, no);
}

void balanceamento(Arvore *a, No *no) {
    while (no != NULL) {
        int fator = fb(no);

        if (fator > 1) { //arvore mais profunda a esquerda
            //rotacao a direita
            if (fb(no->esquerda) > 0) {
                printf("RSD(%d)\n", no->valor);
                rsd(a, no);
            } else {
                printf("RDD(%d)\n", no->valor);
                rdd(a, no);
            }
        } else if (fator < -1) {
            //rotacao a esquerda
            if (fb(no->direita) < 0) {
                printf("RSE(%d)\n", no->valor);
                rse(a, no);
            } else {
                printf("RDE(%d)\n", no->valor);
                rde(a, no);
            }
        }

        no = no->pai;
    }
}