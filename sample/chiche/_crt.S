.text
.global _start
.type   _start,%function

.type   main,%function
.type   _kayou,%function

_start:
	/* Clear the frame pointer.  The ABI suggests this be done, to mark
	   the outermost frame obviously.  */
	xorl %ebp, %ebp

	/* Extract the arguments as encoded on the stack and set up
	   the arguments for `main': argc, argv.  envp will be determined
	   later in __libc_start_main.  */

	movl $__task_id, %edi
	popl 0(%edi)
	popl 4(%edi)
	movl $__as_id, %edi
	popl 0(%edi)
	popl 4(%edi)

	popl %esi		/* Pop the argument count.  */
	movl %esp, %ecx		/* argv starts just at the current stack top.*/

	/* Before pushing the arguments align the stack to a 16-byte
	(SSE needs 16-byte alignment) boundary to avoid penalties from
	misaligned accesses.  Thanks to Edward Seidl <seidl@janed.com>
	for pointing this out.  */
	andl $0xfffffff0, %esp
	pushl %eax		/* Push garbage because we allocate
				   28 more bytes.  */

	/* Provide the highest stack address to the user code (for stacks
	   which grow downwards).  */
	pushl %esp

	pushl %ecx		/* Push second argument: argv.  */
	pushl %esi		/* Push first argument: argc.  */

	pushl $main

	/* Call the user's main function, and exit with its value.
	   But let the libc call main.    */
	call _kayou

	hlt			/* Crash if somehow `exit' does return.  */
.size _start,.-_start

/* Define a symbol for the first piece of initialized data.  */
.data
.global __task_id
__task_id:
.long 0
.long 0
.global __as_id
__as_id:
.long 0
.long 0
.global __system_id
__system_id:
.long 0
.long 0

.global __data_start
__data_start:
.long 0
.weak data_start
	data_start = __data_start
