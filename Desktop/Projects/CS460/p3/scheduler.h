//define thread state
typedef enum {
	RUNNING,
	READY,
	BLOCKED,
	DONE
} state_t;

//define thread table entry struct
struct thread_t
{
	void* stack_pointer;	
	void (*initial_function) (void*);
	void* initial_arg;
	state_t state;
};

void scheduler_begin();
void scheduler_end();
void thread_fork(void(*target)(void*), void * arg);
void thread_finish();
void yield();



