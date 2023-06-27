#include "avl/avl.c"
#include "rb/rb.c"
#include "b/b.c"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int findValueInArray(int array[], int size, int value) {
    for (int i = 0; i < size; i++) {
        if (array[i] == value) {
            return 1; // found
        }
    }

    return 0; // not found
}

void popula(int vetor[], int tamanho) {
    int numero;
    int i = 0;
    while (i != tamanho) {
        numero = rand() % 5000;
        if (findValueInArray(vetor, tamanho, numero) == 0) {
            vetor[i] = numero;
            i++;
        }
    }
}

void main() {
    extern long int avlCount;
    extern long int bCount;

    srand(time(NULL));

    FILE *arquivomedio;
    FILE *arquivopior;

    arquivomedio = fopen("/home/asaas/CLionProjects/tree-algorithmic-complexity/output/worstCase.csv", "w");
    if (arquivomedio == NULL) {
        printf("Erro ao abrir arquivo Caso Medio\n");
        return;
    }

    arquivopior = fopen("/home/asaas/CLionProjects/tree-algorithmic-complexity/output/piorCaso.csv", "w");
    if (arquivopior == NULL) {
        printf("Erro ao abrir arquivo PiorCaso\n");
        return;
    }

    fprintf(arquivomedio, "%s", "n;RN;AVL;B-1;B-5;B-10\n");
    fprintf(arquivopior, "%s", "n;RN;AVL;B-1;B-5;B-10\n");

    int numRegistros = 1000;

    for (int j = 1; j < numRegistros; j++) {
        long int media_rn = 0, media_avl = 0, media_b1 = 0, media_b5 = 0, media_b10 = 0;
        //printf("Execucao: %d\n", j);
        //Rb_arvore a com Caso Medio
        int* v = malloc(j * sizeof(int));

        for (int numero = 0; numero < 10; numero++) {
            popula(v, j);

            Arvore *arvoreAVL = cria();
            Rb_arvore* rbArvore = rb_criarArvore();
            ArvoreB* arvoreBOrdem1 = criaArvoreB(1);
            ArvoreB* arvoreBOrdem5 = criaArvoreB(5);
            ArvoreB* arvoreBOrdem10 = criaArvoreB(10);

            for (int i = 0; i < j; i++) {
                avlCount = 0;
                adicionar(arvoreAVL, v[i]);
                media_avl += avlCount;

                RNcontador=0;
                rb_adicionar(rbArvore, v[i]);
                media_rn += RNcontador;

                bCount = 0;
                adicionaChaveB(arvoreBOrdem1, v[i]);
                media_b1 += bCount;

                bCount = 0;
                adicionaChaveB(arvoreBOrdem5, v[i]);
                media_b5 += bCount;

                bCount = 0;
                adicionaChaveB(arvoreBOrdem10, v[i]);
                media_b10 += bCount;
            }

            free(arvoreAVL);
            free(rbArvore);
            free(arvoreBOrdem1);
            free(arvoreBOrdem5);
            free(arvoreBOrdem10);
        }

        fprintf(arquivomedio, "%d;%ld;", j, media_rn / 10);
        fprintf(arquivomedio, "%ld;", media_avl / 10);
        fprintf(arquivomedio, "%ld;", media_b1 / 10);
        fprintf(arquivomedio, "%ld;", media_b5 / 10);
        fprintf(arquivomedio, "%ld", media_b10 / 10);
        fprintf(arquivomedio, "\n");

        //Arvore com Pior Caso
        Arvore *arvoreAVLPior = cria();
        Rb_arvore* rnPior = rb_criarArvore();
        ArvoreB* arvoreBOrdem1Pior = criaArvoreB(1);
        ArvoreB* arvoreBOrdem5Pior = criaArvoreB(5);
        ArvoreB* arvoreBOrdem10Pior = criaArvoreB(10);

        long int bContador1 = 0, bContador5 = 0, bContador10 = 0, contadorRN = 0, avlContador = 0;
        for (int i = 1; i <= j; i++) {
            avlCount = 0;
            adicionar(arvoreAVLPior, i);
            avlContador += avlCount;

            RNcontador = 0;
            rb_adicionar(rnPior, i);
            contadorRN += RNcontador;
            
            bCount = 0;
            adicionaChaveB(arvoreBOrdem1Pior, v[i]);
            bContador1 += bCount;

            bCount = 0;
            adicionaChaveB(arvoreBOrdem5Pior, v[i]);
            bContador5 += bCount;

            bCount = 0;
            adicionaChaveB(arvoreBOrdem10Pior, v[i]);
            bContador10 += bCount;
        }

        free(arvoreAVLPior);
        free(rnPior);
        free(arvoreBOrdem1Pior);
        free(arvoreBOrdem5Pior);
        free(arvoreBOrdem10Pior);

        fprintf(arquivopior, "%d;%ld;", j, contadorRN);
        fprintf(arquivopior, "%ld;", avlContador);
        fprintf(arquivopior, "%ld;", bContador1);
        fprintf(arquivopior, "%ld;", bContador5);
        fprintf(arquivopior, "%ld", bContador10);
        fprintf(arquivopior, "\n");
    }

    fclose(arquivomedio);
    fclose(arquivopior);
    printf("FIM");
}