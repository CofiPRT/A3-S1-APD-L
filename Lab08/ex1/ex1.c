#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
	int numtasks, rank;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	int recv_num;

	// First process starts the circle.
	if (rank == 0) {
		// First process starts the circle.
		// Generate a random number.
		// Send the number to the next process.

		int reasonable_number = rand() % 50;
		// FULL SEND IT
		MPI_Send(&reasonable_number, 1, MPI_INT, (rank + 1) % numtasks, 0,
			MPI_COMM_WORLD);
		printf("[INFO] The process ranked '%d' has generated '%d'\n",
			rank, reasonable_number);

		MPI_Recv(&recv_num, 1, MPI_INT, numtasks - 1, 0, MPI_COMM_WORLD,
			&status);
		printf("[INFO] The process ranked '%d' received '%d' with tag '%d'\n",
			rank, recv_num, status.MPI_TAG);

	} else if (rank == numtasks - 1) {
		// Last process close the circle.
		// Receives the number from the previous process.
		// Increments the number.
		// Sends the number to the first process.
		
		MPI_Recv(&recv_num, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
		printf("[INFO] The process ranked '%d' received '%d' with tag '%d'\n",
			rank, recv_num, status.MPI_TAG);

		// Route to ROOT
		recv_num += 2;
		MPI_Send(&recv_num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		printf("[INFO] The process ranked '%d' has sent '%d'\n", rank,
			recv_num);

	} else {
		// Middle process.
		// Receives the number from the previous process.
		// Increments the number.
		// Sends the number to the next process.

		MPI_Status status;
		MPI_Recv(&recv_num, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
		printf("[INFO] The process ranked '%d' received '%d' with tag '%d'\n",
			rank, recv_num, status.MPI_TAG);

		// Route another number
		recv_num += 2;
		MPI_Send(&recv_num, 1, MPI_INT, (rank + 1) % numtasks, 0,
			MPI_COMM_WORLD);
		printf("[INFO] The process ranked '%d' has sent '%d'\n", rank,
			recv_num);
	}

	MPI_Finalize();

}

