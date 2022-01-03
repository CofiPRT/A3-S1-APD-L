#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/*
	schelet pentru exercitiul 5
*/

#define NUM_THREADS 2

int min(int a, int b) {
	return a <= b ? a : b;
}

struct index_pair {
	int start;
	int end;
};

int* arr;
int array_size;

void *f(void *arg) {
	struct index_pair args = *(struct index_pair*) arg;

	for (int i = args.start; i < args.end; i++) {
		arr[i] += 100;
	}
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		perror("Specificati dimensiunea array-ului\n");
		exit(-1);
	}

	array_size = atoi(argv[1]);

	arr = malloc(array_size * sizeof(int));
	for (int i = 0; i < array_size; i++) {
		arr[i] = i;
	}

	for (int i = 0; i < array_size; i++) {
		printf("%d", arr[i]);
		if (i != array_size - 1) {
			printf(" ");
		} else {
			printf("\n");
		}
	}

	// TODO: aceasta operatie va fi paralelizata
	// for (int i = 0; i < array_size; i++) {
	// 	arr[i] += 100;
	// }

	pthread_t threads[NUM_THREADS];
	struct index_pair indexes[NUM_THREADS];
	void *status;

	for (int id = 0; id < NUM_THREADS; id++) {
		indexes[id].start = id * (double) array_size / NUM_THREADS;
		indexes[id].end = min((id + 1) * (double) array_size / NUM_THREADS, array_size);

		if (pthread_create(&threads[id], NULL, f, &indexes[id])) {
			printf("[ERROR] Could not create thread '%d'\n", id);
			exit(-1);
		}
	}

	for (int id = 0; id < NUM_THREADS; id++) {
		if (pthread_join(threads[id], &status)) {
			printf("[ERROR] Could not join thread '%d'\n", id);
			exit(-1);
		}
	}

	for (int i = 0; i < array_size; i++) {
		printf("%d", arr[i]);
		if (i != array_size - 1) {
			printf(" ");
		} else {
			printf("\n");
		}
	}

	pthread_exit(NULL);
}
