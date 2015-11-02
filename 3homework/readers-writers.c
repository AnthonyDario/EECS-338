#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// threads
#define NUM_THREADS 200

// semaphores
sem_t mutex, writing;

// variables
int read_count = 0;

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
void writer(int tid) {

	printf("writer: %d is waiting for 'write'\n", tid);
	wait_sem(&writing);

	printf("writer: %d is signaling 'write'\n", tid);
	signal_sem(&writing);

}

// a reader process
void reader(int tid) {

	printf("reader: %d is waiting for 'mutex'\n", tid);
	wait_sem(&mutex);

		read_count++;
		if (read_count == 1) {
			printf("reader: %d is waiting for 'write'\n", tid);
			wait_sem(&writing);
		}

	printf("reader: %d is signaling 'mutex'\n", tid);
	signal_sem(&mutex);

}

int main(int argc, char *argv[]) {


}
