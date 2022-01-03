#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 4

void *f(void *arg) {
	long id = *(long*) arg;

	int i;
	for (i = 0; i < 100; i++)
		printf("Hello World din thread-ul %ld, iteratia %d!\n", id, i + 1);

	pthread_exit(NULL);
}

void load(char *text) {

	int i;
	for (i = 0; i < 1000; i++) {
		float reasonable_number = ((float)i)/10 + ((float)(rand() % 10))/100;
		printf("[IN PROGRESS] %s....................%.2f\n",
				text, reasonable_number);
	}

	printf("%s....................Done\n", text);
}

void *hack_pentagon(void *arg) {
	long port = *(long*) arg;

	load("Parsing port");
	printf("Successfully hacked the pentagon on port %ld\n", port);
}

void *mine_bitcoin(void *arg) {
	long count = *(long*) arg;

	load("Mining bitcoin");
	printf("Successfully mined %ld bitcoins\n", count);
}

int main(int argc, char *argv[]) {

	long cores = sysconf(_SC_NPROCESSORS_CONF);

	printf("This system has %ld cores\n", cores);

	pthread_t threads[cores];
	int r;
	long id;
	void *status;
	long arguments[cores];

	for (id = 0; id < cores; id++) {
		arguments[id] = id;
		r = pthread_create(&threads[id], NULL, f, &arguments[id]);

		if (r) {
			printf("[ERROR] Could not create thread '%ld'\n", id);
			exit(-1);
		}
	}

	for (id = 0; id < cores; id++) {
		r = pthread_join(threads[id], &status);

		if (r) {
			printf("[ERROR] Could not join thread '%ld'\n", id);
			exit(-1);
		}
	}

	// 4
	arguments[0] = 25565;
	if (pthread_create(&threads[0], NULL, hack_pentagon, &arguments[0])) {
		printf("[ERROR] Could not create thread '%ld'\n", id);
		exit(-1);
	}

	arguments[0] = 877060;
	if (pthread_create(&threads[1], NULL, mine_bitcoin, &arguments[1])) {
		printf("[ERROR] Could not create thread '%ld'\n", id);
		exit(-1);
	}

	pthread_exit(NULL);
}
