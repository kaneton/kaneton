/*
 * licence       Kaneton licence
 *
 * project       exception.h
 *
 * file          /home/rhino/kaneton/libs/libia32/interrupt/exception.h
 *
 * created       renaud voltz   [sat feb 18 16:08:50 2006]
 * updated       renaud voltz   [sat feb 18 16:08:50 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * exception handling
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 *
 */


/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * global exception handler table
 */

t_interrupt_handler		exception_handlers[32];

/*
 * ---------- functions -------------------------------------------------------
 */

static t_interrupt_handler	exception[32] =
  {
    handler_exception0,
    handler_exception1,
    handler_exception2,
    handler_exception3,
    handler_exception4,
    handler_exception5,
    handler_exception6,
    handler_exception7,
    handler_exception8,
    handler_exception9,
    handler_exception10,
    handler_exception11,
    handler_exception12,
    handler_exception13,
    handler_exception14,
    handler_exception15,
    handler_exception16,
    handler_exception17,
    handler_exception18,
    handler_exception19,
    handler_exception20,
    handler_exception21,
    handler_exception22,
    handler_exception23,
    handler_exception24,
    handler_exception25,
    handler_exception26,
    handler_exception27,
    handler_exception28,
    handler_exception29,
    handler_exception30,
    handler_exception31
  };

/*
 * add an idt entry
 *
 * steps:
 *
 * 1) check exception identifier
 * 2) set interrupt gate and add it into the idt
 */

t_error			exception_add(t_uint8			nr,
				      t_prvl			privilege,
				      t_interrupt_handler	handler)
{
  t_gate		gate;

  /*
   * 1)
   */

  if (nr > 31)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  gate.offset = (t_uint32)exception[nr];
  gate.segsel = 1 << 3;
  gate.privilege = privilege;
  gate.type = type_gate_interrupt;

  return ERROR_NONE;
}

/*
 * init exceptions
 *
 * steps:
 *
 * for each exception:
 * 1) assign default handler
 * 2) fill appropriate idt entry
 */

t_error			exception_init(void)
{
  int			i;

  for (i = 0; i < 32; i++)
    {

      /*
       * 1)
       */

      exception_handlers[i] = exception_handler_default;

      /*
       * 2)
       */

      if (exception_add(i, 0, exception[i]) != ERROR_NONE)
	return ERROR_UNKNOWN;
    }

  return ERROR_NONE;
}

/*
 * call exception handler
 */

void			exception_wrapper(t_uint32		nr)
{
  exception_handlers[nr]();
}

/*
 * do nothing
 */

void			exception_handler_default(void)
{
  return;
}

/*
 * exception pre-handlers definitions
 */

EXCEPTION_PREHANDLER(0)
EXCEPTION_PREHANDLER(1)
EXCEPTION_PREHANDLER(2)
EXCEPTION_PREHANDLER(3)
EXCEPTION_PREHANDLER(4)
EXCEPTION_PREHANDLER(5)
EXCEPTION_PREHANDLER(6)
EXCEPTION_PREHANDLER(7)
EXCEPTION_PREHANDLER(8)
EXCEPTION_PREHANDLER(9)
EXCEPTION_PREHANDLER(10)
EXCEPTION_PREHANDLER(11)
EXCEPTION_PREHANDLER(12)
EXCEPTION_PREHANDLER(13)
EXCEPTION_PREHANDLER(14)
EXCEPTION_PREHANDLER(15)
EXCEPTION_PREHANDLER(16)
EXCEPTION_PREHANDLER(17)
EXCEPTION_PREHANDLER(18)
EXCEPTION_PREHANDLER(19)
EXCEPTION_PREHANDLER(20)
EXCEPTION_PREHANDLER(21)
EXCEPTION_PREHANDLER(22)
EXCEPTION_PREHANDLER(23)
EXCEPTION_PREHANDLER(24)
EXCEPTION_PREHANDLER(25)
EXCEPTION_PREHANDLER(26)
EXCEPTION_PREHANDLER(27)
EXCEPTION_PREHANDLER(28)
EXCEPTION_PREHANDLER(29)
EXCEPTION_PREHANDLER(30)
EXCEPTION_PREHANDLER(31)
