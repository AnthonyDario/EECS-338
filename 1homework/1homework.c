#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <unistd.h>
#include <sys/times.h>

#define LOOP_ITERATIONS (99999999)	// maximum integer

// computes the factorial of a number
int factorial(int num) {
	int i;
	int factorial = (num == 0) ? 1 : num;

	for (i = num - 1; i > 0; i--) {
		factorial = factorial * i;
	}

	return factorial;
}

// computes the binomial coefficient of two numbers
int binomial(int n, int k) {

	int coefficient = factorial(n) / (factorial(k) * factorial(n - k));

	return (coefficient);
}

// prints out every other binomial coefficient starting from the process
// number and ending at 10
void printBinomial(int process) {

	int i;

	char* processString;

	if (process == 2) {
		processString = "second";
	}
	else if (process == 3) {
		processString = "third";
	}

	for (i = process; i < 11; i += 2) {

		printf("The %6s process: %d produced the binomial coefficient %2d for integers %2d, %d\n", processString, getpid(), binomial(i, i - 2), i, i - 2);
		sleep(2);
	}
}

// checks for error of the times() system call
void checkTimesError(clock_t result) {

	if (result == (clock_t) -1) {
		perror("times() failed");
		exit(EXIT_FAILURE);
	}
}

// print out user info for the child processes
void printUserInfo(int process) {

	char username[15];

	cuserid(username);

	printf("\nChild %d has a userid of:  %d\n", process, getuid());
	printf("Child %d has a groupid of: %d\n", process, getgid());
	printf("Child %d has an effective userid of:  %d\n", process, geteuid());
	printf("Child %d has an effective groupid of: %d\n", process, getegid());
	printf("the child's user is %s\n", username);
}

// prints the CPU time of the user and the system
void printCPUTime(int process) {

	struct tms process_times;
	double utime;
	double stime;

	checkTimesError(times(&process_times));

	utime = (double) process_times.tms_utime / CLOCKS_PER_SEC;
	stime = (double) process_times.tms_stime / CLOCKS_PER_SEC;

	printf("child %d had a user time of:  %f seconds\n", process, utime);
	printf("and it had a system time of: %f seconds\n\n", stime);

}

// synchronizes children and prints their user info
void synchronize(int process) {

	int i;

	sleep(process - 1);
	printUserInfo(process);

	// loop to accumulate non-zero CPU times
	for (i = 0; i < LOOP_ITERATIONS; i++) {

	}

	sleep(4);
}

int main(int argc, char* argv[]) {

	int child_pid[4];
	int status[4];
	int i;
	int pid;

	time_t starts[5];
	time_t ends[5];
	double time_diff[4];
	clock_t result;

	char parent_userid[15];

	checkTimesError(time(&starts[4]));

	// fork the process
	for (i = 0; i < 4; i++) {

		pid = fork();
		checkTimesError(time(&starts[i]));
		if (pid == 0) {

			switch(i) {

				case 0 :
					synchronize(i + 1);
					printf("\n(n(n-2)) binomial coefficient computations of integers n = 2 ... 10, start now!\n\n");
					sleep(10);
					printCPUTime(i + 1);
					exit(EXIT_SUCCESS);
					break;

				case 1 :
					synchronize(i + 1);
					printBinomial(2);
					printCPUTime(i + 1);
					exit(EXIT_SUCCESS);
					break;

				case 2 :
					synchronize(i + 1);
					printBinomial(3);
					sleep(2);
					printCPUTime(i + 1);
					exit(EXIT_SUCCESS);
					break;

				case 3 :
					synchronize(i + 1);
					sleep(10);
					printf("\nthe fourth process is now executing ls -l\n\n");
					printCPUTime(i + 1);
					execl("/bin/ls", "ls", "-l", NULL);
					exit(EXIT_SUCCESS);
					break;
			}
		}

		else if (pid < 0) {
			perror("fork has failed");
			exit(EXIT_FAILURE);
		}
	}

	// wait for the child processes to finish
	for (i = 0; i < 4; i++) {

		child_pid[i] = wait(&status[i]);

		checkTimesError(time(&ends[i]));
		time_diff[i] = difftime(ends[i], starts[i]);
	}

	printf("\n");

	// print out info for the child processes
	for (i = 0; i < 4; i++) {
		printf("Child: %d had process id: %d and finished with termination status: %d\n", i, child_pid[i], status[i]);
		printf("\tit took %f seconds to execute\n", time_diff[i]);
		printf("\tit ended %s\n", ctime(&ends[i]));
	}

	// print out info about the parent process
	checkTimesError(time(&ends[4]));
	time_diff[4] = difftime(ends[4], starts[4]);
	cuserid(parent_userid);

	printf("the parent process had a run time of %f\n", time_diff[4]);
	printf("\tit had an end time of %s", ctime(&ends[4]));
	printf("\tthe user is %s\n", parent_userid); 
	printf("\tits real user is id: %d\n", getuid());
	printf("\tits real groupid is: %d\n", getgid());
	printf("\tits effective user id: is  %d\n", geteuid());
	printf("\tits effective group id: is %d\n", getegid());
	printf("\n");

	exit(EXIT_SUCCESS);
}

