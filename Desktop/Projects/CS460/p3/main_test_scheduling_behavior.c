#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "scheduler.h"
#include "async.h"

//find the nth prime number for large values of n (naive algorithm)
void find_nth_prime(void * n) 
{
	char * str = (char*) n;
	int i = 0;
	unsigned long j;
	unsigned long p;
	for(p = 2; i < atoi(str); p++) 
	{        
		for(j = 2; j < p; j++)
		{
			if (p % j == 0)
				break;	
			yield();				
		}
		if (j == p)
			i++; 
		yield();       		
	}       
	printf("Prime %sth is %lu\n", str, p-1);	
}

void user_interaction(void *__not_used)
{
	srand(time(NULL));
	int random_number = rand()%(101)+1;
	printf("I have chosen a secret number between 1 and 100.\n");
	printf("What do you think it is?\n");
	char buf[100];
	int bytesread;
	int guess = -1;
	int tries_count = 0;
	while (guess != random_number)
	{
		bytesread = read_wrap(1, buf, 100);
		tries_count++;
		buf[bytesread] = '\0';
		guess = atoi(buf);
		if (guess == 0)
			printf("\n%s is not a valid number!\n", buf);
		else if (guess == random_number)
			printf("\nCorrect!\nThe correct number is %d.\nYou have successfully guessed the correct number in %d tries!\n",
				   random_number, tries_count);
		else if (guess < random_number)
			printf("\nHigher...\n");
		else // guess > random_number
			printf("\nLower...\n");
		if (guess != random_number)
			printf("What do you think it is?\n");
	}
}

int main(void)
{
	scheduler_begin();
	thread_fork(user_interaction, NULL);
	thread_fork(find_nth_prime, (void*)"15000");
	thread_fork(find_nth_prime, (void*)"10000");
	thread_fork(find_nth_prime, (void*)"5000");
	thread_fork(find_nth_prime, (void*)"1000");
	scheduler_end(); 
	printf("%s\n", "main_thread: return");
}