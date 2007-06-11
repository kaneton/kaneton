/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...kaneton/machine/glue/ibm-pc.ia32/timer.c
 *
 * created       julien quintard   [mon jun 11 05:41:14 2007]
 * updated       julien quintard   [mon jun 11 05:57:55 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements dependant code for the timer managment on a ibm-pc.ia32
 * machine.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

#include <glue/glue.h>
#include <architecture/architecture.h>
#include <platform/platform.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_timer*		timer;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the ibm-pc.ia32 timer manager dispatch.
 */

d_timer				timer_dispatch =
  {
/*								   [cut] k2 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    glue_timer_init,
    NULL,
/*								   [cut] /k2 */
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*								   [cut] k2 */

/*
 * init the timer on the ibm-pc.ia32 machine.
 */

t_error			glue_timer_init(void)
{
  TIMER_ENTER(timer);

  /* XXX[matthieu] peut etre ici meme appeler ibmpc_pit_init() sans argument
     de maniere a alleger le code de glue. il faut qu'il y ait le minimum
     de chose a voir dans glue finalement. */

  if (ibmpc_pit_init(1000 / TIMER_MS_PER_TICK) != ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  /* XXX[matthieu] soit on laisse ca ici, soit on appelle une fonction
     de ia32 qui fera ca mais je pense que ca a plus de sens de le laisse la */
  if (event_reserve(32, EVENT_FUNCTION, EVENT_HANDLER(timer_handler)) !=
      ERROR_NONE)
    TIMER_LEAVE(timer, ERROR_UNKNOWN);

  TIMER_LEAVE(timer, ERROR_NONE);
}

/*								   [cut] /k2 */
