#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

int N;

void compareVectors(int * a, int * b) {
	// DO NOT MODIFY
	int i;
	for(i = 0; i < N; i++) {
		if(a[i]!=b[i]) {
			printf("Sorted incorrectly\n");
			return;
		}
	}
	printf("Sorted correctly\n");
}

void displayVector(int * v) {
	// DO NOT MODIFY
	int i;
	for(i = 0; i < N; i++) {
		printf("%d ", v[i]);
	}
	printf("\n");
}

int cmp(const void *a, const void *b) {
	// DO NOT MODIFY
	int A = *(int*)a;
	int B = *(int*)b;
	return A-B;
}

// Use 'mpirun -np 20 --oversubscribe ./pipeline_sort' to run the application with more processes
int main(int argc, char * argv[]) {
	int rank;
	int nProcesses;
	MPI_Init(&argc, &argv);
	MPI_Status status;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	printf("Hello from %i/%i\n", rank, nProcesses - 1);

	if (rank == 0) { // This code is run by a single process
		int initialValue = -1;
		int sorted = 0;
		int aux;
		int *v = (int*)malloc(sizeof(int) * (nProcesses - 1));
		int *vQSort = (int*)malloc(sizeof(int) * (nProcesses - 1));
		int i, val;

		// generate the vector v with random values
		// DO NOT MODIFY
		srandom(42);
		for(i = 0; i < nProcesses - 1; i++)
			v[i] = random() % 200;
		N = nProcesses - 1;
		displayVector(v);

		// make copy to check it against qsort
		// DO NOT MODIFY
		for(i = 0; i < nProcesses - 1; i++)
			vQSort[i] = v[i];
		qsort(vQSort, nProcesses - 1, sizeof(int), cmp);

		for (i = 1; i <= N; i++)
			MPI_Send(&initialValue, 1, MPI_INTEGER, i, 0, MPI_COMM_WORLD);

		for (i = 0; i < N; i++)
			MPI_Send(&v[i], 1, MPI_INTEGER, rank + 1, 0, MPI_COMM_WORLD);

		for (i = 0; i < N; i++)
			MPI_Recv(&v[i], 1, MPI_INTEGER, i + 1, 0, MPI_COMM_WORLD, &status);


		displayVector(v);
		compareVectors(v, vQSort);
	} else {
		int i, value, value_recv;

		MPI_Recv(&value, 1, MPI_INTEGER, 0, 0, MPI_COMM_WORLD, &status);
		
		for (i = 0; i < nProcesses - rank; i++) {
			MPI_Recv(&value_recv, 1, MPI_INTEGER, rank - 1, 0,
				MPI_COMM_WORLD, &status);

			if (value == -1) {
				value = value_recv;
			} else if (value <= value_recv) {
				MPI_Send(&value_recv, 1, MPI_INTEGER, rank + 1, 0, MPI_COMM_WORLD);
			} else {
				MPI_Send(&value, 1, MPI_INTEGER, rank + 1, 0, MPI_COMM_WORLD);
				value = value_recv;
			}
		}

		MPI_Send(&value, 1, MPI_INTEGER, 0, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}