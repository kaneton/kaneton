/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/set-bpt.h
 *
 * created       julien quintard   [sun jun 19 14:51:33 2005]
 * updated       matthieu bucchianeri   [tue feb  6 23:22:48 2007]
 */

#ifndef CORE_SET_BPT_H
#define CORE_SET_BPT_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/set.h>
#include <arch/machdep/machdep.h>

/*
 * ---------- includes --------------------------------------------------------
 */

#undef BPT_DEBUG

#include <sys/bpt.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define SET_BPT_ADDR_T		void*
#define SET_BPT_KEY_T		t_id
#define SET_BPT_VALUE_T		void*

#define SET_BPT_UADDR		NULL
#define SET_BPT_UKEY		ID_UNUSED
#define SET_BPT_UVALUE		NULL

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * bpt inentry and lfentry structures
 */

typedef struct
{
  SET_BPT_KEY_T			id;
  SET_BPT_ADDR_T		data;
}				t_set_bpt_inentry;

typedef struct
{
  SET_BPT_KEY_T			id;
  SET_BPT_VALUE_T		data;
}				t_set_bpt_lfentry;

/*
 * bpt types
 */

bpt_make_types(set, BPT_NODESZ_T, BPT_NDI_T, BPT_UNI_T, BPT_NODES_T,
	       BPT_HEIGHT_T, SET_BPT_ADDR_T, SET_BPT_KEY_T, SET_BPT_VALUE_T,
	       t_set_bpt_inentry, t_set_bpt_lfentry)

/*
 * specific bpt set
 */

typedef struct
{
  t_bpt_unused(set)		unused;
  t_bpt_uni(set)		unusedsz;

  t_bpt(set)			bpt;
}				t_set_bpt;

/*
 * bpt iterator
 */

typedef struct
{
  t_bpt_entry(set)		entry;
}				t_iterator_bpt;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 */

/*
 * bpt prototypes
 */

bpt_make_protos(set)

#endif
