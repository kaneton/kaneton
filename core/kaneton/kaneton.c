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
 * last update   Mon Nov 28 12:52:04 2005   buckman
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file is the entry point of the kaneton microkernel.
 *
 * from the kaneton() function, every managers will be called to initialise
 * them.
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * the students just have to write some code to initialise and clean
 * the different managers.
 *
 * finally some critical services will be launched.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
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
 * 6) if needed, initialises the stats manager.
 * 7) initialises the id manager.
 * 8) initialises the set manager.
 * 9) initialises the address space manager.
 * 10) initialises the segment manager.
 *
 * XXX
 */

void			kaneton(t_init*				bootloader)
{

/*                                                                  [cut] k2 */

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

  /*
   * 6)
   */

  STATS_INIT();

  /*
   * 7)
   */

  if (id_init() != ERROR_NONE)
    kaneton_error("cannot initialise the id manager\n");

  /*
   * 8)
   */

  if (set_init() != ERROR_NONE)
    kaneton_error("cannot initialise the set manager\n");

  /*
   * 9)
   */

  if (as_init() != ERROR_NONE)
    kaneton_error("cannot initialise the address space manager\n");

  /*
   * 10)
   */

  if (segment_init(FIT_FIRST) != ERROR_NONE)
    kaneton_error("cannot initialise the segment manager\n");

/*                                                                 [cut] /k2 */

/*                                                                  [cut] k2 */

  /*
   * XXX
   */

  STATS_DUMP();

  printf("=> %016b %d %u\n", 0x89ab, 0x89ab, 0x89ab);

  /*
   * XXX
   */

  segment_clean();

  as_clean();

  set_clean();

  id_clean();

  STATS_CLEAN();

/*                                                                 [cut] /k2 */

  while (1)
    ;
}

/*                                                                  [cut] k2 */

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

/*                                                                 [cut] /k2 */
