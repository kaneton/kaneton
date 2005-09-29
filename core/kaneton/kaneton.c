/*
 * copyright quintard julien
 *
 * kaneton
 *
 * kaneton.c
 *
 * path          /home/mycure/kaneton/core/kaneton
 *
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 *
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Wed Sep 28 19:10:32 2005   mycure
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * this variable is generated when the kernel is compiled, indicating
 * the current kernel version.
 */

extern const char	version[];

/*
 * init variable received from the bootloader specifying segments, regions,
 * physical memory layout etc..
 */

t_init*			init;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function simply initialises each manager.
 *
 * steps:
 *
 * 1) sets the init variable from the bootloader argument.
 * 2) initialises the console manager.
 * 3) displays the current kaneton version.
 * 4) if needed, displays the kernel parameters.
 * 5) initialises the fine grained allocator.
 * 6) initialises the id manager.
 * 7) initialises the set manager.
 * 8) initialises the address space manager.
 * 9) initialises the segment manager.
 *
 * XXX
 */

void			kaneton(t_init*				bootloader)
{
  /*
   * 1)
   */

  init = bootloader;

  /*
   * 2)
   */

  if (cons_init() != ERROR_NONE)
    kaneton_error("cannot initialise the console manager\n");

  /*
   * 3)
   */

  printf("\n");
  cons_msg('+', "%s\n", version);
  printf("\n");

  /*
   * 4)
   */

#if (DEBUG & DEBUG_PARAMS)
  kaneton_dump();
#endif

  /*
   * 5)
   */

  alloc_init(init->alloc, init->allocsz, FIT_FIRST);

#if 0 /* XXX abuse de mettre ca ici!!! => alloc_test() */
  /* -- Here begins allocation test */
  printf("starting allocation tests\n");
  {
    void* ptr;

    ptr = malloc(7);
    if (!ptr)
      {
	kaneton_error("Cannot allocate memory: 7\n");
	for (;;);
      }

    ptr = malloc(100);
    if (!ptr)
      {
	kaneton_error("Cannot allocate memory: 100\n");
	for (;;);
      }

    ptr = malloc(200);
    if (!ptr)
      {
	kaneton_error("Cannot allocate memory: 200\n");
	for (;;);
      }

    ptr = malloc(300);
    if (!ptr)
      {
	kaneton_error("Cannot allocate memory: 300\n");
	for (;;);
      }

    ptr = malloc(300);
    if (!ptr)
      {
	kaneton_error("Cannot allocate memory: 300\n");
	for (;;);
      }

    ptr = malloc(300);
    if (!ptr)
      {
	kaneton_error("Cannot allocate memory: 300\n");
	for (;;);
      }

    ptr = malloc(300);
    if (!ptr)
      {
	kaneton_error("Cannot allocate memory: 300\n");
	for (;;);
      }

    ptr = malloc(100000);
    if (!ptr)
      {
	kaneton_error("Cannot allocate memory: 100000\n");
	for (;;);
      }
    alloc_dump();
  }
  printf("ending allocation tests\n");
  for (;;);
  /* Here ends allocation test */
#endif

  /*
   * 6)
   */

  if (id_init() != ERROR_NONE)
    kaneton_error("cannot initialise the id manager\n");

  /*
   * 7)
   */

  if (set_init() != ERROR_NONE)
    kaneton_error("cannot initialise the set manager\n");

  /*
   * 8)
   */

  STATS_INIT();

  /*
   * 8)
   */

  if (as_init() != ERROR_NONE)
    kaneton_error("cannot initialise the address space manager\n");

  /*
   * 9)
   */
  /*
  if (segment_init(FIT_FIRST) != ERROR_NONE)
    kaneton_error("cannot initialise the segment manager\n");

#ifdef KANETON_SERIAL
  printf("serial init\n");
  serial_init();
#endif
  */
  /*
   * XXX)
   */

  STATS_DUMP();

  printf("ici c'est une boucle\n");
  while (1)
    ;

  /*
   * XXX)
   */
  /*
  segment_clean();

  as_clean();

  set_clean();

  id_clean();
  */
}

/*
 * this function displays the kaneton parameters.
 */

void			kaneton_dump(void)
{
  cons_msg('#', "dumping kernel parameters:\n");

#if defined(___bootloader)
  cons_msg('#', " mode: bootloader\n");
  kaneton_error("kernel compiled in a bad way where ___bootloader"
		"is defined\n");
#elif defined(___kernel)
  cons_msg('#', " mode: kernel\n");
#else
  kaneton_error("no mode defined\n");
#endif

#if defined(___kaneton)
  cons_msg('#', " name: kaneton\n");
#else
  kaneton_error("___kaneton not defined\n");
#endif

#if (___wordsz == WORDSZ_32)
  cons_msg('#', " wordsize: 32-bit\n");
#elif (___wordsz == WORDSZ_64)
  cons_msg('#', " wordsize: 64-bit\n");
#elif (___wordsz == WORDSZ_128)
  cons_msg('#', " wordsize: 128-bit\n");
#else
  kaneton_error("no word size defined\n");
#endif

#if (___endian == ENDIAN_LITTLE)
  cons_msg('#', " endian: little\n");
#elif (___endian == ENDIAN_BIG)
  cons_msg('#', " endian: big\n");
#else
  kaneton_error("no endian defined\n");
#endif
}
