#include <stdlib.h>
#include <stdio.h>
#include <time.h>

enum coloracao { Vermelho, Preto };
typedef enum coloracao Cor;

typedef struct no {
    struct no* pai;
    struct no* esquerda;
    struct no* direita;
    Cor cor;
    int valor;
} No;

typedef struct arvore {
    struct no* raiz;
    struct no* nulo; 
} Arvore;

long int RNcontador = 0; 

No* rb_criarNo(Arvore* arvore, No* pai, int valor);
void rb_balancear(Arvore* arvore, No* no);
void rb_rotacionarEsquerda(Arvore* arvore, No* no);
void rb_rotacionarDireita(Arvore* arvore, No* no);

Arvore* criar();
int rb_vazia(Arvore* arvore);
No* rb_adicionar(Arvore* arvore, int valor);
No* rb_localizar(Arvore* arvore, int valor);

Arvore* criar() {
    Arvore *arvore = malloc(sizeof(Arvore));
    arvore->nulo = NULL;
    arvore->raiz = NULL;

    arvore->nulo = rb_criarNo(arvore, NULL, 0);
    arvore->nulo->cor = Preto;

    return arvore;
}

int rb_vazia(Arvore* arvore) {
    return arvore->raiz == NULL;
}

No* rb_criarNo(Arvore* arvore, No* pai, int valor) {
    No* no = malloc(sizeof(No));

    no->pai = pai;
    no->valor = valor;
    no->direita = arvore->nulo;
    no->esquerda = arvore->nulo;

    return no;
}

No* rb_adicionarNo(Arvore* arvore, No* no, int valor) {
    RNcontador++;
    if (valor > no->valor) {
        if (no->direita == arvore->nulo) {
            no->direita = rb_criarNo(arvore, no, valor);
            no->direita->cor = Vermelho;       
        		
            return no->direita;
        } else {
            return rb_adicionarNo(arvore, no->direita, valor);
        }
    } else {
        if (no->esquerda == arvore->nulo) {
            no->esquerda = rb_criarNo(arvore, no, valor);
            no->esquerda->cor = Vermelho;
            
            return no->esquerda;
        } else {
            return rb_adicionarNo(arvore, no->esquerda, valor);
        }
    }
}

No* rb_adicionar(Arvore* arvore, int valor) {
    RNcontador++;
    if (rb_vazia(arvore)) {
        arvore->raiz = rb_criarNo(arvore, arvore->nulo, valor);
        arvore->raiz->cor = Preto;
        	
        return arvore->raiz;
    } else {
        No* no = rb_adicionarNo(arvore, arvore->raiz, valor);
        rb_balancear(arvore, no);
        
        return no;
    }
}

No* rb_localizar(Arvore* arvore, int valor) {
    if (!rb_vazia(arvore)) {
        No* no = arvore->raiz;

        while (no != arvore->nulo) {
            if (no->valor == valor) {
                return no;
            } else {
                no = valor < no->valor ? no->esquerda : no->direita;
            }
        }
    }

    return NULL;
}

void percorrerProfundidadeInOrder(Arvore* arvore, No* no, void (*callback)(int)) {
    if (no != arvore->nulo) {
        
        
        percorrerProfundidadeInOrder(arvore, no->esquerda,callback);
        callback(no->valor);
        percorrerProfundidadeInOrder(arvore, no->direita,callback);
    }
}

void percorrerProfundidadePreOrder(Arvore* arvore, No* no, void (*callback)(int)) {
    if (no != arvore->nulo) {
        callback(no->valor);
        percorrerProfundidadePreOrder(arvore, no->esquerda,callback);
        percorrerProfundidadePreOrder(arvore, no->direita,callback);
    }
}

void percorrerProfundidadePosOrder(Arvore* arvore, No* no, void (callback)(int)) {
    if (no != arvore->nulo) {
        percorrerProfundidadePosOrder(arvore, no->esquerda,callback);
        percorrerProfundidadePosOrder(arvore, no->direita,callback);
        callback(no->valor);
    }
}

void visitar(int valor){
    printf("%d ", valor);
}

void rb_balancear(Arvore* arvore, No* no) {
    while (no->pai->cor == Vermelho) {
        RNcontador++;
        if (no->pai == no->pai->pai->esquerda) {
            No *tio = no->pai->pai->direita;
            
            if (tio->cor == Vermelho) {
                RNcontador++;
                tio->cor = Preto; //Caso 1
                no->pai->cor = Preto; 

                no->pai->pai->cor = Vermelho; //Caso 1
                no = no->pai->pai; //Caso 1
            } else {
                if (no == no->pai->direita) {
                    RNcontador++;
                    no = no->pai; //Caso 2
                    rb_rotacionarEsquerda(arvore, no); //Caso 2
                } else {
                    RNcontador++;
                    no->pai->cor = Preto; 
                    no->pai->pai->cor = Vermelho; //Caso 3
                    rb_rotacionarDireita(arvore, no->pai->pai); //Caso 3
                }
            }
        } else {
            No *tio = no->pai->pai->esquerda;
            
            if (tio->cor == Vermelho) {
                RNcontador++;
                tio->cor = Preto; //Caso 1
                no->pai->cor = Preto; 

                no->pai->pai->cor = Vermelho; //Caso 1
                no = no->pai->pai; //Caso 1
            } else {
                if (no == no->pai->esquerda) {
                    RNcontador++;
                    no = no->pai; //Caso 2
                    rb_rotacionarDireita(arvore, no); //Caso 2
                } else {
                    RNcontador++;
                    no->pai->cor = Preto; 
                    no->pai->pai->cor = Vermelho; //Caso 3
                    rb_rotacionarEsquerda(arvore, no->pai->pai); //Caso 3
                }
            }
        }
    }
    arvore->raiz->cor = Preto; //Conserta possível quebra regra 2
}

void rb_rotacionarEsquerda(Arvore* arvore, No* no) {
    RNcontador++;
    No* direita = no->direita;
    no->direita = direita->esquerda; 

    if (direita->esquerda != arvore->nulo) {
        direita->esquerda->pai = no;
    }

    direita->pai = no->pai;
    
    if (no->pai == arvore->nulo) {
        arvore->raiz = direita;
    } else if (no == no->pai->esquerda) {
        no->pai->esquerda = direita;
    } else {
        no->pai->direita = direita;
    }

    direita->esquerda = no;
    no->pai = direita;
}

void rb_rotacionarDireita(Arvore* arvore, No* no) {
    RNcontador++;   
    No* esquerda = no->esquerda;
    no->esquerda = esquerda->direita;
    
    if (esquerda->direita != arvore->nulo) {
        esquerda->direita->pai = no;
    }
    
    esquerda->pai = no->pai;
    
    if (no->pai == arvore->nulo) {
        arvore->raiz = esquerda;
    } else if (no == no->pai->esquerda) {
        no->pai->esquerda = esquerda;
    } else {
        no->pai->direita = esquerda;
    }
    
    esquerda->direita = no;
    no->pai = esquerda;
}

