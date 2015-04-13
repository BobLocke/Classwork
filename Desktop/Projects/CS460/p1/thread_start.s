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
	#%rsp is current stack pointer
	#%rdi is old
	movq %rsp, (%rdi)
	 
	#load the stack pointer from new's thread table entry into %rsp
	#%rsi is new
	movq (%rsi), %rsp
	
	#jump to initial function of new
	jmp *8(%rsi)
