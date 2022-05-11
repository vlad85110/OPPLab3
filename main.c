#include "matrix.h"
#include "lattice.h"
#include "schedule.h"

int main() {
    MPI_Init(NULL, NULL);

    _lattice* lattice = create_lattice();
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    _matrix* matrix_a = create_matrix(1600, 1600);
    _matrix* matrix_b = create_matrix(1600, 1600);
    init(matrix_a);
    init(matrix_b);

    _matrix* sub_a = sched_matrix_a(matrix_a, lattice);
    _matrix* sub_b = sched_matrix_b(matrix_b, lattice);
    _matrix* sub_c = multiply(sub_a, sub_b);

    /*for (int i = 0; i < size; ++i) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == i) {
            print_matrix(sub_c);
            printf("%d-%d\n", lattice->rank_y, lattice->rank_x);
        }
    }*/

    _matrix* matrix_c = build_matrix(sub_c, lattice, matrix_a->height, matrix_b->wight);

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        //printf("\n");
        //print_matrix(multiply(matrix_a, matrix_b));
    }

    free_matrix(matrix_b);
    free_matrix(matrix_a);
    free_matrix(matrix_c);
    free_matrix(sub_b);
    free_matrix(sub_a);
    free_matrix(sub_c);

    MPI_Finalize();
    return 0;
}
