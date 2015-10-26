/*
 * Anthony Dario - ard74
 * Homework # 4
 * 10/26/2015
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// a semaphore key
#define SEMAPHORE_KEY 0xFA2B

// semaphores positions in the array
#define SEMAPHORE_MUTEX			0
#define SEMAPHORE_WAITING		1
#define NUMBER_OF_SEMAPHORES	2

// required by semctl(2)
union semun {
	int				val;	 /* Value for SETVAL */
	struct semid_ds	*buf;	 /* Buffer for IPC_STAT, IPC_SET */
	unsigned short	*array;  /* Array for GETALL, SETALL */
	struct seminfo	*__buf;  /* Buffer for IPC_INFO
							    (linux-specific) */
};

// variables that will be shared across processes
struct shared_variable_struct {
	int waiting_customers;
	int balance;
};

// gets the semaphore key 
int get_semid(key_t semkey) {
	int id = semget(semkey, NUMBER_OF_SEMAPHORES, 0777 | IPC_CREAT);
	if (id == -1) {
		perror("semget failed");
		exit(EXIT_FAILURE);
	}
	return id;
}

// gets the shared memory id
int get_shmid(key_t shmkey) {
	int id = shmget(shmkey,
					sizeof(struct shared_variables),
					0777 | IPC_CREAT
				    );

	if (id == -1) {
		perror("shmget failed");
		exit(EXIT_ FAILURE);
	}
	return id;
}

// performs a wait on the specified semaphore
void semaphore_wait(int semid, int semnumber) {
		struct sembuf wait_buffer;
		wait_buffer.sem_num = semnumber;
		wait_buffer.semop = -1;
		wait_buffer.semflg = 0;
		if (semop(semid, &wait_buffer, 1) == -1){
			perror("semaphore_wait failed");
			exit(EXIT_FAILURE);
		}
}

// performs a wait on the specified semaphore
void semaphore_signal(int semid, int semnumber) {
		struct sembuf signal_buffer;
		signal_buffer.sem_num = semnumber;
		signal_buffer.semop = 1;
		signal_buffer.semflg = 0;
		if (semop(semid, &signal_buffer, 1) == -1){
			perror("semaphore_signal failed");
			exit(EXIT_FAILURE);
		}
}

// the depositing customer
void depositor(void) {

	printf("I, %d am going to deposit\n", getpid());
	int semid = get_semid((key_t)SEMAPHORE_KEY);
	int shmid = get_shmid((key_t)SEMAPHORE_KEY);
	struct shared_variable_struct * shared_variables = shmat(shmid, 0, 0);

	printf("depositor waiting for mutex\n");
	semaphore_wait(semid, SEMAPHORE_MUTEX);
	printf("depositor done with mutex\n");
}

// the withdrawing customer
void withdrawer(void) {

	printf("I, %d am going to withdraw\n", getpid());
	int semid = get_semid((key_t)SEMAPHORE_KEY);
	int shmid = get_shmid((key_t)SEMAPHORE_KEY);
	struct shared_variable_struct * shared_variables = shmat(shmid, 0, 0);

	printf("withdrawer waiting for mutex\n");
	semaphore_wait(semid, SEMAPHORE_MUTEX);
	printf("withdrawer done with mutex\n");

}

int main(int argc, char *argv[]) {

	printf("main process: %d\n", getpid());

	// create the semaphores
	union semun semaphore_values;

	unsigned short semaphore_init_values[NUMBER_OF_SEMAPHORES];
	semaphore_init_values[SEMAPHORE_MUTEX]   = 1;
	semaphore_init_values[SEMAPHORE_WAITING] = 0;
	semaphore_values.array = semaphore_init_values;

	int semid = get_semid((ket_t)SEMAPHORE_KEY);
	if (semctl(semid, SEMAPHORE_MUTEX, SETALL, semaphore_values) == -1) {
			perror("semctl failed");
			exit(EXIT_FAILURE);
	}

	// set up the shared memory
	int shmid = get_shmid((key_t)SEMAPHORE_KEY);
	struct shared_variable_struct * shared_variables = shmat(shmid, 0, 0);
}
