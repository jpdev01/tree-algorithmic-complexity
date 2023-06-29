#include <stdlib.h>
#include <stdio.h>

long int rnCount = 0;

enum coloracao { Vermelho, Preto };
typedef enum coloracao Cor;

typedef struct noRN {
    struct noRN* pai;
    struct noRN* esquerda;
    struct noRN* direita;
    Cor cor;
    int valor;
} NoRN;

typedef struct arvoreRN {
    struct noRN* raiz;
    struct noRN* nulo; 
} ArvoreRN;

NoRN* criarNoRN(ArvoreRN*, NoRN*, int);
void balancearRN(ArvoreRN*, NoRN*);
void rotacionarEsquerdaRN(ArvoreRN*, NoRN*);
void rotacionarDireitaRN(ArvoreRN*, NoRN*);
ArvoreRN* criarArvoreRN();
int vaziaRN(ArvoreRN*);
NoRN* adicionarValorRN(ArvoreRN*, int);
NoRN* localizarRN(ArvoreRN*, int);

ArvoreRN* criarArvoreRN() {
    ArvoreRN *arvoreRN = malloc(sizeof(ArvoreRN));
    arvoreRN->nulo = NULL;
    arvoreRN->raiz = NULL;

    arvoreRN->nulo = criarNoRN(arvoreRN, NULL, 0);
    arvoreRN->nulo->cor = Preto;

    return arvoreRN;
}

int vaziaRN(ArvoreRN* arvoreRN) {
    return arvoreRN->raiz == NULL;
}

NoRN* criarNoRN(ArvoreRN* arvoreRN, NoRN* pai, int valor) {
    NoRN* noRN = malloc(sizeof(NoRN));

    noRN->pai = pai;    
    noRN->valor = valor;
    noRN->direita = arvoreRN->nulo;
    noRN->esquerda = arvoreRN->nulo;

    return noRN;
}

NoRN* adicionarNoRN(ArvoreRN* arvoreRN, NoRN* noRN, int valor) {
    rnCount++;
    if (valor > noRN->valor) {
        if (noRN->direita == arvoreRN->nulo) {
            noRN->direita = criarNoRN(arvoreRN, noRN, valor);     
            noRN->direita->cor = Vermelho;       
        		
            return noRN->direita;
        } else {
            return adicionarNoRN(arvoreRN, noRN->direita, valor);
        }
    } else {
        if (noRN->esquerda == arvoreRN->nulo) {
            noRN->esquerda = criarNoRN(arvoreRN, noRN, valor);
            noRN->esquerda->cor = Vermelho;
            
            return noRN->esquerda;
        } else {
            return adicionarNoRN(arvoreRN, noRN->esquerda, valor);
        }
    }
}

NoRN* adicionarValorRN(ArvoreRN* arvoreRN, int valor) {
    rnCount++;
    if (vaziaRN(arvoreRN)) {
        arvoreRN->raiz = criarNoRN(arvoreRN, arvoreRN->nulo, valor);
        arvoreRN->raiz->cor = Preto;
        	
        return arvoreRN->raiz;
    } else {
        NoRN* noRN = adicionarNoRN(arvoreRN, arvoreRN->raiz, valor);
        balancearRN(arvoreRN, noRN);
        
        return noRN;
    }
}

void removerValorRN(ArvoreRN* arvoreRN, int valor) {
    rnCount++;
    NoRN* no = localizarRN(arvoreRN, valor);

    if (no != NULL) {
        while (1) {
            rnCount++;
            if (no->esquerda == arvoreRN->nulo && no->direita == arvoreRN->nulo) {
                rnCount++;
                if (no->pai == arvoreRN->nulo) {
                    arvoreRN->raiz = arvoreRN->nulo;
                } else if (no == no->pai->esquerda) {
                    no->pai->esquerda = arvoreRN->nulo;
                } else {
                    no->pai->direita = arvoreRN->nulo;
                }

                free(no);
                break;
            } else if (no->esquerda != arvoreRN->nulo && no->direita != arvoreRN->nulo) {
                NoRN* sucessor = no->direita;
                while (sucessor->esquerda != arvoreRN->nulo) {
                    rnCount++;
                    sucessor = sucessor->esquerda;
                }

                no->valor = sucessor->valor;
                no = sucessor;
            } else {
                rnCount++;
                NoRN* filho = (no->esquerda != arvoreRN->nulo) ? no->esquerda : no->direita;
                filho->pai = no->pai;

                if (no->pai == arvoreRN->nulo) {
                    arvoreRN->raiz = filho;
                } else if (no == no->pai->esquerda) {
                    no->pai->esquerda = filho;
                } else {
                    no->pai->direita = filho;
                }

                free(no);
                break;
            }
        }

        balancearRN(arvoreRN, arvoreRN->raiz);
    }
}

