#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct no {
    int total;
    int* chaves;
    struct no** filhos;
    struct no* pai;
} No;

typedef struct arvoreB {
    No* raiz;
    int ordem;
} ArvoreB;

ArvoreB* criaArvore(int);
No* criaNo(ArvoreB*);
void percorreArvore(No*);
int pesquisaBinaria(No*, int);
int localizaChave(ArvoreB*, int);
No* localizaNo(ArvoreB*, int);
void adicionaChaveNo(No*, No*, int);
int transbordo(ArvoreB*, No*);
No* divideNo(ArvoreB*, No*);
void adicionaChaveRecursivo(ArvoreB*, No*, No*, int);
void adicionaChave(ArvoreB*, int);

// Implementação de remoção

void removeChave(ArvoreB* arvore, int chave);
void removeChaveRecursivo(ArvoreB* arvore, No* no, int chave);
void removerChaveNo(No* no, int indice);
void redistribuirChaves(No* pai, No* filhoEsq, No* filhoDir, int indiceFilhoEsq);

ArvoreB* criaArvore(int ordem) {
    ArvoreB* a = malloc(sizeof(ArvoreB));
    a->ordem = ordem;
    a->raiz = criaNo(a);

    return a;
}

int contador = 0;

No* criaNo(ArvoreB* arvore) {
    int max = arvore->ordem * 2;
    No* no = malloc(sizeof(No));

    no->pai = NULL;

    no->chaves = malloc(sizeof(int) * (max + 1));
    no->filhos = malloc(sizeof(No) * (max + 2));
    no->total = 0;

    for (int i = 0; i < max + 2; i++)
        no->filhos[i] = NULL;

    return no;
}

void percorreArvore(No* no) {
    if (no != NULL) {
        for (int i = 0; i < no->total; i++) {
            percorreArvore(no->filhos[i]); //visita o filho a esquerda

            printf("%d ", no->chaves[i]);
        }

        percorreArvore(no->filhos[no->total]); //visita ultimo filho (direita)
    }
}

int pesquisaBinaria(No* no, int chave) {
    int inicio = 0, fim = no->total - 1, meio;

    while (inicio <= fim) {
        contador++;

        meio = (inicio + fim) / 2;

        if (no->chaves[meio] == chave) {
            return meio; //encontrou	
        }
        else if (no->chaves[meio] > chave) {
            fim = meio - 1;
        }
        else {
            inicio = meio + 1;
        }
    }
    return inicio; //n o encontrou	
}

int localizaChave(ArvoreB* arvore, int chave) {
    No* no = arvore->raiz;

    while (no != NULL) {
        int i = pesquisaBinaria(no, chave);

        if (i < no->total && no->chaves[i] == chave) {
            return 1; //encontrou
        }
        else {
            no = no->filhos[i];
        }
    }

    return 0; //n o encontrou	
}

No* localizaNo(ArvoreB* arvore, int chave) {
    No* no = arvore->raiz;

    while (no != NULL) {
        contador++;

        int i = pesquisaBinaria(no, chave);

        if (no->filhos[i] == NULL)
            return no; //encontrou n 
        else
            no = no->filhos[i];
    }

    return NULL; //n o encontrou nenhum n 
}

void adicionaChaveNo(No* no, No* novo, int chave) {
    int i = pesquisaBinaria(no, chave);

    contador++;

    for (int j = no->total - 1; j >= i; j--) {
        no->chaves[j + 1] = no->chaves[j];
        no->filhos[j + 2] = no->filhos[j + 1];
    }

    no->chaves[i] = chave;
    no->filhos[i + 1] = novo;

    no->total++;
}

int transbordo(ArvoreB* arvore, No* no) {
    contador++;

    return no->total > arvore->ordem * 2;
}

No* divideNo(ArvoreB* arvore, No* no) {
    int meio = no->total / 2;
    No* novo = criaNo(arvore);
    novo->pai = no->pai;

    contador++;

    for (int i = meio + 1; i < no->total; i++) {
        novo->filhos[novo->total] = no->filhos[i];
        novo->chaves[novo->total] = no->chaves[i];
        if (novo->filhos[novo->total] != NULL) novo->filhos[novo->total]->pai = novo;

        novo->total++;
    }

    novo->filhos[novo->total] = no->filhos[no->total];
    if (novo->filhos[novo->total] != NULL) novo->filhos[novo->total]->pai = novo;
    no->total = meio;
    return novo;
}

