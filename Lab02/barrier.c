#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 2

pthread_barrier_t barrier;

void *f(void *arg)
{
	int thread_id = *(int *)arg;

	if (thread_id == 1) {
		printf("1\n");
	}

	pthread_barrier_wait(&barrier);

	if (thread_id == 0) {
		printf("2\n");
	}

	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	int i;
	void *status;
	pthread_t threads[NUM_THREADS];
	int arguments[NUM_THREADS];

	if (pthread_barrier_init(&barrier, NULL, 2)) {
		printf("[ERROR] Could not init barrer\n");
		exit(-1);
	}

	for (i = 0; i < NUM_THREADS; i++) {
		arguments[i] = i;
		if (pthread_create(&threads[i], NULL, f, &arguments[i])) {
			printf("[ERROR] Could not create thread %d\n", i);
			exit(-1);
		}
	}

	for (i = 0; i < NUM_THREADS; i++) {
		if (pthread_join(threads[i], &status)) {
			printf("[ERROR] Could not join thread %d\n", i);
			exit(-1);
		}
	}

	if (pthread_barrier_destroy(&barrier)) {
		printf("[ERROR} Could not destroy barrier\n");
		exit(-1);
	}

	return 0;
}
