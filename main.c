#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <pthread.h>

#define ARRAY_SIZE 1000

//Estrutura de dados para poder trabalhar com threads POSIX
typedef struct {
    int *array;
    int start;
    int end;
} ThreadData;

void bubbleSortCrescente(int array[], int tamanho) {
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = 0; j < tamanho - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

void bubbleSortDecrescente(int array[], int tamanho) {
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = 0; j < tamanho - i - 1; j++) {
            if (array[j] < array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

void insertionSortCrescente(int vetor[], int tamanho) {
    for (int i = 1; i < tamanho; i++) {
        int chave = vetor[i];
        int j = i - 1;
        while (j >= 0 && vetor[j] > chave) {
            vetor[j + 1] = vetor[j];
            j--;
        }
        vetor[j + 1] = chave;
    }
}

void insertionSortDecrescente(int vetor[], int tamanho) {
    for (int i = 1; i < tamanho; i++) {
        int chave = vetor[i];
        int j = i - 1;
        while (j >= 0 && vetor[j] < chave) {
            vetor[j + 1] = vetor[j];
            j--;
        }
        vetor[j + 1] = chave;
    }
}

//FUnção que permite utilizar do BubbleSort em threads
void *threadedBubbleSort(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    for (int i = data->start; i < data->end - 1; i++) {
        for (int j = data->start; j < data->end - (i - data->start) - 1; j++) {
            if (data->array[j] > data->array[j + 1]) {
                int temp = data->array[j];
                data->array[j] = data->array[j + 1];
                data->array[j + 1] = temp;
            }
        }
    }
    return NULL;
}

void myBubbleSort(int *array, int arraySize) {
    //Verifica se a array é maior que 80, se não for, incializa o sistema de threads
    if (arraySize <= 80) {
        bubbleSortCrescente(array, arraySize);
        return;
    }

    int partSize = arraySize / 3;
    pthread_t threads[3];
    ThreadData threadData[3];

    for (int i = 0; i < 3; i++) {
        threadData[i].array = array;
        threadData[i].start = i * partSize;
        threadData[i].end = (i == 2) ? arraySize : (i + 1) * partSize;
        pthread_create(&threads[i], NULL, threadedBubbleSort, &threadData[i]);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    bubbleSortCrescente(array, arraySize);
}

void copyArray(int *source, int *dest, int size) {
    for (int i = 0; i < size; i++) {
        dest[i] = source[i];
    }
}

void fillArray(int *array, int arraySize) {
    srand(time(NULL));
    for (int i = 0; i < arraySize; i++) {
        array[i] = rand() % 10000;
    }
}

void printArray(int *array, int arraySize) {
    for (int i = 0; i < arraySize; i++) {
        printf("%d, ", array[i]);
    }
    printf("\n");
}

void printFirstElements(int *array, int arraySize) {
    int limit = arraySize < 20 ? arraySize : 20;
    for (int i = 0; i < limit; i++) {
        printf("%d, ", array[i]);
    }
    printf("\n");
}

void printLastElements(int *array, int arraySize) {
    int start = arraySize > 20 ? arraySize - 20 : 0;
    for (int i = start; i < arraySize; i++) {
        printf("%d, ", array[i]);
    }
    printf("\n");
}

int main() {
    //Inicializando as array com malloc para a proposta do trabalho
    int *array = (int *)malloc(ARRAY_SIZE * sizeof(int));
    int *insertionArrayCre = (int *)malloc(ARRAY_SIZE * sizeof(int));
    int *insertionArrayDec = (int *)malloc(ARRAY_SIZE * sizeof(int));
    int *bubbleSortArrayCre = (int *)malloc(ARRAY_SIZE * sizeof(int));
    int *bubbleSortArrayDec = (int *)malloc(ARRAY_SIZE * sizeof(int));
    int *parallelArray = (int *)malloc(ARRAY_SIZE * sizeof(int));

    printf("Gerando o vetor com %d elementos de forma aleatória...\n", ARRAY_SIZE);
    fillArray(array, ARRAY_SIZE);

    printf("20 primeiros elementos: ");
    printFirstElements(array, ARRAY_SIZE);
    printf("20 últimos elementos: ");
    printLastElements(array, ARRAY_SIZE);
    printf("\n");

    printf("Criando cópias do vetor para aplicar os algoritmos de ordenação...\n");
    copyArray(array, insertionArrayCre, ARRAY_SIZE);
    copyArray(array, insertionArrayDec, ARRAY_SIZE);
    copyArray(array, bubbleSortArrayCre, ARRAY_SIZE);
    copyArray(array, bubbleSortArrayDec, ARRAY_SIZE);
    copyArray(array, parallelArray, ARRAY_SIZE);

    // Bubble Sort Crescente
    printf("Aplicando Bubble Sort Crescente:\n");
    bubbleSortCrescente(bubbleSortArrayCre, ARRAY_SIZE);
    printFirstElements(bubbleSortArrayCre, ARRAY_SIZE);
    printLastElements(bubbleSortArrayCre, ARRAY_SIZE);

    // Bubble Sort Decrescente
    printf("\nAplicando Bubble Sort Decrescente:\n");
    bubbleSortDecrescente(bubbleSortArrayDec, ARRAY_SIZE);
    printFirstElements(bubbleSortArrayDec, ARRAY_SIZE);
    printLastElements(bubbleSortArrayDec, ARRAY_SIZE);

    // Insertion Sort Crescente
    printf("\nAplicando Insertion Sort Crescente:\n");
    insertionSortCrescente(insertionArrayCre, ARRAY_SIZE);
    printFirstElements(insertionArrayCre, ARRAY_SIZE);
    printLastElements(insertionArrayCre, ARRAY_SIZE);

    // Insertion Sort Decrescente
    printf("\nAplicando Insertion Sort Decrescente:\n");
    insertionSortDecrescente(insertionArrayDec, ARRAY_SIZE);
    printFirstElements(insertionArrayDec, ARRAY_SIZE);
    printLastElements(insertionArrayDec, ARRAY_SIZE);

    // Bubble Sort com paralelismo (myBubbleSort)
    printf("\nAplicando myBubbleSort (com paralelismo se array > 80):\n");
    myBubbleSort(parallelArray, ARRAY_SIZE);
    printFirstElements(parallelArray, ARRAY_SIZE);
    printLastElements(parallelArray, ARRAY_SIZE);

    free(array);
    free(insertionArrayCre);
    free(insertionArrayDec);
    free(bubbleSortArrayCre);
    free(bubbleSortArrayDec);
    free(parallelArray);

    return 0;
}
