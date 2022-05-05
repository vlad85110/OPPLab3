#pragma once
#include <stdlib.h>
#include <mpi.h>

#define _lattice struct lattice

_lattice {
MPI_Comm comm;
int* dims;

int rank_x;
int rank_y;
};

_lattice* create_lattice();

void free_lattice(_lattice* lattice);