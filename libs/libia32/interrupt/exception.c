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
 *
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

t_exception_handler	exception_handlers[32];

static t_exception_handler	exception[32] =
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
 * ---------- functions -------------------------------------------------------
 */

t_error			exception_add(t_uint8			nr)
{
  t_gate		gate;

  if (nr > 31)
    return ERROR_UNKNOWN;

  gate.offset = exception[nr];
  gate.segsel = 1 << 3;
  gate.privilege = 0;
  gate.type = type_gate_interrupt;

  return ERROR_NONE;
}

t_error			exception_init(void)
{
  int			i;

  for (i = 0; i < 32; i++)
    {
      exception_handlers[i] = exception_handler_default;
      if (exception_add(i) != ERROR_NONE)
	return ERROR_UNKNOWN;
    }

  return ERROR_NONE;
}

void			exception_generic_handler(t_uint8	nr)
{
  EXCEPTION_ENTER();

  exception_handlers[nr]();

  EXCEPTION_LEAVE();
}


void			exception_handler_default(void)
{
  printf("default exception handler");
}


void			handler_exception0(void)
{
  exception_generic_handler(0);
}

void			handler_exception1(void)
{
  exception_generic_handler(1);
}

void			handler_exception2(void)
{
  exception_generic_handler(2);
}

void			handler_exception3(void)
{
  exception_generic_handler(3);
}

void			handler_exception4(void)
{
  exception_generic_handler(4);
}

void			handler_exception5(void)
{
  exception_generic_handler(5);
}

void			handler_exception6(void)
{
  exception_generic_handler(6);
}

void			handler_exception7(void)
{
  exception_generic_handler(7);
}

void			handler_exception8(void)
{
  exception_generic_handler(8);
}

void			handler_exception9(void)
{
  exception_generic_handler(9);
}

void			handler_exception10(void)
{
  exception_generic_handler(10);
}

void			handler_exception11(void)
{
  exception_generic_handler(11);
}

void			handler_exception12(void)
{
  exception_generic_handler(12);
}

void			handler_exception13(void)
{
  exception_generic_handler(13);
}

void			handler_exception14(void)
{
  exception_generic_handler(14);
}

void			handler_exception15(void)
{
  exception_generic_handler(15);
}

void			handler_exception16(void)
{
  exception_generic_handler(16);
}

void			handler_exception17(void)
{
  exception_generic_handler(17);
}

void			handler_exception18(void)
{
  exception_generic_handler(18);
}

void			handler_exception19(void)
{
  exception_generic_handler(19);
}

void			handler_exception20(void)
{
  exception_generic_handler(20);
}

void			handler_exception21(void)
{
  exception_generic_handler(21);
}

void			handler_exception22(void)
{
  exception_generic_handler(22);
}

void			handler_exception23(void)
{
  exception_generic_handler(23);
}

void			handler_exception24(void)
{
  exception_generic_handler(24);
}

void			handler_exception25(void)
{
  exception_generic_handler(25);
}

void			handler_exception26(void)
{
  exception_generic_handler(26);
}

void			handler_exception27(void)
{
  exception_generic_handler(27);
}

void			handler_exception28(void)
{
  exception_generic_handler(28);
}

void			handler_exception29(void)
{
  exception_generic_handler(29);
}

void			handler_exception30(void)
{
  exception_generic_handler(30);
}

void			handler_exception31(void)
{
  exception_generic_handler(31);
}
