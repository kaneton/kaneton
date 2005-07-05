/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * set.c
 * 
 * path          /home/mycure/kaneton/core/kaneton/set
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Mon Jul  4 21:48:55 2005   mycure
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the set manager is used to manage the data structures in order to simplify
 * the other kernel managers. indeed, every kernel manager including the
 * task manager, the thread manager, the segment manager etc.. uses the
 * set manager to store the data rather than create themselves data structures
 * by their own.
 *
 * notice that the set manager is based on the malloc() suite functions. indeed
 * the set manager maintains a data structure containing the set descriptors.
 * moreover some data structure type required the malloc() suite functions, the
 * simplest example being the array data structure which require the realloc()
 * function to be able to extend the data structure without effort.
 *
 * due to this requirement, the set manager cannot manage the data structure
 * for the segments of the segment manager, the kernel address space segments
 * and the kernel address space regions of the address space manager because
 * the malloc() function is not provided yet. to bypass this restriction
 * the kaneton kernel uses a initial malloc() version which is based on
 * a little amount of memory provided by the bootloader, via the fields
 * alloc and allocsz of the init structure.
 *
 * then once the kernel starts, the malloc() suite functions are initialized
 * using this special memory used by the segment manager, the region
 * manager and the address space manager to initialize themselves. Thereafter,
 * the malloc() suite functions are able to work properly.
 *
 * the implementation use macros to provide different data structure
 * creation with different number of arguments and number of arguments.
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
 * the set manager variable.
 */

m_set			set;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function adds the set object to the set objects list.
 */

int			set_object(o_set*			o)
{
  /*
   * XXX
   */

  return (0);
}

/*
 * this function returns an unique unused set identifier.
 */

int			set_id(t_setid*				setid)
{
  return (set.id++);
}

/*
 * this function returns the set corresponding to a set identifier.
 */

int			set_get(t_setid				setid,
				o_set**				o)
{
  printf("set_get(): %qu\n", setid);

  /*
   * XXX
   */

  return (-1);
}

/*
 * this function initializes the set manager.
 *
 * steps:
 *
 * 1) XXX
 */

int			set_init(void)
{
  memset(&set, 0x0, sizeof(m_set));

  return (0);
}
