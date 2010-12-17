/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ue/ibm-pc.ia32/educational/include/map.h
 *
 * created       julien quintard   [sun jun  3 23:54:56 2007]
 * updated       julien quintard   [fri dec 10 21:15:45 2010]
 */

#ifndef GLUE_MAP_H
#define GLUE_MAP_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * these macro-function redirect the calls from the core to the appropriate
 * glue function but also provide the machine-specific data to include
 * in the core managers, objects etc.
 */

#define		machine_include_map()					\
  extern d_map			glue_map_dispatch

#define		machine_call_map(_function_, _args_...)			\
  (									\
    {									\
      t_error	_r_ = ERROR_OK;						\
									\
      if (glue_map_dispatch.map_ ## _function_ != NULL)			\
        _r_ = glue_map_dispatch.map_ ## _function_(_args_);		\
									\
      _r_;								\
    }									\
  )

#define		machine_data_m_map()

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <modules/modules.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../map.c
 */

/*
 * ../map.c
 */


/*
 * eop
 */

#endif
