#include "trees/avl.c"
#include "trees/rn.c"
#include "trees/b.c"

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
const char SEP = '\\';
#else
const char SEP = '/';
#endif

char* getCurrentPath() {
    char* currentDir = (char*)malloc(FILENAME_MAX);
    if (currentDir == NULL) {
        printf("Erro ao alocar memória\n");
        return NULL;
    }

    strcpy(currentDir, __FILE__);

    char* lastSeparator = strrchr(currentDir, SEP);
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
        return;
    }

    char csvPath[FILENAME_MAX];
    snprintf(csvPath, FILENAME_MAX, "%s%ccsv%c", currentPath, SEP, SEP);

    char addFilePath[FILENAME_MAX];
    snprintf(addFilePath, FILENAME_MAX, "%savgCaseAdd.csv", csvPath);

    char remFilePath[FILENAME_MAX];
    snprintf(remFilePath, FILENAME_MAX, "%savgCaseRem.csv", csvPath);

    FILE* arquivomedioadd = fopen(addFilePath, "w");
    if (arquivomedioadd == NULL) {
        printf("Erro ao abrir arquivo csv da média da adição\n");
        free(currentPath);
        fclose(arquivomedioadd);
        return;
    }

    FILE* arquivomediorem = fopen(remFilePath, "w");
    if (arquivomediorem == NULL) {
        printf("Erro ao abrir arquivo csv da média da remoção\n");
        free(currentPath);
        fclose(arquivomedioadd);
        fclose(arquivomediorem);
        return;
    }

    fprintf(arquivomedioadd, "%s", "n;RN;AVL;B-1;B-5;B-10\n");
    fprintf(arquivomediorem, "%s", "n;RN;AVL;B-1;B-5;B-10\n");

    int numRegistros = 1000;

    for (int j = 1; j <= numRegistros; j++) {
        long int 
            media_rn = 0, 
            media_avl = 0, 
            media_b1 = 0, 
            media_b5 = 0, 
            media_b10 = 0, 
            media_rn_remocao = 0, 
            media_avl_remocao = 0, 
            media_b1_remocao = 0, 
            media_b5_remocao = 0, 
            media_b10_remocao = 0;

        int* v = malloc(j * sizeof(int));

        for (int numero = 0; numero < 10; numero++) {
            popula(v, j);

            ArvoreAVL* arvoreAVL = criarArvoreAVL();
            ArvoreRN* arvoreRN = criarArvoreRN();
            ArvoreB* arvoreBOrdem1 = criarArvoreB(1);
            ArvoreB* arvoreBOrdem5 = criarArvoreB(5);
            ArvoreB* arvoreBOrdem10 = criarArvoreB(10);

            for (int i = 0; i < j; i++) {
                avlCount = 0;
                adicionarValorAVL(arvoreAVL, v[i]);
                media_avl += avlCount;

                rnCount = 0;
                adicionarValorRN(arvoreRN, v[i]);
                media_rn += rnCount;

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

            for (int i = 0; i < j; i++) {
                int indiceAleatorio = rand() % (j + 1);
                int valorAleatorio = (indiceAleatorio == (j + 1)) ? -1 : v[indiceAleatorio];

                avlCount = 0;
                removerValorAVL(arvoreAVL, valorAleatorio);
                media_avl_remocao += avlCount;

                rnCount = 0;
                removerValorRN(arvoreRN, valorAleatorio);
                media_rn_remocao += rnCount;

                bCount = 0;
                removerChaveB(arvoreBOrdem1, valorAleatorio);
                media_b1_remocao += bCount;

                bCount = 0;
                removerChaveB(arvoreBOrdem5, valorAleatorio);
                media_b5_remocao += bCount;

                bCount = 0;
                removerChaveB(arvoreBOrdem10, valorAleatorio);
                media_b10_remocao += bCount;
            }

            free(arvoreAVL);
            free(arvoreRN);
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

    system("python graph.py");
}
