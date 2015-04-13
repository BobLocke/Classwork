/* 	Ryan "Bob" Dean
	CS 360 Assignment 6 - Philo
	Something something philosophers
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include "random.c"

				//currentEat = randomGaussian(9, 3);
				//currentThink = randomGaussian(11, 7);

//Bits of code borrowed from http://www.hep.wisc.edu/~pinghc/SemaphoreNotes.htm

union semun {
     int val;                         // value  for SETVAL
     struct semid_ds *buf;            // buffer for IPC_STAT, IPC_SET
     unsigned short int *array;       // array  for GETALL, SETALL
     struct seminfo *__buf;           // buffer for IPC_INFO
};

int eat(int philoID){
	int currentEat = randomGaussian(9, 3);
	if (currentEat< 0) currentEat = 0;
	printf("(eat) Philo %d is eating for %d seconds.\n", philoID, currentEat);
	fflush(stdout);
	sleep(currentEat);
	return currentEat;
}

void think(int philoID){
	int currentThink = randomGaussian(11, 7);
	if (currentThink < 0) currentThink = 0;
	printf("(think) Philo %d is thinking for %d seconds.\n", philoID, currentThink);
	fflush(stdout);
	sleep(currentThink);
	return;
}

void debug(int semSetID){
	//union semun arg;
	//unsigned short int sem_array[5];
	//arg.array = sem_array;
	//semctl(semSetID, 0, GETALL, arg.array);
	for (int i = 0; i < 5; ++i) {
    	int sem_value = semctl(semSetID, i, GETVAL, 0);
    	printf("?%d?\n", sem_value);
    }
	//printf("[%d, %d, %d, %d, %d]\n", arg.array[0], arg.array[1], arg.array[2], arg.array[3], arg.array[4]);
	return;
}







int main(int argc, char** argv){

	int i;
	int philo[5];
	int totalEat = 0;
	//int currentEat = 0;
	//int currenttThink = 0;

	int semSetID = semget(IPC_PRIVATE, 5, (IPC_CREAT|0666));
	union semun arg;
	unsigned short int sem_array[5] = {1, 1, 1, 1, 1};
	arg.array = sem_array;
	semctl(semSetID, 0, SETALL, arg.array);
	//debug(semSetID);

	for(i = 0; i < 5; i++){	
		if ((philo[i] = fork())){
		} else {
			srand(i);
			//printf("Philo %d created, setting sem ops", i);
			struct sembuf acquire[2] = {{i, -1, 0}, {(i+1) % 5, -1, 0}}, release[2] = {{i, 1, 0}, {(i+1) % 5, 1, 0}};
			while(totalEat < 100){
				//printf("Philo %d created and waiting for chopsticks\n", i);
				fflush(stdout);
				semop(semSetID, acquire, 2);
				//printf("Philo %d has chopsticks, running eat function\n", i);
				fflush(stdout);
				totalEat += eat(i);
				//printf("Philo %d has finished eating, relasing chopsticks\n", i);
				fflush(stdout);
				semop(semSetID, release, 2);
				//printf("Philo %d is thinking\n", i);
				fflush(stdout);
				think(i);	
				//printf("Philo %d is done thinking\n", i);
				fflush(stdout);
			}
			printf("Philo %d has finished eating and has left the table.\n", i);
			exit(0);
		}

	}
	for (int j = 0; j < i; j++) {
		wait(&philo[i]);
	}
	printf("All philosophers have finished eating.\n");
	for (int i = 0; i < 5; i++){
		semctl(semSetID, i, IPC_RMID);
	}
	return 0;
}