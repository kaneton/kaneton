/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/include/clock.h
 *
 * created       julien quintard   [wed nov 24 18:52:04 2010]
 * updated       julien quintard   [fri dec 10 21:16:50 2010]
 */

#ifndef CORE_CLOCK_H
#define CORE_CLOCK_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the structure of a clock state instance.
 */

typedef struct
{
  t_uint32		millisecond;
  t_uint8		second;
  t_uint8		minute;
  t_uint8		hour;

  t_uint8		day;
  t_uint8		month;
  t_uint16		year;
}			s_clock;

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/error.h>
#include <machine/machine.h>

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro-function takes a clock structure and computes a unique number
 * representing the number of milliseconds elapsed since the birth of Jesus
 * Christ.
 *
 * since this number can be larger than 32-bit, a 64-bit number should be
 * used to store it. the type t_clock_unique has been introduced for that
 * purpose.
 */

#define CLOCK_UNIQUE(_clock_)						\
  (									\
    {									\
      (t_uint64)(							\
		 (_clock_)->millisecond +				\
		 (_clock_)->second * 1000 +				\
		 (_clock_)->minute * 60 * 1000 +			\
		 (_clock_)->hour * 60 * 60 * 1000 +			\
		 (_clock_)->day * 24 * 60 * 60 * 1000 +			\
		 (_clock_)->month * 30 * 60 * 60 * 1000 +		\
		 (_clock_)->year * 12 * 30 * 60 * 60 * 1000		\
		);							\
    }									\
  )

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the clock unique representation.
 */

typedef t_uint64		t_clock_unique;

/*
 * the clock manager's structure.
 */

typedef struct
{
  machine_data(m_clock);
}				m_clock;

/*
 * the clock dispatcher.
 */

typedef struct
{
  t_status		(*clock_show)(s_clock*,
					      mt_margin);
  t_status		(*clock_update)(t_uint32);
  t_status		(*clock_current)(s_clock*);
  t_status		(*clock_initialize)(void);
  t_status		(*clock_clean)(void);
}				d_clock;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/clock/clock.c
 */

/*
 * ../../core/clock/clock.c
 */

t_status		clock_show(s_clock*			clock,
				   mt_margin			margin);

t_status		clock_update(t_uint32			millisecond);

t_status		clock_current(s_clock*			clock);

t_status		clock_initialize(void);

t_status		clock_clean(void);


/*
 * eop
 */

#endif
