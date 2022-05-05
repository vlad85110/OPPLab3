#pragma once
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define _matrix struct matrix


_matrix {
    int height;
    int wight;
    double *matrix;
};

_matrix* create_matrix(int height, int wight);

void init(_matrix* matrix);

void print_matrix(_matrix* matrix);

void free_matrix(_matrix* matrix);

_matrix* multiply(_matrix* matrix1, _matrix* matrix2);