#include "avl/avl.c"
#include "rb/rb.c"

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

    int v[1000];
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

    //Criação Loop 1000 registros
    // 1000
    for (int j = 1; j < 1000; j++) {
        long int media_rn = 0, media_avl = 0, media_b1 = 0, media_b5 = 0, media_b10 = 0;
        printf("Execucao: %d\n", j);
        //Rb_arvore a com Caso Medio
        for (int numero = 0; numero < 10; numero++) {
            int v[j];
            popula(v, j);

            Arvore *arvoreAVL = cria();
            Rb_arvore* rbArvore = rb_criarArvore();

            for (int i = 0; i < j; i++) {
                avlCount = 0;
                adicionar(arvoreAVL, v[i]);
                media_avl += avlCount;

                RNcontador=0;
                rb_adicionar(rbArvore, v[i]);
                media_rn+=RNcontador;
            }
            free(arvoreAVL);
            free(rbArvore);
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
        long int bContador1 = 0, bContador5 = 0, bContador10 = 0, contadorRN = 0, avlContador = 0;
        for (int i = 1; i <= j; i++) {
            avlCount = 0;
            adicionar(arvoreAVLPior, i);
            avlContador += avlCount;

            RNcontador = 0;
            rb_adicionar(rnPior, i);
            contadorRN += RNcontador;
        }
        free(arvoreAVLPior);
        free(rnPior);

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