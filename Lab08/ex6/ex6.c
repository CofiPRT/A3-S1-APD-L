#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define GROUP_SIZE 4

int main (int argc, char *argv[])
{
	int old_size, new_size;
	int old_rank, new_rank;
	int recv_rank;
	MPI_Comm custom_group;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &old_size); // Total number of processes.
	MPI_Comm_rank(MPI_COMM_WORLD, &old_rank); // The current process ID / Rank.

	// Split the MPI_COMM_WORLD in small groups.
	int color   = old_rank / GROUP_SIZE;	// QUOTINENT
	int key     = old_rank % GROUP_SIZE;	// REMAINDER
	MPI_Comm_split(MPI_COMM_WORLD, color, key, &custom_group);

	MPI_Comm_size(custom_group, &new_size);
	MPI_Comm_rank(custom_group, &new_rank);

	printf("Rank [%d] / size [%d] in MPI_COMM_WORLD and "
		"rank [%d] / size [%d] in custom group.\n",
		old_rank, old_size, new_rank, new_size);

	// Send the rank to the next process.
	int tag = 0;
	int next_rank; 

	if (new_rank == new_size - 1)
		next_rank = 0;
	else
		next_rank = new_rank + 1;

	MPI_Send(&new_rank, 1, MPI_INT, next_rank, tag, custom_group);

	// Receive the rank.
	int prev_rank;
	if (new_rank == 0)
		prev_rank = new_size - 1;
	else
		prev_rank = new_rank - 1;

	MPI_Status status;
	MPI_Recv(&recv_rank, 1, MPI_INT, prev_rank, tag, custom_group, &status);

	printf("Process [%d] from group [%d] received [%d].\n", new_rank,
			old_rank / GROUP_SIZE, recv_rank);

	MPI_Finalize();
}

