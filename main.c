#include "avl/avl.c"
#include "rb/rb.c"
#include "b/b.c"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int findValueInArray(int array[], int size, int value) {
    for (int i = 0; i < size; i++) {
        if (array[i] == value) {
            return 1;
        }
    }

    return 0;
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

#ifdef _WIN32
const char DIRECTORY_SEPARATOR = '\\';
#else
const char DIRECTORY_SEPARATOR = '/';
#endif

char* getCurrentPath() {
    char* currentDir = (char*)malloc(FILENAME_MAX);
    if (currentDir == NULL) {
        printf("Erro ao alocar memória\n");
        return NULL;
    }

    strcpy(currentDir, __FILE__);

    char* lastSeparator = strrchr(currentDir, DIRECTORY_SEPARATOR);
    if (lastSeparator != NULL) {
        *lastSeparator = '\0';
    }

    return currentDir;
}

void main() {
    extern long int avlCount;
    extern long int bCount;

    srand(time(NULL));

    char* currentPath = getCurrentPath();
    if (currentPath == NULL) {
        printf("Ocorreu um erro ao obter diretório raiz do projeto\n");
        return 1;
    }

    char outputPath[FILENAME_MAX];
    snprintf(outputPath, FILENAME_MAX, "%s%coutput%c", currentPath, DIRECTORY_SEPARATOR, DIRECTORY_SEPARATOR);

    char addFilePath[FILENAME_MAX];
    snprintf(addFilePath, FILENAME_MAX, "%savgCaseAdd.csv", outputPath);

    char remFilePath[FILENAME_MAX];
    snprintf(remFilePath, FILENAME_MAX, "%savgCaseRem.csv", outputPath);

    FILE* arquivomedioadd = fopen(addFilePath, "w");
    if (arquivomedioadd == NULL) {
        printf("Erro ao abrir arquivo csv da média da adição\n");
        free(currentPath);
        fclose(arquivomedioadd);
        return 1;
    }

    FILE* arquivomediorem = fopen(remFilePath, "w");
    if (arquivomediorem == NULL) {
        printf("Erro ao abrir arquivo csv da média da remoção\n");
        free(currentPath);
        fclose(arquivomedioadd);
        fclose(arquivomediorem);
        return 1;
    }

    fprintf(arquivomedioadd, "%s", "n;RN;AVL;B-1;B-5;B-10\n");
    fprintf(arquivomediorem, "%s", "n;RN;AVL;B-1;B-5;B-10\n");

    int numRegistros = 1000;

    for (int j = 1; j < numRegistros; j++) {
        long int media_rn = 0, media_avl = 0, media_b1 = 0, media_b5 = 0, media_b10 = 0;
        long int media_rn_remocao = 0, media_avl_remocao = 0, media_b1_remocao = 0, media_b5_remocao = 0, media_b10_remocao = 0;

        int* v = malloc(j * sizeof(int));

        for (int numero = 0; numero < 10; numero++) {
            int rem = rand() % j;
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
                removerChaveB(arvoreBOrdem1, rem);
                media_b1_remocao += bCount;

                bCount = 0;
                adicionaChaveB(arvoreBOrdem5, v[i]);
                media_b5 += bCount;
                bCount = 0;
                removerChaveB(arvoreBOrdem5, rem);
                media_b5_remocao += bCount;

                bCount = 0;
                adicionaChaveB(arvoreBOrdem10, v[i]);
                media_b10 += bCount;
                bCount = 0;
                removerChaveB(arvoreBOrdem10, rem);
                media_b10_remocao += bCount;
            }

            free(arvoreAVL);
            free(rbArvore);
            free(arvoreBOrdem1);
            free(arvoreBOrdem5);
            free(arvoreBOrdem10);
        }

        fprintf(arquivomedioadd, "%d;%ld;", j, media_rn / 10);
        fprintf(arquivomedioadd, "%ld;", media_avl / 10);
        fprintf(arquivomedioadd, "%ld;", media_b1 / 10);
        fprintf(arquivomedioadd, "%ld;", media_b5 / 10);
        fprintf(arquivomedioadd, "%ld", media_b10 / 10);
        fprintf(arquivomedioadd, "\n");

        fprintf(arquivomediorem, "%d;%ld;", j, media_rn_remocao / 10);
        fprintf(arquivomediorem, "%ld;", media_avl_remocao / 10);
        fprintf(arquivomediorem, "%ld;", media_b1_remocao / 10);
        fprintf(arquivomediorem, "%ld;", media_b5_remocao / 10);
        fprintf(arquivomediorem, "%ld", media_b10_remocao / 10);
        fprintf(arquivomediorem, "\n");
    }

    fclose(arquivomedioadd);
    fclose(arquivomediorem);

    free(currentPath);
}
