#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define ROOT 0

int main (int argc, char *argv[])
{
	int  numtasks, rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	// Checks the number of processes allowed.
	if (numtasks != 2) {
		printf("[ERROR] Wrong number of processes. Only 2 allowed!\n");
		MPI_Finalize();
		return 0;
	}

	// How many numbers will be sent.
	int send_numbers = 10;

	if (rank == 0) {

		// Generate the random numbers.
		// Generate the random tags.
		// Sends the numbers with the tags to the second process.
		for(int i = 0; i < send_numbers; i++) {
			int random_number = rand() % 50;
			int random_tag = rand() % 50 + 50;

			MPI_Send(&random_number, 1, MPI_INT, ROOT + 1, random_tag,
				MPI_COMM_WORLD);
			printf("[INFO] Root process sent '%d' with tag '%d'\n",
				random_number, random_tag);
		}

	} else {

		// Receives the information from the first process.
		// Prints the numbers with their corresponding tags.
		int number;
		MPI_Status status;

		while(send_numbers--) {
			MPI_Recv(&number, 1, MPI_INT, ROOT, MPI_ANY_TAG, MPI_COMM_WORLD,
				&status);
			printf("[INFO] Process ranked '%d' received '%d' with tag '%d'\n",
				rank, number, status.MPI_TAG);
		}
	}

	MPI_Finalize();

}