void adicionaChaveRecursivo(ArvoreB* arvore, No* no, No* novo, int chave) {
    contador++;

    adicionaChaveNo(no, novo, chave);

    if (transbordo(arvore, no)) {
        int promovido = no->chaves[arvore->ordem];
        No* novo = divideNo(arvore, no);

        if (no->pai == NULL) {
            contador++;

            No* pai = criaNo(arvore);
            pai->filhos[0] = no;
            adicionaChaveNo(pai, novo, promovido);

            no->pai = pai;
            novo->pai = pai;
            arvore->raiz = pai;
        }
        else
            adicionaChaveRecursivo(arvore, no->pai, novo, promovido);
    }
}

void adicionaChave(ArvoreB* arvore, int chave) {
    No* no = localizaNo(arvore, chave);

    adicionaChaveRecursivo(arvore, no, NULL, chave);
}

void removeChave(ArvoreB* arvore, int chave) {
    removeChaveRecursivo(arvore, arvore->raiz, chave);
}

void removeChaveRecursivo(ArvoreB* arvore, No* no, int chave) {
    if (no == NULL)
        return;

    int indice = pesquisaBinaria(no, chave);

    if (indice < no->total && no->chaves[indice] == chave) {
        if (no->filhos[indice] != NULL) {
            // Caso 1: O nó contém a chave e é um nó interno da árvore B
            No* noSubstituto = no->filhos[indice + 1];

            // Encontra o nó mais à esquerda na subárvore direita
            while (noSubstituto->filhos[0] != NULL)
                noSubstituto = noSubstituto->filhos[0];

            // Substitui a chave pelo valor do nó mais à esquerda
            no->chaves[indice] = noSubstituto->chaves[0];

            // Remove a chave do nó mais à esquerda recursivamente
            removeChaveRecursivo(arvore, noSubstituto, noSubstituto->chaves[0]);
        }
        else {
            // Caso 2: O nó contém a chave e é uma folha da árvore B
            removerChaveNo(no, indice);
        }
    }
    else {
        // Caso 3: A chave não está no nó atual
        removeChaveRecursivo(arvore, no->filhos[indice], chave);
    }
}

void removerChaveNo(No* no, int indice) {
    for (int i = indice; i < no->total - 1; i++) {
        no->chaves[i] = no->chaves[i + 1];
        no->filhos[i + 1] = no->filhos[i + 2];
    }

    no->total--;
}

void redistribuirChaves(No* pai, No* filhoEsq, No* filhoDir, int indiceFilhoEsq) {
    // Move a chave do pai para o filho esquerdo
    filhoEsq->chaves[filhoEsq->total] = pai->chaves[indiceFilhoEsq];
    filhoEsq->filhos[filhoEsq->total + 1] = filhoDir->filhos[0];
    filhoEsq->total++;

    // Move a chave do filho direito para o pai
    pai->chaves[indiceFilhoEsq] = filhoDir->chaves[0];

    // Realoca as chaves e filhos no filho direito
    for (int i = 0; i < filhoDir->total - 1; i++) {
        filhoDir->chaves[i] = filhoDir->chaves[i + 1];
        filhoDir->filhos[i] = filhoDir->filhos[i + 1];
    }

    filhoDir->filhos[filhoDir->total - 1] = filhoDir->filhos[filhoDir->total];
    filhoDir->total--;
}

void analisarComplexidadeArvoreB(int ordem, int tamanhoMax, int numConjuntos) {
    ArvoreB* arvore = criaArvore(ordem);

    printf("tamanho,adicao,remocao");

    for (int i = 1; i <= tamanhoMax; i++) {
        int totalAdicao = 0;
        int totalRemocao = 0;

        for (int k = 0; k < numConjuntos; k++) {
            contador = 0;

            for (int j = 0; j < i; j++)
                adicionaChave(arvore, rand() % i);

            totalAdicao += contador;

            contador = 0;

            for (int j = 0; j < i; j++)
                removeChave(arvore, rand() % i);

            totalRemocao += contador;
        }

        int mediaAdicao = totalAdicao / numConjuntos;
        int mediaRemocao = totalRemocao / numConjuntos;

        printf("\n%d,%d,%d", i, mediaAdicao, mediaRemocao);
    }

    printf("\n--------\n");
}

int main() {
    srand(time(NULL));

    int tamanho = 1000;
    int numConjuntos = 10;

    analisarComplexidadeArvoreB(1, tamanho, numConjuntos);
    analisarComplexidadeArvoreB(5, tamanho, numConjuntos);
    analisarComplexidadeArvoreB(10, tamanho, numConjuntos);

    return 0;
}