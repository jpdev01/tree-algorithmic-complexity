#include "trees/avl.c"
#include "trees/rn.c"
#include "trees/b.c"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define NUMBER_SETS 10

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

FILE* getFile(char* currentPath, char * fileName) {
    char csvPath[FILENAME_MAX];
    snprintf(csvPath, FILENAME_MAX, "%s%ccsv%c", currentPath, SEP, SEP);

    char addFilePath[FILENAME_MAX];
    snprintf(addFilePath, FILENAME_MAX, fileName, csvPath);

    FILE* arquivo = fopen(addFilePath, "w");
    return arquivo;
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

    FILE* arquivomedioadd = getFile(currentPath, "%savgCaseAdd.csv");
    if (arquivomedioadd == NULL) {
        printf("Erro ao abrir arquivo csv da média da adição\n");
        free(currentPath);
        fclose(arquivomedioadd);
        return;
    }

    FILE* arquivomediorem = getFile(currentPath, "%savgCaseRem.csv");
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

        for (int numero = 0; numero < NUMBER_SETS; numero++) {
            popula(v, j);

            ArvoreAVL* arvoreAVL = criarArvoreAVL();
            ArvoreRN* arvoreRN = criarArvoreRN();
            ArvoreB* arvoreBOrdem1 = criarArvoreB(1);
            ArvoreB* arvoreBOrdem5 = criarArvoreB(5);
            ArvoreB* arvoreBOrdem10 = criarArvoreB(10);

            for (int i = 0; i < j; i++) {
                int valueToAdd = v[i];
                avlCount = 0;
                adicionarValorAVL(arvoreAVL, valueToAdd);
                media_avl += avlCount;

                rnCount = 0;
                adicionarValorRN(arvoreRN, valueToAdd);
                media_rn += rnCount;

                bCount = 0;
                adicionaChaveB(arvoreBOrdem1, valueToAdd);
                media_b1 += bCount;

                bCount = 0;
                adicionaChaveB(arvoreBOrdem5, valueToAdd);
                media_b5 += bCount;

                bCount = 0;
                adicionaChaveB(arvoreBOrdem10, valueToAdd);
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

        fprintf(arquivomedioadd,
                "%d;%ld;%ld;%ld;%ld;%ld\n",
                j,
                media_rn / NUMBER_SETS,
                media_avl / NUMBER_SETS,
                media_b1 / NUMBER_SETS,
                media_b5 / NUMBER_SETS,
                media_b10 / NUMBER_SETS
        );

        fprintf(arquivomediorem,
                "%d;%ld;%ld;%ld;%ld;%ld\n",
                j,
                media_rn_remocao / NUMBER_SETS,
                media_avl_remocao / NUMBER_SETS,
                media_b1_remocao / NUMBER_SETS,
                media_b5_remocao / NUMBER_SETS,
                media_b10_remocao / NUMBER_SETS
        );
    }

    fclose(arquivomedioadd);
    fclose(arquivomediorem);

    free(currentPath);

    system("python graph.py");
}
