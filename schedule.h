#pragma once
#include "matrix.h"
#include "lattice.h"

int round_up(int x, int y);

int get_chunk(int array_size, int mpi_size);

void sched_matrix(_matrix* matrix, _lattice* lattice);
