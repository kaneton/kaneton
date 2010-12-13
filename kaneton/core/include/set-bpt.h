/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/set-bpt.h
 *
 * created       julien quintard   [wed jun  6 14:14:36 2007]
 * updated       julien quintard   [sun dec 12 22:46:19 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file supposes it is included by the set.h header file.
 */

#ifndef CORE_SET_BPT_H
#define CORE_SET_BPT_H			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#undef BPT_DEBUG

#include <core/bpt.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * these macro defines the set implementation bpt types.
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
 * bpt internal and leaf entries.
 */

typedef struct
{
  SET_BPT_KEY_T			id;
  SET_BPT_ADDR_T		data;
}				s_set_bpt_inentry;

typedef struct
{
  SET_BPT_KEY_T			id;
  SET_BPT_VALUE_T		data;
}				s_set_bpt_lfentry;

/*
 * this macro-function call generates the bpt types.
 */

bpt_make_types(set, BPT_NODESZ_T, BPT_NDI_T, BPT_UNI_T, BPT_NODES_T,
	       BPT_HEIGHT_T, SET_BPT_ADDR_T, SET_BPT_KEY_T, SET_BPT_VALUE_T,
	       s_set_bpt_inentry, s_set_bpt_lfentry)

/*
 * set descriptor's bpt-specific types.
 */

typedef struct
{
  t_bpt_unused(set)		unused;
  t_bpt_uni(set)		unusedsz;

  t_bpt(set)			bpt;
}				s_set_bpt;

/*
 * the bpt-specific iterator.
 */

typedef struct
{
  t_bpt_entry(set)		entry;
}				s_iterator_bpt;

/*
 * ---------- prototypes ------------------------------------------------------
 */

/*
 * this macro-function call generates the bpt prototypes.
 */

bpt_make_protos(set)

#endif
