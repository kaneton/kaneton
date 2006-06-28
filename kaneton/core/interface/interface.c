/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/interface/interface.c
 *
 * created       julien quintard   [sat dec 10 13:56:00 2005]
 * updated       julien quintard   [sat jun 24 16:34:09 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file describes the core's interface.
 *
 * in other words, this file describes the services provided by the kernel
 * so the core functions a task is allowed to use.
 *
 * each time a message is destinated to the core, the corresponding
 * function of the interface is launched.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function launchs the segment_perms() function.
 */

void			interface_segment_perms(/*o_message*	message*/void)
{
  /*
   * XXX
   *
   * get the argument depending of the function prototype. here we
   * are talking about segment_perms() which takes two arguments:
   * i_segment and t_perms.
   *
   * check the operations field to be sure the caller has the right to
   * use this functionality. if not returns an error.
   *
   *   capability_check(segment, SEGMENT_OPERATION_PERMS);
   *
   * which will actually call the function capability_check_segment().
   *
   * check the capability correctness with capability_verify(segment, c)
   *
   * finally, the capability is correct and the caller has to right to
   * perform the desired operation, then the effective function is just
   * called.
   *
   *   segment_perms(o.capability.id, o.perms);
   *
   * then, the result of the operation is returned to the caller.
   */
}

/*
 * XXX en fait dans ce fichier on va avoir une fonction interface_dispatch
 * ou interface_call qui va lancer les dites fonctions...
 *
 * et on aura interface_segment avec toutes les fonctions de segment
 */
