#include <stdio.h>
#include <stdlib.h>

// Try with 128, 256, 512, 1024, 2048 Bytes
#define CURRENT_THREAD_STACK_SIZE 2048

//define thread struct
typedef struct
{
	void* stack_pointer;
	void (*initial_function) (void);
}thread_t;

//declare functions
void thread_start(thread_t * old, thread_t * new);
void thread_switch(thread_t * old, thread_t * new);
void yield();
void performYield();

//create 2 global threads
thread_t current_thread;
thread_t stored_thread;

//define yield function
void yield(){	
	thread_t temp = current_thread;
	current_thread = stored_thread;
	stored_thread = temp;
	thread_switch(&stored_thread, &current_thread);		
}

//define print function
void performYield(){
	//continuously run the following
	while (1){
		printf("%s\n", "current_thread: perform yield");
		yield();
	}
}

//test context switch
int main(void) {
    /* student to complete the logic in main here */
	
	// Malloc a suitable region for current_thread's stack. Experiment with different stack sizes. Remember, stack grows down.
	
	current_thread.stack_pointer = (void*) malloc(CURRENT_THREAD_STACK_SIZE);
	current_thread.stack_pointer += CURRENT_THREAD_STACK_SIZE;
	
	// current_thread is soon to start with performYield function, set this function to be the initial function of current_thread.
	
	current_thread.initial_function = performYield;
	
	// Given the above, stored_thread reprents the main thread, soon to be suspended and stashed.
	//		Q2: Stored_thread's stack does not have to be allocated. Why not?
	// 		ANS: Stored thread's stack does not have to be allocated because it holds a temporary copy of a suspended thread, and does not start off with thread context data.
	
	// call thread_start(&stored_thread, &current_thread)
	thread_start(&stored_thread, &current_thread);
	
	// After thread_start, have main continuously print a message:
	// ("stored_thread:  perform yield") and call yield().
	while(1){
		printf("stored_thread: perform yield\n");
		yield();
	}
	
	// Run program with current_thread's stack size from 128, 256, 512, 1024, 2048 Bytes. What do you expect from the outputs? What are the actual outputs and explain why?
	
    return 0;
}
