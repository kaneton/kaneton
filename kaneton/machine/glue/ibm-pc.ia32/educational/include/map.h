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
 * updated       julien quintard   [sun dec 19 13:43:53 2010]
 */

#ifndef GLUE_MAP_H
#define GLUE_MAP_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro-function defines the map dispatcher.
 */

#define		machine_include_map()					\
  extern d_map			glue_map_dispatch

/*
 * this macro-function dispatches the map calls.
 */

#define		machine_call_map(_function_, _args_...)			\
  (									\
    {									\
      t_status _r_ = STATUS_OK;						\
									\
      if (glue_map_dispatch.map_ ## _function_ != NULL)			\
        _r_ = glue_map_dispatch.map_ ## _function_(_args_);		\
									\
      _r_;								\
    }									\
  )

/*
 * this macro-function includes data in 'm_map'.
 */

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
