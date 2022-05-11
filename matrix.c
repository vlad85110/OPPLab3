#include "matrix.h"

_matrix* create_matrix(int height, int wight) {
    _matrix* new = (_matrix*)malloc(sizeof(_matrix));
    new->height = height;
    new->wight = wight;
    new->matrix = (double*)calloc(height * wight, sizeof(double));
    return new;
}

void init(_matrix* matrix) {
    double* array = matrix->matrix;
    int height = matrix->height;
    int wight = matrix->wight;

    for (int i = 0; i < height * wight; ++i) {
        array[i] = i;
    }
}

void print_matrix(_matrix* matrix) {
    int height = matrix->height;
    int wight = matrix->wight;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < wight; ++j) {
            printf("%f ", matrix->matrix[wight * i + j]);
        }
        printf("\n");
    }
}

void free_matrix(_matrix* matrix) {
    free(matrix->matrix);
    free(matrix);
}

_matrix* multiply(_matrix* matrix1, _matrix* matrix2) {
    if (matrix1->wight != matrix2->height) {
        write(2, "err", 3);
        return NULL;
    }

    double* array1 = matrix1->matrix;
    double* array2 = matrix2->matrix;

    int height = matrix1->height;
    int wight = matrix2->wight;
    _matrix* matrix = create_matrix(height, wight);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < wight; ++j) {
            for (int k = 0; k < matrix1->wight; ++k) {
                matrix->matrix[i * wight + j] +=
                        array1[i * matrix1->wight + k] * array2[k * matrix2->wight + j];
            }
        }
    }

    return matrix;
}

