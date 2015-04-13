#void thread_start(thread_t * old, thread_t * new);

.globl thread_start

thread_start:
	#push all callee-save registers onto the current stack
	pushq %rbx
	pushq %rbp
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	
	#save the current stack pointer (%rsp) in old's thread table entry 
	movq %rsp, (%rdi)
	 
	#load the stack pointer from new's thread table entry into %rsp
	movq (%rsi), %rsp
	
	#put the address of thread_finish on the bottom of the new thread's stack 
	pushq $thread_finish
	
	#pass our initial argument to the target function
	movq 16(%rsi), %rdi
	
	#jump to initial function of new	
	jmp *8(%rsi)

