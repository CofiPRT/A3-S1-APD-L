#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int N;
int P;
int *v;
int *vQSort;

// pentru asteptarea fazelor (impare si pare)
pthread_barrier_t barrier;

void compare_vectors(int *a, int *b) {
	int i;

	for (i = 0; i < N; i++) {
		if (a[i] != b[i]) {
			printf("Sortare incorecta\n");
			return;
		}
	}

	printf("Sortare corecta\n");
}

void display_vector(int *v) {
	int i;
	int display_width = 2 + log10(N);

	for (i = 0; i < N; i++) {
		printf("%*i", display_width, v[i]);
	}

	printf("\n");
}

int cmp(const void *a, const void *b) {
	int A = *(int*)a;
	int B = *(int*)b;
	return A - B;
}

void get_args(int argc, char **argv)
{
	if(argc < 3) {
		printf("Numar insuficient de parametri: ./oets N P\n");
		exit(1);
	}

	N = atoi(argv[1]);
	P = atoi(argv[2]);
}

void init()
{
	int i;
	v = malloc(sizeof(int) * N);
	vQSort = malloc(sizeof(int) * N);

	if (v == NULL || vQSort == NULL) {
		printf("Eroare la malloc!");
		exit(1);
	}

	srand(42);

	for (i = 0; i < N; i++)
		v[i] = rand() % N;

	pthread_barrier_init(&barrier, NULL, P);
}

void term()
{
	free(v);
	free(vQSort);

	pthread_barrier_destroy(&barrier);
}

void print()
{
	printf("v:\n");
	display_vector(v);
	printf("vQSort:\n");
	display_vector(vQSort);
	compare_vectors(v, vQSort);
}

void swap(int *a, int *b) {
	*a = *a + *b;
	*b = *a - *b;
	*a = *a - *b;
}

void *thread_function(void *arg)
{
	int thread_id = *(int *)arg;

	int start_index = thread_id * N / P;
	int start_index_par, start_index_impar;

	if (start_index % 2) {
		start_index_par = start_index + 1;
		start_index_impar = start_index;
	} else {
		start_index_par = start_index;
		start_index_impar = start_index + 1;
	}

	int end_index = fmin((thread_id + 1) * N / P, N - 1);

	for (int k = 0; k < N; k++) {

		// faza para
		for (int i = start_index_par; i < end_index; i += 2)
			if (v[i] > v[i + 1])
				swap(&v[i], &v[i + 1]);

		// asteptam ca fiecare thread sa termine faza para
		pthread_barrier_wait(&barrier);

		// faza impara
		for (int i = start_index_impar; i < end_index; i += 2)
			if (v[i] > v[i + 1])
				swap(&v[i], &v[i + 1]);

		// asteptam ca fiecare thread sa termine faza impara
		pthread_barrier_wait(&barrier);

	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	get_args(argc, argv);
	init();

	int i;
	pthread_t tid[P];
	int thread_id[P];

	// se sorteaza vectorul etalon
	for (i = 0; i < N; i++)
		vQSort[i] = v[i];
	qsort(vQSort, N, sizeof(int), cmp);

	// se creeaza thread-urile
	for (i = 0; i < P; i++) {
		thread_id[i] = i;
		pthread_create(&tid[i], NULL, thread_function, &thread_id[i]);
	}

	// se asteapta thread-urile
	for (i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}

	// se afiseaza vectorul etalon
	// se afiseaza vectorul curent
	// se compara cele doua
	print();

	term();

	return 0;
}
