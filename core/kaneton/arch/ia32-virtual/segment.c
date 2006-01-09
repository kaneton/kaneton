/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/kaneton/arch/ia32-virtual/kaneton/segment.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [wed dec 28 19:46:51 2005]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_segment*	segment;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the segment manager interface.
 */

i_segment		segment_interface =
  {
    NULL,
    ia32_segment_reserve,
    ia32_segment_release,
    NULL,
    NULL,
    NULL,
    NULL,
    ia32_segment_init,
    ia32_segment_clean
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function just initialises the machine-dependent segment manager.
 *
 * for the intel 32-bit architecture we just initialise the protected mode.
 */

t_error			ia32_segment_init(t_fit			fit)
{
  SEGMENT_ENTER(segment);

  if (pmode_init() != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function cleans the machine-dependent segment manager.
 */

t_error			ia32_segment_clean(void)
{
  SEGMENT_ENTER(segment);

  if (pmode_clean() != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  SEGMENT_LEAVE(segment, ERROR_NONE);
}

/*
 * this function reserves a segment given the desired size.
 */

t_error			ia32_segment_reserve(t_asid		asid,
					     t_psize		size,
					     t_perms		perms,
					     t_segid*		segid)
{
  o_as*			as;
  o_task*		tsk;
  t_segment		seg;
  t_uint16		segidx;
  t_error		res;

  SEGMENT_ENTER(segment);

  seg.base = 0; // XXX
  seg.limit = seg.base + size;

  if (as_get(asid, &as) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if (task_get(as->tskid, &tsk) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if (tsk->class == TASK_CLASS_CORE)
    seg.privilege = prvl_supervisor;
  else if (tsk->class == TASK_CLASS_DRIVER || tsk->class == TASK_CLASS_SERVICE)
    seg.privilege = prvl_service;
  else if (tsk->class == TASK_CLASS_PROGRAM)
    seg.privilege = prvl_user;
  else
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  seg.is_system = 0;
  if (perms & PERM_EXEC)
    seg.type.usr = type_code;
  else if ((perms & PERM_READ) || (perms & PERM_WRITE))
    seg.type.usr = type_data;
  else
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  res = ldt_reserve_segment(&as->machdep.ldt, seg, &segidx);

  // XXX segid ?

  SEGMENT_LEAVE(segment, res);
}

/*
 * this function releases a segment.
 */

t_error			ia32_segment_release(t_segid			segid)
{
  o_as*			as;
  o_segment*		seg;
  t_error		res;

  SEGMENT_ENTER(segment);

  if (segment_get(segid, &seg) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  if (as_get(seg->asid, &as) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /* XXX
  res = ldt_delete_segment(&as->machdep.ldt, seg);
  */
  res = ERROR_UNKNOWN;

  SEGMENT_LEAVE(segment, res);
}
