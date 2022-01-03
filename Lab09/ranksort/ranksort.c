#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define N 1000
#define MASTER 0

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
	int displayWidth = 2 + log10(v[N-1]);
	for(i = 0; i < N; i++) {
		printf("%*i", displayWidth, v[i]);
	}
	printf("\n");
}

int cmp(const void *a, const void *b) {
	// DO NOT MODIFY
	int A = *(int*)a;
	int B = *(int*)b;
	return A-B;
}
 
int main(int argc, char * argv[]) {
	int rank, i, j;
	int nProcesses;
	MPI_Init(&argc, &argv);
	int pos[N];
	int sorted = 0;
	int *v = (int*)malloc(sizeof(int)*N);
	int *v2 = (int*)malloc(sizeof(int)*N); // not in-place sorting
	int *vQSort = (int*)malloc(sizeof(int)*N);

	for (i = 0; i < N; i++)
		pos[i] = 0;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	printf("Hello from %i/%i\n", rank, nProcesses);

    if (rank == MASTER) {
        // generate random vector
        srandom(42);
        for (i = 0; i < N; i++) {
        	v[i] = random() % 420;
        	v2[i] = -1;
        }

        // send the vector to all processes
	    for (i = 1; i < nProcesses; i++)
	    	MPI_Send(v, N, MPI_INTEGER, i, 0, MPI_COMM_WORLD);
    }

    MPI_Status status;

	if(rank == 0) {
		// DO NOT MODIFY
		displayVector(v);

		// make copy to check it against qsort
		// DO NOT MODIFY
		for(i = 0; i < N; i++)
			vQSort[i] = v[i];
		qsort(vQSort, N, sizeof(int), cmp);

		// sort the vector v
		int size = N / nProcesses;

		for (i = 0; i < size; i++)
			for (j = 0; j < N; j++)
				if (v[size * rank + i] > v[j])
					pos[size * rank + i]++;
		
        // recv the new pozitions
		for (i = 1; i < nProcesses; i++)
			for (j = 0; j < size; j++)
				MPI_Recv(&pos[size * i + j], 1, MPI_INTEGER, i, 0,
					MPI_COMM_WORLD, &status);

		// sort
		for (i = 0; i < N; i++) {
			int larger_than_pos = pos[i];

			while (v2[larger_than_pos] == v[i])
				larger_than_pos++;

			v2[larger_than_pos] = v[i];
		}

		v = v2;

		displayVector(v);
		compareVectors(v, vQSort);
	} else {
		
        // compute the positions
        // send the new positions to process MASTER
        int size = N / nProcesses;

        MPI_Recv(v, N, MPI_INTEGER, 0, 0, MPI_COMM_WORLD, &status);

        for (i = 0; i < size; i++) {
        	for (j = 0; j < N; j++)
        		if (v[size * rank + i] > v[j])
        			pos[size * rank + i]++;

        	MPI_Send(&pos[size * rank + i], 1, MPI_INTEGER, 0, 0, MPI_COMM_WORLD);
        }
	}

	MPI_Finalize();
	return 0;
}
