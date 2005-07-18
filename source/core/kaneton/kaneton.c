/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * kaneton.c
 * 
 * path          /home/mycure/kaneton/core/kaneton/set
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Mon Jul 18 11:10:31 2005   mycure
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
 * this function displays the kaneton parameters.
 */

#if (KANETON_DEBUG & KANETON_DEBUG_PARAMS)
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
#endif

/*
 * this function simply initializes each manager.
 *
 * steps:
 *
 * 1) sets the init variable from the bootloader argument.
 * 2) initializes the console manager.
 * 3) displays the current kaneton version.
 * 4) if needed, displays the kernel parameters.
 * 5) initializes the fine grained allocator.
 * 6) initializes the set manager.
 * 7) initializes the address space manager.
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

  if (cons_init() != 0)
    kaneton_error("cannot initialize the console manager\n");

  /*
   * 3)
   */

  printf("\n");
  cons_msg('+', "%s\n", version);
  printf("\n");

  /*
   * 4)
   */

#if (KANETON_DEBUG & KANETON_DEBUG_PARAMS)
  kaneton_dump();
#endif

  /*
   * 5)
   */

  alloc_init(init->alloc, init->allocsz);

  /*
   * 6)
   */

  if (set_init() != 0)
    kaneton_error("cannot initialize the set manager\n");

  /*
   * 7)
   */

  if (as_init() != 0)
    kaneton_error("cannot initialize the address space manager\n");

  /*
   * XXX)
   */

  if (segment_init() != 0)
    kaneton_error("cannot initialize the segment manager\n");

  /*
   * XXX)
   */

  while (1)
    ;
}
