#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define NUM_THREADS 11

int jadval[9][9] = 
{
    {1, 3, 4, 6, 7, 8, 9, 1, 2},
    {6, 7, 2, 1, 9, 5, 3, 4, 8},
    {1, 9, 8, 3, 4, 2, 5, 6, 7},
    {8, 5, 9, 7, 6, 1, 4, 2, 3},
    {4, 2, 6, 8, 5, 3, 7, 9, 1},
    {7, 1, 3, 9, 2, 4, 8, 5, 6},
    {9, 6, 1, 5, 3, 7, 2, 8, 4},
    {2, 8, 7, 4, 1, 9, 6, 3, 5},
    {3, 4, 5, 2, 8, 6, 1, 7, 9}
};

void* checkingRows(void* arg) {
    bool* valid = malloc(sizeof(bool));
    *valid = true;

    for (int i = 0; i < 9; i++) {
        bool seen[10] = { false };
        for (int j = 0; j < 9; j++) {
            int num = jadval[i][j];
            if (num < 1 || num > 9 || seen[num]) {
                *valid = false;
                pthread_exit(valid);
            }
            seen[num] = true;
        }
    }
    pthread_exit(valid);
}

void* checkingColumns(void* arg) {
    bool* valid = malloc(sizeof(bool));
    *valid = true;

    for (int j = 0; j < 9; j++) {
        bool seen[10] = { false };
        for (int i = 0; i < 9; i++) {
            int num = jadval[i][j];
            if (num < 1 || num > 9 || seen[num]) {
                *valid = false;
                pthread_exit(valid);
            }
            seen[num] = true;
        }
    }
    pthread_exit(valid);
}

void* checkSquare(void* arg) {
    int squareNumber = *(int*)arg;
    int startRow = (squareNumber / 3) * 3;
    int startCol = (squareNumber % 3) * 3;

    bool* valid = malloc(sizeof(bool));
    *valid = true;

    bool seen[10] = { false };

    for (int i = startRow; i < startRow + 3; i++) {
        for (int j = startCol; j < startCol + 3; j++) {
            int num = jadval[i][j];
            if (num < 1 || num > 9 || seen[num]) {
                *valid = false;
                pthread_exit(valid);
            }
            seen[num] = true;
        }
    }

    pthread_exit(valid);
}

int main() {
    pthread_t threads[NUM_THREADS];
    void* results[NUM_THREADS];
    int indices[9];  // for square numbers

    pthread_create(&threads[0], NULL, checkingRows, NULL);
    pthread_create(&threads[1], NULL, checkingColumns, NULL);

    for (int i = 0; i < 9; i++) {
        indices[i] = i;
        pthread_create(&threads[i + 2], NULL, checkSquare, &indices[i]);
    }

    bool is_valid = true;

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], &results[i]);
        if (!*((bool*)results[i])) {
            is_valid = false;
        }
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        bool* result = (bool*)results[i];
        if(i==0)
            printf("ROW result: %s\n", *result ? "VALID" : "NOT VALID");
        if(i==1)
            printf("COLUMN result:  %s\n", *result ? "VALID" : "NOT VALID");    
        else printf("SQUARE %d result: %s\n", i-1, *result ? "VALID" : "NOT VALID");
    }

    if (is_valid) {
        printf("TOTAL RESULT = VALID\n");
    } else {
        printf("TOTAL RESULT = NOT VALID\n");
    }

    return 0;
}
