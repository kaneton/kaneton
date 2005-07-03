/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * set.c
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Sun Jul  3 19:44:09 2005   mycure
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the set manager is used to manages the data structure in order to simplify
 * the other kernel managers. indeed, every kernel manager including the
 * task manager, the thread manager, the segment manager etc.. use the
 * set manager to store the data rather than create themselves data structures
 * by their own.
 *
 * notice that the set manager is based on the malloc() function. indeed
 * the set manager maintains a data structure containing the set descriptors.
 * moreover some data structure type required the malloc() function, the
 * simplest example being the array data structure which require the realloc()
 * function to be able to extend the data structure without effort.
 *
 * due to this requirement, the set manager cannot manage the data structure
 * for the segments of the segment manager, the kernel address space segments
 * and the kernel address space regions of the address space manager because
 * the malloc() function is not provided yet. to bypass this restriction these
 * three sets are managed by the hand to allow every kernel manager to use
 * the set manager.
 *
 * the implementation use macros. XXX
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

  switch (setid)
    {
    case SETID_SEGMENTS:

      printf("SEGMENTS\n");

      *o = &set.segments;

      return (0);
    }

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

  set.id = SET_ID;

  set_make(ll, &set.segments, SETID_SEGMENTS, NULL, NULL);

  set_make(ll, &set.ksegments, SETID_KSEGMENTS, NULL, NULL);
  set_make(ll, &set.kregions, SETID_KREGIONS, NULL, NULL);

  return (0);
}
