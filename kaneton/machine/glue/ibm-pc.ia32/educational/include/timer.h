
/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../ibm-pc.ia32/educational/include/timer.h
 *
 * created       julien quintard   [wed jun  6 16:29:41 2007]
 * updated       julien quintard   [thu dec  9 15:16:01 2010]
 */

#ifndef GLUE_TIMER_H
#define GLUE_TIMER_H			1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_timer()					\
  extern d_timer	glue_timer_dispatch

#define		machine_call_timer(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_OK;						\
									\
      if (glue_timer_dispatch._function_ != NULL)			\
        _r_ = glue_timer_dispatch._function_(_args_);			\
									\
      _r_;								\
    }									\
  )

#define		machine_data_m_timer()

#define		machine_data_o_timer()

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../timer.c
 */

/*
 * ../timer.c
 */

t_error			glue_timer_initialize(void);

t_error			glue_timer_clean(void);


/*
 * eop
 */

#endif
