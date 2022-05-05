#include "lattice.h"

_lattice* create_lattice() {
    _lattice* new = (_lattice*)malloc(sizeof(_lattice));
    new->dims = (int*)calloc(2, sizeof(int));

    int periods[2]={0,0}, coords[2], reorder = 1;
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD,&size);

    MPI_Dims_create(size, 2, new->dims);

    MPI_Cart_create(MPI_COMM_WORLD,2, new->dims, periods, reorder,&new->comm);

    MPI_Comm_rank(new->comm,&rank);

    MPI_Cart_get(new->comm, 2, new->dims, periods, coords);
    new->rank_y=coords[0]; new->rank_x=coords[1];

    return new;
}

void free_lattice(_lattice* lattice) {
    free(lattice->dims);
    free(lattice);
}
