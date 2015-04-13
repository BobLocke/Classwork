	/* 	Ryan "Bob" Dean
	CS 360 Assignment 6 - Philo
	Something something philosophers
*/

	#include <stdio.h>
	#include <stdlib.h>
	#include <pthread.h>
	#include <errno.h>
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/wait.h>
	#include "random_r.h"

  /* Global (and shared) integers */
	pthread_mutex_t chopstick[5];
	unsigned int philoNum[5] = {0, 1, 2, 3, 4};
	unsigned int seed[5] = {1, 2, 3, 4, 5};

int eat(unsigned int myID){
	int currentEat = randomGaussian_r(9, 3, &seed[myID]);
	if (currentEat< 0) currentEat = 0;
	printf("(eat) Philo %d is eating for %d seconds.\n", myID, currentEat);
	fflush(stdout);
	sleep(currentEat);
	return currentEat;
}

void think(unsigned int myID){
	int currentThink = randomGaussian_r(11, 7, &seed[myID]);
	if (currentThink < 0) currentThink = 0;
	printf("(think) Philo %d is thinking for %d seconds.\n", myID, currentThink);
	fflush(stdout);
	sleep(currentThink);
	return;
}

int acquire(unsigned int myID){
	pthread_mutex_lock(&chopstick[myID]);
	if (pthread_mutex_trylock(&chopstick[(myID + 1) % 5]) == EBUSY){
		pthread_mutex_unlock(&chopstick[myID]);
		return 1;
	} else return 0;
}

void* philoT(void *philoID){
	unsigned int myID = *((unsigned int *)philoID);
	//printf("%i\n", myID);
	int ready = 1;
	int totalEat = 0;
	//printf("%i\n", myID);
	while(totalEat < 20){
		//printf("Philo %d created and waiting for chopsticks\n", myID);
		//fflush(stdout);
		while(ready) {
			ready = acquire(myID);
			sleep(1);
		}
		//printf("Philo %d has chopsticks, running eat function\n", myID);
		//fflush(stdout);
		totalEat += eat(myID);
		printf(" ***** Philo %d has eaten for %d seconds *****\n", myID, totalEat);
		fflush(stdout);
		pthread_mutex_unlock(&chopstick[myID]);
		pthread_mutex_unlock(&chopstick[(myID + 1) % 5]);
		//printf("Philo %d is thinking\n", myID);
		//fflush(stdout);
		think(myID);	
		//printf("Philo %d is done thinking\n", myID);
		fflush(stdout);
	}
	printf("Philo %d has finished eating and has left the table.\n", myID);
	return 0;
}




int main() { 
	pthread_t philo[5];
	for (int i = 0; i < 5; i++){
		pthread_mutex_init(&chopstick[i], NULL);	
	}
	for (int i = 0; i < 5; i++){
		pthread_create(&philo[i], NULL, philoT, &philoNum[i]);
	}
	for (int j = 0; j < 5; j++){
		pthread_join(philo[j], NULL);
	}

  printf("All philosphers have finished eating and left the table.\n");

  return 0;
}