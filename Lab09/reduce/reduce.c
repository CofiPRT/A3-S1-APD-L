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
    MPI_Status status;

    int value = rank, value_recv;

    for (int i = 2; i <= procs; i *= 2) {
        if (rank % i == 0 && rank + i / 2 < procs) {
            // this is a receiver
            MPI_Recv(&value_recv, 1, MPI_INTEGER, rank + i / 2, 0,
                MPI_COMM_WORLD, &status);
            value += value_recv;
        } else if (rank % (i / 2) == 0 && rank >= i / 2) {
            // this is a sender
            MPI_Send(&value, 1, MPI_INTEGER, rank - i / 2, 0, MPI_COMM_WORLD);
        }
    }

    if (rank == MASTER) {
        printf("Result = %d\n", value);
    }

    MPI_Finalize();

}

