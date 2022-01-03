#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0

int main (int argc, char *argv[])
{
    int procs, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int value = rank, value_recv;
    MPI_Status status;

    for (int i = 1; i < procs; i *= 2) {
        if (rank + i < procs) {
            // this is a sender
            MPI_Send(&value, 1, MPI_INTEGER, rank + i, 1, MPI_COMM_WORLD);
        }

        if (rank >= i) {
            // this is (also) a receiver
            MPI_Recv(&value_recv, 1, MPI_INTEGER, rank - i, MPI_ANY_TAG,
                MPI_COMM_WORLD, &status);
            value += value_recv;
        }
    }

    printf("Process [%d] has result = %d\n", rank, value);

    MPI_Finalize();

}

