#pragma once

#include "matrix.h"
#include "lattice.h"

int round_up(int x, int y);

int get_chunk(int array_size, int mpi_size);

_matrix* sched_matrix_a(_matrix* matrix_a, _lattice* lattice);

_matrix* sched_matrix_b(_matrix* matrix_b, _lattice* lattice);

_matrix* build_matrix(_matrix* matrix_c, _lattice* lattice, int height, int wight);
