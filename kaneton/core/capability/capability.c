/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/capability/capability.c
 *
 * created       julien quintard   [sat dec 10 13:56:00 2005]
 * updated       julien quintard   [sat jun 24 14:25:34 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * although the capabilities over the distributed system will have different
 * formats, the kaneton core uses a unique capability format for every
 * kaneton object.
 *
 * the capability manager provides an interface to manipulate the
 * capabilities.
 *
 * the kaneton core capability format is composed of a node identifier on
 * 64 bits specifying the task which generated this capability, an object
 * identifier on 64 bits, an operation field on 16 bits describing the
 * allowed operations and finally a check field on 64 bits to achieve
 * security of the capability.
 *
 * the node identifier is used to locate a service independently of the
 * machine on which it resides. indeed, if a task is migrated on another
 * machine, the node identifier of this task will remain the same. then
 * the user programs of the service will continue to work properly.
 *
 * note that capabilities are not kaneton objects. nevertheless a tip
 * is used to store the capabilities in set objects. indeed, the capability
 * format begins with the check which is a 64-bit number. this check field
 * will so play the role of the kaneton id because.
 *
 * be careful, two capabilities may have the same check field since
 * object identifiers are not uniques between managers. indeed, a segment
 * could have the same identifier than a task for example. for this reason,
 * the check field is assumed unique only for a manager.
 *
 * therefore, the capability manager holds the capabilities in different
 * sets depending on the manager from which the capability was generated.
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
 * XXX
 */

t_error			capability_reserve(void)
{

  return (ERROR_NONE);
}

/*
 * XXX
 */

t_error			capability_release(void)
{

  return (ERROR_NONE);
}

/*
 * XXX
 */

t_error			capability_clone(void)
{

  return (ERROR_NONE);
}

/*
 * XXX
 */

t_error			capability_init(void)
{

  return (ERROR_NONE);
}

/*
 * XXX
 */

t_error			capability_clean(void)
{

  return (ERROR_NONE);
}
