#include "schedule.h"

int round_up(int x, int y) {
    return x / y + ((x % y != 0) ? 1 : 0);
}

int get_chunk(int array_size, int mpi_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    return round_up(array_size, mpi_size);
}

_matrix* sched_matrix_a(_matrix* matrix_a, _lattice* lattice) {
    int mpi_size;
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    int chunk = get_chunk(matrix_a->height, lattice->dims[0]);
    int tmp_size = chunk * matrix_a->wight;
    _matrix *tmp = create_matrix(chunk, matrix_a->wight);

    int var_coords[2] = {1, 0};
    MPI_Comm line;
    MPI_Cart_sub(lattice->comm, var_coords, &line);

    MPI_Scatter(matrix_a->matrix, tmp_size, MPI_DOUBLE,
                tmp->matrix, tmp_size, MPI_DOUBLE,
                0, line);

    var_coords[0] = 0;
    var_coords[1] = 1;
    MPI_Cart_sub(lattice->comm, var_coords, &line);

    MPI_Bcast(tmp->matrix, tmp_size, MPI_DOUBLE, 0, line);

    return tmp;
}

_matrix* sched_matrix_b(_matrix* matrix_b, _lattice* lattice) {
    int chunk = get_chunk(matrix_b->wight, lattice->dims[1]);
    _matrix *tmp = create_matrix(matrix_b->height, chunk);
    int tmp_size = chunk * matrix_b->height;

    MPI_Datatype col_type, new;
    MPI_Type_vector(matrix_b->height, tmp->wight,
                    matrix_b->wight, MPI_DOUBLE, &col_type);
    MPI_Type_commit(&col_type);
    MPI_Type_create_resized(col_type, 0, chunk * sizeof(double), &new);///че это
    MPI_Type_commit(&new);

    int var_coords[2] = {0, 1};
    MPI_Comm line;
    MPI_Cart_sub(lattice->comm, var_coords, &line);


    if (lattice->rank_y == 0) {
        int dis[lattice->dims[1]];
        int res_cnt[lattice->dims[1]];
        for (int i = 0; i < lattice->dims[1]; ++i) {
            dis[i] = i;
            res_cnt[i] = 1;
        }

         MPI_Scatterv(matrix_b->matrix, res_cnt, dis, new, tmp->matrix,
                      tmp_size, MPI_DOUBLE, 0, line);
    }

    var_coords[0] = 1;
    var_coords[1] = 0;
    MPI_Cart_sub(lattice->comm, var_coords, &line);

    MPI_Bcast(tmp->matrix, tmp_size, MPI_DOUBLE, 0, line);

    return tmp;
}

_matrix* build_matrix(_matrix* sub_matrix, _lattice* lattice, int height, int wight) {
    _matrix* matrix_c = create_matrix(height, wight);

    int var_coords[2] = {0, 1};
    MPI_Comm line;
    MPI_Cart_sub(lattice->comm, var_coords, &line);
    int tmp_size = sub_matrix->height * sub_matrix->wight;

    MPI_Datatype col_type, new;
    MPI_Type_vector(sub_matrix->height, sub_matrix->wight,
                    matrix_c->wight, MPI_DOUBLE, &col_type);
    MPI_Type_commit(&col_type);
    MPI_Type_create_resized(col_type, 0, sub_matrix->wight * sizeof(double), &new);///че это
    MPI_Type_commit(&new);

    _matrix* tmp = create_matrix(sub_matrix->height, wight);

    int dis[lattice->dims[1]];
    int res_cnt[lattice->dims[1]];
    for (int i = 0; i < lattice->dims[1]; ++i) {
        dis[i] = i;
        res_cnt[i] = 1;
    }

    MPI_Gather(sub_matrix->matrix, tmp_size, MPI_DOUBLE, tmp->matrix,
               1, new, 0, line);

    /*for (int i = 0; i < lattice->dims[0]; ++i) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (lattice->rank_y == i && lattice->rank_x == 0) {
            print_matrix(tmp);
            printf("%d-%d\n", lattice->rank_y, lattice->rank_x);
        }
    }*/

    var_coords[0] = 1;
    var_coords[1] = 0;
    MPI_Cart_sub(lattice->comm, var_coords, &line);

    tmp_size = matrix_c->wight * sub_matrix->height;
    if (lattice->rank_x == 0) {
        MPI_Gather(tmp->matrix, tmp_size, MPI_DOUBLE, matrix_c->matrix,
                   tmp_size, MPI_DOUBLE, 0, line);
    }

    if (lattice->rank_x == 0 && lattice->rank_y==0) {
       // print_matrix(matrix_c);
        //printf("\n");
    }

    return matrix_c;
}