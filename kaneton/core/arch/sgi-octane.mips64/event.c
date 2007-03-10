/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/core/arch/sgi-o2.mips64/event.c
 *
 * created       enguerrand raymond   [wed oct 18 14:26:01 2006]
 * updated       enguerrand raymond   [sat mar 10 14:20:26 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements dependent code for the events on octane with paging
 * architecture.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libmipsr10000.h>
#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- extern ---------------------------------------------------------
 */

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the event manager dispatch.
 */

d_event				event_dispatch =
  {
    NULL,
    mipsr10k_event_init,
    NULL,
    NULL,
    NULL,
    NULL,
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * Function to handle non maskable interruption (NMI)
 *
 */
void	mipsr10k_event_handler(void)
{
  asm("nop\n\t"
      "nop\n\t");
  printf("Handler\n");
  asm("mfc0 $10, $12\n\t"
      "ori $10, $10, 0xff01\n\t"
      "mtc0 $10, $12\n\t");
}

/*
 * This function set the IE register of the status register
 * to activate all of the eight interrupts and set the interrupt
 * bit in the regisdter IM of the status register
 *
 * 1) Set the IE bit and the set compare register with an arbitrary value
 */

static void		mipsr10k_event_interrupt_activate(void)
{
  /*
   * 1)
   */

  asm("mfc0 $10, $12\n\t"
      "ori $10, $10, 0xff01\n\t"
      "mtc0 $10, $12\n\t"
      "add $10,$0, $0\n\t"
      "li  $10, 0x8000\n\t"
      "mtc0 $10, $11\n\t");
}

/*
 * Function to record the handler at the address 0x80000180 with a jump
 * instruction to the function mipsr10k_event_handler, then activate
 * interruptions and exceptions
 *
 * 1) Record the handler with jump instruction
 * 2) Call function to activate interruption and exception
 */

t_error			mipsr10k_event_init(void)
{
  unsigned long*	vector_addr = 0x80000180;
  unsigned int		handler_addr = (unsigned long*)mipsr10k_event_handler;
  unsigned int		instr = 0x08000000;

  printf("[+] Interrupts activation\n");

  /*
   * 1)
   */

  handler_addr = handler_addr << 2;
  handler_addr = handler_addr >> 4;
  handler_addr &= 0x03ffffff;
  instr ^= handler_addr;
  *vector_addr = instr;
  ++vector_addr;
  *vector_addr = 0x00000000;

  /*
   * 2)
   */

  mipsr10k_event_interrupt_activate();

  return ERROR_NONE;
}
