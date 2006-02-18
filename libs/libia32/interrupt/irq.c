/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/libs/libia32/interrupt/handlers.c
 *
 * created       renaud voltz   [fri feb 17 15:18:57 2006]
 * updated       renaud voltz   [fri feb 17 15:18:57 2006]
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


t_irq_handler	irq_handlers[16];

t_irq_handler	irq[16] =
  {
    handler_irq0,
    handler_irq1,
    handler_irq2,
    handler_irq3,
    handler_irq4,
    handler_irq5,
    handler_irq6,
    handler_irq7,
    handler_irq8,
    handler_irq9,
    handler_irq10,
    handler_irq11,
    handler_irq12,
    handler_irq13,
    handler_irq14,
    handler_irq15
  };

/*
 * ---------- functions -------------------------------------------------------
 */

t_error			irq_add(t_uint8			nr)
{
  t_gate		gate;

  if (nr > 15)
    return ERROR_UNKNOWN;

  gate.offset = irq[nr];
  gate.segsel = 1 << 3;
  gate.privilege = 0;
  gate.type = type_gate_interrupt;

  return ERROR_NONE;
}

t_error		irq_init(void)
{
  int		i;

  for (i = 0; i < 16; i++)
    {
      irq_handlers[i] = irq_handler_default;
      if (irq_add(i) != ERROR_NONE)
	return ERROR_UNKNOWN;
    }

  return ERROR_NONE;
}

void		irq_generic_handler(t_uint8	irq)
{
  IRQ_ENTER();

  irq_handlers[irq]();

  IRQ_LEAVE();
}


void	irq_handler_default(void)
{
  printf("default irq handler\n");
}


void	handler_irq0(void)
{
  irq_generic_handler(0);
}

void	handler_irq1(void)
{
  irq_generic_handler(1);
}

void	handler_irq2(void)
{
  irq_generic_handler(2);
}

void	handler_irq3(void)
{
  irq_generic_handler(3);
}

void	handler_irq4(void)
{
  irq_generic_handler(4);
}

void	handler_irq5(void)
{
  irq_generic_handler(5);
}

void	handler_irq6(void)
{
  irq_generic_handler(6);
}

void	handler_irq7(void)
{
  irq_generic_handler(7);
}

void	handler_irq8(void)
{
  irq_generic_handler(8);
}

void	handler_irq9(void)
{
  irq_generic_handler(9);
}

void	handler_irq10(void)
{
  irq_generic_handler(10);
}

void	handler_irq11(void)
{
  irq_generic_handler(11);
}

void	handler_irq12(void)
{
  irq_generic_handler(12);
}

void	handler_irq13(void)
{
  irq_generic_handler(13);
}

void	handler_irq14(void)
{
  irq_generic_handler(14);
}

void	handler_irq15(void)
{
  irq_generic_handler(15);
}