NoRN* localizarRN(ArvoreRN* arvoreRN, int valor) {
    if (!vaziaRN(arvoreRN)) {
        NoRN* noRN = arvoreRN->raiz;

        while (noRN != arvoreRN->nulo) {
            if (noRN->valor == valor) {
                return noRN;
            } else {
                noRN = valor < noRN->valor ? noRN->esquerda : noRN->direita;
            }
        }
    }

    return NULL;
}

void percorrerProfundidadeInOrder(ArvoreRN* arvoreRN, NoRN* noRN, void (*callback)(int)) {
    if (noRN != arvoreRN->nulo) {
        
        
        percorrerProfundidadeInOrder(arvoreRN, noRN->esquerda,callback);
        callback(noRN->valor);
        percorrerProfundidadeInOrder(arvoreRN, noRN->direita,callback);
    }
}

void percorrerProfundidadePreOrder(ArvoreRN* arvoreRN, NoRN* noRN, void (*callback)(int)) {
    if (noRN != arvoreRN->nulo) {
        callback(noRN->valor);
        percorrerProfundidadePreOrder(arvoreRN, noRN->esquerda,callback);
        percorrerProfundidadePreOrder(arvoreRN, noRN->direita,callback);
    }
}

void percorrerProfundidadePosOrder(ArvoreRN* arvoreRN, NoRN* noRN, void (callback)(int)) {
    if (noRN != arvoreRN->nulo) {
        percorrerProfundidadePosOrder(arvoreRN, noRN->esquerda,callback);
        percorrerProfundidadePosOrder(arvoreRN, noRN->direita,callback);
        callback(noRN->valor);
    }
}

void visitarRN(int valor){
    printf("%d ", valor);
}

void balancearRN(ArvoreRN* arvoreRN, NoRN* noRN) {
    while (noRN->pai != NULL && noRN->pai->cor == Vermelho) {
        rnCount++;
        if (noRN->pai == noRN->pai->pai->esquerda) {
            NoRN *tio = noRN->pai->pai->direita;
            
            if (tio->cor == Vermelho) {
                rnCount++;
                tio->cor = Preto;
                noRN->pai->cor = Preto; 

                noRN->pai->pai->cor = Vermelho;
                noRN = noRN->pai->pai;
            } else {
                if (noRN == noRN->pai->direita) {
                    rnCount++;
                    noRN = noRN->pai;
                    rotacionarEsquerdaRN(arvoreRN, noRN);
                } else {
                    rnCount++;
                    noRN->pai->cor = Preto; 
                    noRN->pai->pai->cor = Vermelho;
                    rotacionarDireitaRN(arvoreRN, noRN->pai->pai);
                }
            }
        } else {
            NoRN *tio = noRN->pai->pai->esquerda;
            
            if (tio->cor == Vermelho) {
                rnCount++;
                tio->cor = Preto;
                noRN->pai->cor = Preto; 

                noRN->pai->pai->cor = Vermelho;
                noRN = noRN->pai->pai;
            } else {
                if (noRN == noRN->pai->esquerda) {
                    rnCount++;
                    noRN = noRN->pai;
                    rotacionarDireitaRN(arvoreRN, noRN);
                } else {
                    rnCount++;
                    noRN->pai->cor = Preto; 
                    noRN->pai->pai->cor = Vermelho;
                    rotacionarEsquerdaRN(arvoreRN, noRN->pai->pai);
                }
            }
        }
    }
    arvoreRN->raiz->cor = Preto;
}

void rotacionarEsquerdaRN(ArvoreRN* arvoreRN, NoRN* noRN) {
    rnCount++;
    NoRN* direita = noRN->direita;
    noRN->direita = direita->esquerda; 

    if (direita->esquerda != arvoreRN->nulo) {
        direita->esquerda->pai = noRN;
    }

    direita->pai = noRN->pai;
    
    if (noRN->pai == arvoreRN->nulo) {
        arvoreRN->raiz = direita;
    } else if (noRN == noRN->pai->esquerda) {
        noRN->pai->esquerda = direita;
    } else {
        noRN->pai->direita = direita;
    }

    direita->esquerda = noRN;
    noRN->pai = direita;
}

void rotacionarDireitaRN(ArvoreRN* arvoreRN, NoRN* noRN) {
    rnCount++;
    NoRN* esquerda = noRN->esquerda;
    noRN->esquerda = esquerda->direita;
    
    if (esquerda->direita != arvoreRN->nulo) {
        esquerda->direita->pai = noRN;
    }
    
    esquerda->pai = noRN->pai;
    
    if (noRN->pai == arvoreRN->nulo) {
        arvoreRN->raiz = esquerda;
    } else if (noRN == noRN->pai->esquerda) {
        noRN->pai->esquerda = esquerda;
    } else {
        noRN->pai->direita = esquerda;
    }
    
    esquerda->direita = noRN;
    noRN->pai = esquerda;
}