all: async_behavior async_semantics

async_behavior:
	gcc thread_switch.s thread_start.s scheduler.o queue.c async.c main_test_scheduling_behavior.c -lrt -o async_behavior
async_semantics:
	gcc thread_switch.s thread_start.s scheduler.o queue.c async.c main_test_reading_semantics.c -lrt -o async_semantics
snake:
	gcc thread_switch.s thread_start.s scheduler.o queue.c async.c snake.c -lrt -o snake

clean:
	rm async_behavior async_semantics snake
