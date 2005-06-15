/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * kaneton.c
 * 
 * path          /home/mycure/kaneton/core/kaneton/segment
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Tue Jun 14 19:19:37 2005   mycure
 */

#include <libc.h>
#include <kaneton.h>

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
 * this function displays the kaneton parameters.
 */

void			kaneton_dump(void)
{
  cons_msg('#', "dumping kernel parameters:\n");

#if defined(___bootloader)
  cons_msg('#', " mode: bootloader\n");

  cons_msg('!', " error: kernel compiled in a bad way where ___bootloader"
	   " is defined\n");
  // XXX kaneton_error();
#elif defined(___kernel)
  cons_msg('#', " mode: kernel\n");
#else
  cons_msg('!', " error: no mode defined\n");
  // XXX kaneton_error();
#endif

#if defined(___kaneton)
  cons_msg('#', " name: kaneton\n");
#else
  cons_msg('!', " error: ___kaneton not defined\n");
  // XXX kaneton_error();
#endif

#if defined(___32bit)
  cons_msg('#', " wordsize: 32-bit\n");
#elif defined(___64bit)
  cons_msg('#', " wordsize: 64-bit\n");
#else
  cons_msg('!', " error: no wordsize defined\n");
  // XXX kaneton_error();
#endif

#if defined(___little_endian)
  cons_msg('#', " endian: little\n");
#elif defined(___big_endian)
  cons_msg('#', " endian: big\n");
#else
  cons_msg('!', " error: no endian defined\n");
  // XXX kaneton_error();
#endif

}

/*
 * this function simply initializes each manager.
 *
 * steps:
 *
 * 1) sets the init variable from the bootloader argument.
 * 2) initializes the console manager.
 * 3) displays the current kaneton version.
 * 4) if needed, displays the kernel parameters: 
 * 6) initializes the address space manager.
 * 5) initializes the segment manager.
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
    cons_msg('!', "error: cannot initialize the console manager\n");

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

  if (as_init() != 0)
    cons_msg('!', "error: cannot initialize the address space manager\n");
  /*
   * 6)
   */

  if (segment_init() != 0)
    cons_msg('!', "error: cannot initialize the segment manager\n");

  while (1)
    ;
}
