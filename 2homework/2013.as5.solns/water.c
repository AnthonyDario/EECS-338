/*
 * Andrew Hennessy - ajb200
 * Homework #5
 * 22 March 2013
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// This serves as both the key for the semaphores and the shared memory
// (the same key fits two different locks)
#define SEMAPHORE_KEY        0xFA2B

// The position of the various semaphores that we are using in the 
// "semaphore array" that semget gets
#define SEMAPHORE_MUTEX      0
#define SEMAPHORE_OXYGEN     1
#define SEMAPHORE_HYDROGEN   2
#define SEMAPHORE_BARRIER    3
#define NUMBER_OF_SEMAPHORES 4

// This is for our fork function - do we want to fork a Hydrogen
// or an Oxygen?
#define HYDROGEN 1
#define OXYGEN 	 16

void atomic_fork(int atomic_number);
void oxygen();
void hydrogen();

void semaphore_wait(int semid, int semnumber);
void semaphore_signal(int semid, int semnumber);
int create_semaphore(int value);
int get_semid(key_t semkey);
int get_shmid(key_t shmkey);

// This union is required by semctl(2)
// http://linux.die.net/man/2/semctl
union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
};

// Shared memory struct to store HCount, OCount, BCount
struct shared_variable_struct {
	int HCount;
	int OCount;
	int BCount;
};

// Main
int main(int argc, char *argv[]) {
	// For reference, give the user our PID
	printf("*** Hello World! I am %d.\n", getpid());

	// The way you use this program is to give it a command line argument of the form <oO><hH>
	if (argc != 2) {
		printf("!!! PID: %d: Please run this program with one argument, the order of Hydrogen and Oxygen Processes you wish to create.\n", getpid());
		exit(EXIT_FAILURE);
	}

	// Tell the user that their argument has been accepted
 	printf("*** PID: %d: I have been called with the following argument: `%s`.\n", getpid(), argv[1]);

	// Let's try to create some semaphores
	union semun semaphore_values;

	// Set up our semaphore values according to Tekin's solutions
	unsigned short semaphore_init_values[NUMBER_OF_SEMAPHORES];
	semaphore_init_values[SEMAPHORE_MUTEX] = 1;
	semaphore_init_values[SEMAPHORE_OXYGEN] = 0;
	semaphore_init_values[SEMAPHORE_HYDROGEN] = 0;
	semaphore_init_values[SEMAPHORE_BARRIER] = 1;
	semaphore_values.array = semaphore_init_values;

	// Actually make the semaphores now
	int semid = get_semid((key_t)SEMAPHORE_KEY);
	if (semctl(semid, SEMAPHORE_MUTEX, SETALL, semaphore_values) == -1) {
		perror("semctl failed");
		exit(EXIT_FAILURE);
	}

	// Shared Memory
	int shmid = get_shmid((key_t)SEMAPHORE_KEY);
	struct shared_variable_struct * shared_variables = shmat(shmid, 0, 0);
	
	shared_variables->HCount = 0;
	shared_variables->OCount = 0;
	shared_variables->BCount = 0;	

	// Now that we have all of the IPC-related stuff, let's make some Water!
	int i = 0;
	while(argv[1][i] != 0) {
		switch (argv[1][i]) {
			case 'h':
			case 'H':
				printf("!!! PID: %d: I decoded a command to create a Hydrogen Atom!\n", getpid());
				atomic_fork(HYDROGEN);
				break;
	
			case 'o':
			case 'O':
				printf("!!! PID: %d: I decoded a command to create an Oxygen Atom!\n", getpid());
				atomic_fork(OXYGEN);
				break;
			
			default:
				printf("!!! PID: %d: Invalid argument: `%c`! The only valid arguments are `o`, `O`, `h`, or `H`.\n", getpid(), argv[1][i]);
				exit(EXIT_FAILURE);
				break;
			
		}
		i++;
			
	}
	
	// Now that we have made our atoms (and violated the second law of thermodynamics in the 
	// process), let's wait for the atoms to merge together to form water.
	int j;
	for (j = 0; j < i; j++) {
		wait(NULL);
	}

	// And when they are done,
	printf("That's all folks!\n");

	// We need to clean up after ourselves
	if (shmdt(shared_variables) == -1) {
		perror("shmdt failed");
		exit(EXIT_FAILURE);
	}

	if (shmctl(shmid, IPC_RMID, NULL) < 0) {
		perror("shmctrl failed");
		exit(EXIT_FAILURE);
	}

	if (semctl(semid, SEMAPHORE_MUTEX, IPC_RMID, semaphore_values) == -1) {
		perror("semctl failed");
		exit(EXIT_FAILURE);
	}
}

// Fork and go into Hydrogen() or Oxygen()
void atomic_fork(int atomic_number) {
	pid_t child_pid;
	child_pid = fork();
	if (child_pid == -1) {
		perror("Fork Failed");
		exit(EXIT_FAILURE);
	} else if (child_pid == 0) {
		// Child
		if (atomic_number == OXYGEN) {
			oxygen();
		} else if (atomic_number == HYDROGEN) {
			hydrogen();
		} else {
			printf("!!! Invalid Atomic Number!\n"); 
			exit(EXIT_FAILURE);
		}
	} else {
		// Parent
		return;
	}
}

void oxygen(void) {
	printf("*** PID: %d: I am an Oxygen!\n", getpid());

	int semid = get_semid((key_t)SEMAPHORE_KEY);
	int shmid = get_shmid((key_t)SEMAPHORE_KEY);
	struct shared_variable_struct * shared_variables = shmat(shmid, 0, 0);

	printf("--- PID: %d: O: Waiting on Mutex.\n", getpid());
	semaphore_wait(semid, SEMAPHORE_MUTEX);
	printf("--- PID: %d: O: Passed Mutex.\n", getpid());
	
	if (shared_variables->HCount >= 2) {
		printf("--- PID: %d: O: Setting BCount = 2.\n", getpid());
		shared_variables->BCount = 2;
		printf("--- PID: %d: O: Signaling Hydrogen.\n", getpid());
		semaphore_signal(semid, SEMAPHORE_HYDROGEN);
		printf("--- PID: %d: O: Signaling Hydrogen.\n", getpid());
		semaphore_signal(semid, SEMAPHORE_HYDROGEN);	
	} else {
		printf("--- PID: %d: O: OCount++.\n", getpid());
		shared_variables->OCount++;
		printf("--- PID: %d: O: Signaling Mutex.\n", getpid());
		semaphore_signal(semid, SEMAPHORE_MUTEX);
		printf("--- PID: %d: O: Waiting on Oxygen.\n", getpid());
		semaphore_wait(semid, SEMAPHORE_OXYGEN);
		printf("--- PID: %d: O: Passed Oxygen.\n", getpid());
		printf("--- PID: %d: O: Waiting on Barrier.\n", getpid());
		semaphore_wait(semid, SEMAPHORE_BARRIER);
		printf("--- PID: %d: O: Passed Barrier.\n", getpid());
		printf("--- PID: %d: O: Decerementing OCount and BCount.\n", getpid());
		shared_variables->OCount--;
		shared_variables->BCount--;
		if (shared_variables->BCount != 0) {
			printf("--- PID: %d: O: Signaling Barrier. (BCount != 0).\n", getpid());
			semaphore_signal(semid, SEMAPHORE_BARRIER);
		} else {
			printf("--- PID: %d: O: Signaling Barrier. (BCount == 0).\n", getpid());
			semaphore_signal(semid, SEMAPHORE_BARRIER);
			printf("--- PID: %d: O: Signaling Mutex.\n", getpid());
			semaphore_signal(semid, SEMAPHORE_MUTEX);
		}
	}

	printf("@@@ PID: %d: O: I am part of a Water Molecule.\n", getpid());

	if (shmdt(shared_variables) == -1) {
		perror("shmdt failed");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}

void hydrogen(void) {
	printf("*** PID: %d: I am a Hydrogen!\n", getpid());

	int semid = get_semid((key_t)SEMAPHORE_KEY);
	int shmid = get_shmid((key_t)SEMAPHORE_KEY);
	struct shared_variable_struct * shared_variables = shmat(shmid, 0, 0);

	printf("=== PID: %d: H: Waiting on Mutex.\n", getpid());
	semaphore_wait(semid, SEMAPHORE_MUTEX);
	printf("=== PID: %d: H: Passed Mutex.\n", getpid());

	if (shared_variables->OCount >= 1 && shared_variables->HCount >= 1) {
		printf("=== PID: %d: H: Setting BCount = 2.\n", getpid());
		shared_variables->BCount = 2;
		printf("=== PID: %d: H: Signaling Oxygen.\n", getpid());
		semaphore_signal(semid, SEMAPHORE_OXYGEN);
		printf("=== PID: %d: H: Signaling Hydrogen.\n", getpid());
		semaphore_signal(semid, SEMAPHORE_HYDROGEN);
	} else {
		printf("=== PID: %d: H: Incrementing HCount.\n", getpid());
		shared_variables->HCount++;
		printf("=== PID: %d: H: Signaling Mutex.\n", getpid());
		semaphore_signal(semid, SEMAPHORE_MUTEX);
		printf("=== PID: %d: H: Waiting on Hydrogen.\n", getpid());
		semaphore_wait(semid, SEMAPHORE_HYDROGEN);
		printf("=== PID: %d: H: Passed Hydrogen.\n", getpid());
		printf("=== PID: %d: H: Waiting on Barrier.\n", getpid());
		semaphore_wait(semid, SEMAPHORE_BARRIER);
		printf("=== PID: %d: H: Passed Barrier.\n", getpid());
		printf("=== PID: %d: H: Decrementing HCount and BCount.\n", getpid());
		shared_variables->HCount--;
		shared_variables->BCount--;
		if (shared_variables->BCount != 0) {
			printf("=== PID: %d: H: Signaling Barrier.\n", getpid());
			semaphore_signal(semid, SEMAPHORE_BARRIER);
		} else {
			printf("=== PID: %d: H: Signaling Barrier.\n", getpid());
			semaphore_signal(semid, SEMAPHORE_BARRIER);
			printf("=== PID: %d: H: Signaling Mutex.\n", getpid());
			semaphore_signal(semid, SEMAPHORE_MUTEX);
		}
	}

	printf("@@@ PID: %d: H: I am part of a Water Molecule.\n", getpid());

	if (shmdt(shared_variables) == -1) {
		perror("shmdt failed");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}

// These two functions are wrapper functions for the System-V
// style semaphores that were talked about in class. 
// Modified from: http://art.cwru.edu/338.S13/example.semaphore.html
void semaphore_wait(int semid, int semnumber) {
	struct sembuf wait_buffer;
	wait_buffer.sem_num = semnumber;
	wait_buffer.sem_op = -1;
	wait_buffer.sem_flg = 0;
	if (semop(semid, &wait_buffer, 1) == -1)  {
		perror("semaphore_wait failed");
		exit(EXIT_FAILURE);
	}
}

void semaphore_signal(int semid, int semnumber) {
	struct sembuf signal_buffer;
	signal_buffer.sem_num = semnumber;
	signal_buffer.sem_op = 1;
	signal_buffer.sem_flg = 0;
	if (semop(semid, &signal_buffer, 1) == -1)  {
		perror("semaphore_signal failed");
		exit(EXIT_FAILURE);
	}
}

// Small wrapper functions to convert the keys of the shared memory
// and the semaphores to values.
int get_semid(key_t semkey) {
	int value = semget(semkey, NUMBER_OF_SEMAPHORES, 0777 | IPC_CREAT);
	if (value == -1) {
		perror("semget failed");
		exit(EXIT_FAILURE);
	}
	return value;
}

int get_shmid(key_t shmkey) {
	int value = shmget(shmkey, sizeof(struct shared_variable_struct), 0777 | IPC_CREAT);
	if (value == -1) {
		perror("shmkey failed");
		exit(EXIT_FAILURE);
	}
	return value;
}
