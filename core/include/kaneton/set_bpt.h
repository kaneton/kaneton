/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * set_bpt.h
 * 
 * path          /home/mycure/kaneton/core/include/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sun Jun 19 14:51:33 2005   mycure
 * last update   Thu Aug 25 19:03:48 2005   mycure
 */

#ifndef KANETON_SET_BPT_H
#define KANETON_SET_BPT_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <kaneton/set.h>
#include <arch/machdep/machdep.h>

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton/bpt.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

#define SET_BPT_ADDR_T		void*
#define SET_BPT_KEY_T		t_id
#define SET_BPT_VALUE_T		void*

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

bpt_make_types(set, BPT_NODES_T, BPT_NDI_T, BPT_UNI_T, BPT_NODES_T,
	       BPT_HEIGHT_T, SET_BPT_ADDR_T, SET_BPT_KEY_T, SET_BPT_VALUE_T,
	       t_set_bpt_inentry, t_set_bpt_lfentry)

/*
 * bpt set type
 */

typedef struct
{
  t_opts			opts;

  t_size			datasz;

  t_bpt_node(set)		data;
}				t_set_bpt;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/set/set_bpt.c
 */

/*
 * eop
 */

/*
 * bpt prototypes
 */

bpt_make_protos(set)

#endif
