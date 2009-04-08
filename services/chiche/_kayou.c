void *__libc_stack_end = (void*)0;

/*
 * Declare the __environ global variable and create a strong alias environ.
 * Note: Apparently we must initialize __environ to ensure that the strong
 * environ symbol is also included.
 */
char **__environ = 0;

/* __uClibc_main is the new main stub for uClibc. This function is
 * called from crt1 (version 0.9.28 or newer), after ALL shared libraries
 * are initialized, just before we call the application's main function.
 */
void _kayou(int (*main)(int, char **, char **),
	    int argc,
	    char **argv,
	    void *stack_end)
{
    __libc_stack_end = stack_end;

    /* The environment begins right after argv.  */
    __environ = &argv[argc + 1];

    /* If the first thing after argv is the arguments
     * the the environment is empty. */
    if ((char *) __environ == *argv) {
	/* Make __environ point to the NULL at argv[argc] */
	__environ = &argv[argc];
    }

    /*
     * Finally, invoke application's main and then exit.
     */
    main(argc, argv, __environ);

    while (1)
      ;
}
