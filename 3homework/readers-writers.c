#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// constants
#define NUM_THREADS 10

// semaphores
sem_t mutex, writing;

// variables
int read_count;

// gets a random int in the range
int get_random() {
	return rand() % 2;
}

// wait for a semaphore
void wait_sem(sem_t *sem) {
	if (sem_wait(sem) < 0) {
		perror("sem_wait");
		exit(EXIT_FAILURE);
	}
}

// signal a semaphore
void signal_sem(sem_t *sem) {
	if (sem_post(sem) < 0) {
		perror("sem_post");
		exit(EXIT_FAILURE);
	}
}

// a writer process
void *writer(int * tid) {

	printf("writer: %d is waiting for 'write'\n", *tid);
	wait_sem(&writing);

	printf("writer: %d is signaling 'write'\n", *tid);
	signal_sem(&writing);

	pthread_exit(NULL);
}

// a reader process
void *reader(int *tid) {

	printf("reader: %d is waiting for 'mutex'\n", *tid);
	wait_sem(&mutex);

	read_count++;
	if (read_count == 1) {
		printf("reader: %d is waiting for 'write'\n", *tid);
		wait_sem(&writing);
	}

	signal_sem(&mutex);

	printf("reader: %d is waiting for mutex\n", *tid);
	wait_sem(&mutex);

	read_count--;
	if (read_count == 0) {
		printf("reader: %d is signaling writing\n", *tid);
		signal_sem(&writing);
	}

	printf("reader: %d is signaling mutex\n", *tid);
	signal_sem(&mutex);


	pthread_exit(NULL);

}

int main(int argc, char *argv[]) {

	pthread_t threads[NUM_THREADS];
	int thread_ids[NUM_THREADS];

	int i;
	void *thread_func;
	read_count = 0;

	// seed the random function
	srand(time(NULL));

	// initialize semaphores
	if (sem_init(&mutex, 0, (unsigned int)1) < 0 ||
		sem_init(&writing, 0, (unsigned int)1) < 0) {

		perror("sem_init");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < NUM_THREADS; i++) {

		thread_ids[i]= i;

		// randomly decide if thread is reader or writer
		if (get_random() % 2 == 0) {
			thread_func = reader;
		}
		else {
			thread_func = writer;
		}

		// create the threads
		if (pthread_create(&threads[i], NULL, thread_func, &thread_ids[i])) {
			fprintf(stderr, "pthread_create error\n");
			perror("pthread_create");
			return EXIT_FAILURE;
		}
		sleep(1);
	}

	// join the threads
	for (i = 0; i < NUM_THREADS; i++) {
		if (pthread_join(threads[i], NULL)) {
			printf("join error\n");
			fprintf(stderr, "pthread_join error");
		}
	}
	printf("all threads done\n");

	return EXIT_SUCCESS;

}
