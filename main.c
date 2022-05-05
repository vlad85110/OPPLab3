#include "matrix.h"
#include "lattice.h"
#include "schedule.h"

int main() {
    MPI_Init(NULL, NULL);

    _lattice* lattice = create_lattice();
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    _matrix* matrix = create_matrix(2, 2);
    init(matrix);

    /*for (int i = 0; i < 1; ++i) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0) {
            //printf("%d %d\n", lattice->rank_y, lattice->rank_x);
            //print_matrix(matrix);
        }
    }*/
    sched_matrix(matrix, lattice);

    MPI_Finalize();
}
