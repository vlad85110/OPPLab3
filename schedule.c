#include "schedule.h"

int round_up(int x, int y) {
    return x / y + ((x % y != 0) ? 1 : 0);
}

int get_chunk(int array_size, int mpi_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    return round_up(array_size, mpi_size);
}

void sched_matrix(_matrix* matrix, _lattice* lattice) {
    int mpi_size;
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    int chunk = get_chunk(matrix->height, lattice->dims[0]);
    int tmp_size = chunk * matrix->wight;
    _matrix *tmp = create_matrix(chunk, matrix->wight);

    int var_coords[2] = {1, 0};
    MPI_Comm line;
    MPI_Cart_sub(lattice->comm, var_coords, &line);


    MPI_Scatter(matrix->matrix, 2, MPI_DOUBLE,
                tmp->matrix, 2, MPI_DOUBLE,
                0, line);

    if (lattice->rank_x == 0) {
        print_matrix(tmp);
    }



    /*int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int new_wight = get_chunk(matrix->wight, lattice->dims[1]);
    _matrix* sub_matrix = create_matrix(chunk, new_wight);

    double* arr = (double *) malloc(3* sizeof(double ));

    if (lattice->rank_y == 1) {

        print_matrix(tmp);

        MPI_Scatter(tmp->matrix, 1, MPI_DOUBLE,
                    arr, 1, MPI_DOUBLE,
                    2, lattice->comm);


        //print_matrix(sub_matrix);
    }

    /*for (int i = 0; i < lattice->dims[0]; ++i) {
        if (lattice->rank_y == i) {
            for (int j = 0; j < sub_matrix->height; ++j) {
                int root = 0;

                if (lattice->rank_x == 0) {
                    root = rank;
                }

                MPI_Scatter(tmp->matrix + j * new_wight, new_wight, MPI_DOUBLE,
                            sub_matrix->matrix + j * new_wight, new_wight, MPI_DOUBLE,
                            root, lattice->comm);
            }
        }
    }*/



    free(matrix);
    //free(sub_matrix);
    //free(tmp);
}

