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
#include "linkedlist.h"

// a semaphore key
#define SEMAPHORE_KEY 0xFA2B

// semaphores positions in the semaphore array
#define SEMAPHORE_MUTEX			0
#define SEMAPHORE_WAITING		1
#define NUMBER_OF_SEMAPHORES	2

// constants to identify a withdrawer or depositor
#define WITHDRAWER 0
#define DEPOSITOR  1

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
					sizeof(struct shared_variable_struct),
					0777 | IPC_CREAT
				    );

	if (id == -1) {
		perror("shmget failed");
		exit(EXIT_FAILURE);
	}
	return id;
}

// performs a wait on the specified semaphore
void semaphore_wait(int semid, int semnumber) {
		struct sembuf wait_buffer;
		wait_buffer.sem_num = semnumber;
		wait_buffer.sem_op = -1;
		wait_buffer.sem_flg = 0;
		if (semop(semid, &wait_buffer, 1) == -1){
			perror("semaphore_wait failed");
			exit(EXIT_FAILURE);
		}
}

// performs a wait on the specified semaphore
void semaphore_signal(int semid, int semnumber) {
		struct sembuf signal_buffer;
		signal_buffer.sem_num = semnumber;
		signal_buffer.sem_op = 1;
		signal_buffer.sem_flg = 0;
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

	printf("depositor: %d waiting for mutex\n", getpid());
	semaphore_wait(semid, SEMAPHORE_MUTEX);
	printf("depositor: %d done with mutex\n", getpid());
	semaphore_signal(semid, SEMAPHORE_MUTEX);

	exit(EXIT_SUCCESS);
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
	semaphore_signal(semid, SEMAPHORE_MUTEX);

	exit(EXIT_SUCCESS);
}

// fork a withdrawer(0) or depositor(1)
void bank_fork(int customer_type) {

	pid_t child_pid = fork();

	if (child_pid == -1) {
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}
	else if (child_pid == 0) {
		// the child runs whichever process it is
		if (customer_type == WITHDRAWER) {
			withdrawer();
		} else if (customer_type == DEPOSITOR) {
			depositor();
		} else {
			printf("Invalid customer_type");
			exit(EXIT_FAILURE);
		}
	} else {
		// The parent process does nothing
		return;
	}

}

int main(int argc, char *argv[]) {

	printf("main process: %d\n", getpid());

	if (argc != 2) {
		printf("need only two arguments\n");
		exit(EXIT_FAILURE);
	}

	// create the semaphores
	union semun semaphore_values;

	unsigned short semaphore_init_values[NUMBER_OF_SEMAPHORES];
	semaphore_init_values[SEMAPHORE_MUTEX]   = 1;
	semaphore_init_values[SEMAPHORE_WAITING] = 0;
	semaphore_values.array = semaphore_init_values;

	int semid = get_semid((key_t)SEMAPHORE_KEY);
	if (semctl(semid, SEMAPHORE_MUTEX, SETALL, semaphore_values) == -1) {
			perror("semctl failed");
			exit(EXIT_FAILURE);
	}

	// set up the shared memory
	int shmid = get_shmid((key_t)SEMAPHORE_KEY);
	struct shared_variable_struct * shared_variables = shmat(shmid, 0, 0);

	shared_variables->waiting_customers = 0;
	shared_variables->balance = 500;

	// run the given processes
	int i = 0;
	while (argv[1][i] != 0) {
		switch (argv[1][i]) {
				case 'w':
				case 'W':
					bank_fork(WITHDRAWER);
					break;

				case 'd':
				case 'D':
					bank_fork(DEPOSITOR);
					break;

				default:
					printf("invalid argument\n");
					exit(EXIT_FAILURE);
					break;

		}
		i++;
	}

	// wait for processes now
	int j = 0;
	while (j < i) {
		wait(NULL);
		printf("parent waited for child %d of %d\n", j, i - 1);
		j++;
	}

	// clean up all semaphores

	if (shmdt(shared_variables) == -1) {
		perror("shmdt failed");
		exit(EXIT_FAILURE);
	}

	if (shmctl(shmid, IPC_RMID, NULL) < 0) {
		perror("shmctrl failed");
		exit(EXIT_FAILURE);
	}

	if (semctl(semid,
			   SEMAPHORE_MUTEX,
			   IPC_RMID,
			   semaphore_values
			  ) == -1) {
		perror("semctl failed");
		exit(EXIT_FAILURE);
	}


	printf("\nparent is done\n");
	exit(EXIT_SUCCESS);

}
