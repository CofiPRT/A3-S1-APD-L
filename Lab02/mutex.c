#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 2

pthread_mutex_t mutex;

int a = 0;

void *f(void *arg)
{
	pthread_mutex_lock(&mutex);
	a += 2;
	pthread_mutex_unlock(&mutex);

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	int i;
	void *status;
	pthread_t threads[NUM_THREADS];
	int arguments[NUM_THREADS];

	if (pthread_mutex_init(&mutex, NULL)) {
		printf("[ERROR] Could not init the mutex\n");
		exit(-1);
	}

	for (i = 0; i < NUM_THREADS; i++) {
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

	if (pthread_mutex_destroy(&mutex)) {
		printf("[ERROR] Could not destroy mutex\n");
		exit(-1);
	}

	printf("a = %d\n", a);

	return 0;
}
