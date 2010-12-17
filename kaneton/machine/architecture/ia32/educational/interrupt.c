/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/interrupt/interrupt.c
 *
 * created       renaud voltz   [thu feb 23 10:49:43 2006]
 * updated       matthieu bucchianeri   [wed mar 21 22:54:16 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements exception and irq handling on ia32 architecture.
 * both exceptions and irq's are called interrupts in the machine-dependent
 * code of kaneton.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <architecture/architecture.h>


/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the kernel manager.
 */

extern m_kernel*	_kernel;

/*
 * ---------- non-generated prototypes ----------------------------------------
 */

PREHANDLER_PROTOTYPES();

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the segment selector to load on interrupt.
 */

IA32_HANDLER_DATA_SECTION t_uint16	ia32_interrupt_ds = 0;

/*
 * the page directory to load on interrupt.
 */

IA32_HANDLER_DATA_SECTION t_uint32	ia32_interrupt_pdbr = 0;

/*
 * ---------- functions -------------------------------------------------------
 */

static t_ia32_interrupt_prehandler		prehandlers[IA32_HANDLER_NR] =
  {
    prehandler_exception0,
    prehandler_exception1,
    prehandler_exception2,
    prehandler_exception3,
    prehandler_exception4,
    prehandler_exception5,
    prehandler_exception6,
    prehandler_exception7,
    prehandler_exception8,
    prehandler_exception9,
    prehandler_exception10,
    prehandler_exception11,
    prehandler_exception12,
    prehandler_exception13,
    prehandler_exception14,
    prehandler_exception15,
    prehandler_exception16,
    prehandler_exception17,
    prehandler_exception18,
    prehandler_exception19,
    prehandler_exception20,
    prehandler_exception21,
    prehandler_exception22,
    prehandler_exception23,
    prehandler_exception24,
    prehandler_exception25,
    prehandler_exception26,
    prehandler_exception27,
    prehandler_exception28,
    prehandler_exception29,
    prehandler_exception30,
    prehandler_exception31,
    prehandler_irq0,
    prehandler_irq1,
    prehandler_irq2,
    prehandler_irq3,
    prehandler_irq4,
    prehandler_irq5,
    prehandler_irq6,
    prehandler_irq7,
    prehandler_irq8,
    prehandler_irq9,
    prehandler_irq10,
    prehandler_irq11,
    prehandler_irq12,
    prehandler_irq13,
    prehandler_irq14,
    prehandler_irq15,
    prehandler_ipi0,
    prehandler_ipi1,
    prehandler_ipi2,
    prehandler_ipi3,
    prehandler_ipi4,
    prehandler_ipi5,
    prehandler_ipi6,
    prehandler_ipi7,
    prehandler_syscall0,
    prehandler_syscall1,
    prehandler_syscall2,
    prehandler_syscall3,
    prehandler_syscall4,
    prehandler_syscall5,
    prehandler_syscall6,
    prehandler_syscall7,
    prehandler_syscall8,
    prehandler_syscall9 
    /*
    prehandler_syscall10,
    prehandler_syscall11,
    prehandler_syscall12,
    prehandler_syscall13,
    prehandler_syscall14,
    prehandler_syscall15
    */
  };

/*
 * add an idt entry.
 *
 * steps:
 *
 * 1) check interrupt identifier.
 * 3) build an interrupt gate and add it into the idt.
 */

static t_error		interrupt_add(t_uint32			nr,
				      t_ia32_privilege	       	privilege,
				      t_ia32_interrupt_prehandler prehandler)
{
  t_ia32_gate		gate;

  /*
   * 1)
   */

  if (nr >= IA32_HANDLER_NR)
    return ERROR_KO;

  /*
   * 2)
   */

  gate.offset = (t_uint32)prehandler;
  gate.segsel = IA32_PMODE_GDT_KERNEL_CS << 3;
  gate.privilege = privilege;
  gate.type = ia32_type_gate_interrupt;

  return ia32_idt_add_gate(NULL, nr, gate);
}

/*
 * init interrupts.
 *
 * steps:
 *
 * 1) allocate space for the idt.
 * 2) build the idt and activate it.
 * 3) add an interrupt gate descriptor in the idt for each exception.
 * 4) add an interrupt gate descriptor in the idt for each irq.
 * 5) add an interrupt gate descriptor in the idt for each ipi.
 * 6) add an interrupt gate descriptor in the idt for each syscall.
 */

t_error			ia32_interrupt_vector_init(void)
{
  int			i;
  i_segment		seg;
  i_region		reg;
  t_vaddr		vaddr;
  t_ia32_idt		new_idt;
  o_region*		r;

  /*
   * 1)
   */

  if (segment_reserve(_kernel->as,
		      PAGESZ,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &seg) != ERROR_OK)
    return (ERROR_KO);

  if (segment_type(seg, SEGMENT_TYPE_SYSTEM) != ERROR_OK)
    return (ERROR_KO);

  if (region_reserve(_kernel->as,
		     seg,
		     0,
		     REGION_OPTION_GLOBAL | REGION_OPTION_PRIVILEGED,
		     0,
		     PAGESZ,
		     &reg) != ERROR_OK)
    return (ERROR_KO);

  if (region_get(_kernel->as, reg, &r) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  vaddr = r->address;

  //printf("IDT 0x%x\n", vaddr);

  /*
   * 2)
   */

  if (ia32_idt_build(IA32_IDT_MAX_ENTRIES, vaddr, 1, &new_idt) != ERROR_OK)
    return ERROR_KO;

  if (ia32_idt_activate(&new_idt) != ERROR_OK)
    return ERROR_KO;

  /*
   * 3)
   */

  for (i = IA32_IDT_EXCEPTION_BASE;
       i < IA32_IDT_EXCEPTION_BASE + IA32_EXCEPTION_NR;
       i++)
    if (interrupt_add(i, 0, prehandlers[i]) != ERROR_OK)
      return ERROR_KO;

  /*
   * 4)
   */

  for (i = IA32_IDT_IRQ_BASE;
       i < IA32_IDT_IRQ_BASE + IA32_IRQ_NR;
       i++)
    if (interrupt_add(i, 0, prehandlers[i]) != ERROR_OK)
      return ERROR_KO;

  /*
   * 5)
   */

  for (i = IA32_IDT_IPI_BASE;
       i < IA32_IDT_IPI_BASE + IA32_IPI_NR;
       i++)
    if (interrupt_add(i, 0, prehandlers[i]) != ERROR_OK)
      return ERROR_KO;

  /*
   * 6)
   */

  for (i = IA32_IDT_SYSCALL_BASE;
       i < IA32_IDT_SYSCALL_BASE + IA32_SYSCALL_NR;
       i++)
    if (interrupt_add(i, 3, prehandlers[i]) != ERROR_OK)
      return ERROR_KO;

  return ERROR_OK;
}

/*
 * this handler is called on unhandled interrupts.
 */

static void		spurious_interrupt(i_event			id)
{
  t_uint32		stack[8];
  i_thread		th;
  o_task*		o;
  t_ia32_context	ctx;
  o_thread*		othread;

  assert(ia32_context_ring0_stack() == ERROR_OK);

  if (thread_current(&th) != ERROR_OK ||
      ia32_get_context(th, &ctx) != ERROR_OK)
    {
      module_call(console, print,
		  "Unhandled exception %qu\n", id);
      return;
    }

  module_call(console, print,
	      "Unhandled exception %qu @ %p\n", id, ctx.eip);
  module_call(console, print,
	      "eflags=%08x\tcs=%04x\tds=%04x\tss=%04x\n", ctx.eflags,
	      ctx.cs, ctx.ds, ctx.ss);
  module_call(console, print,
	      "%%eax=%08x\t%%ebx=%08x\n", ctx.eax, ctx.ebx);
  module_call(console, print,
	      "%%ecx=%08x\t%%edx=%08x\n", ctx.ecx, ctx.edx);
  module_call(console, print,
	      "%%esi=%08x\t%%edi=%08x\n", ctx.esi, ctx.edi);
  module_call(console, print,
	      "%%esp=%08x\t%%ebp=%08x\n", ctx.esp, ctx.ebp);

  if (thread_get(th, &othread) != ERROR_OK)
    return;

  if (task_get(othread->task, &o) != ERROR_OK)
    return;

  if (as_read(o->as, ctx.esp, 32, stack) == ERROR_OK)
    {
      module_call(console, print,
		  "%08x %08x %08x %08x\n", stack[0], stack[1], stack[2], stack[3]);
      module_call(console, print,
		  "%08x %08x %08x %08x\n", stack[4], stack[5], stack[6], stack[7]);
    }
}

/*
 * this is the second handler called on exception.
 * it calls the user-defined handler.
 */

void			ia32_handler_exception(t_uint32			nr,
					       t_uint32			code)
{
  o_event*		o;
  i_event		id = nr;
  i_thread		current;

  //printf("[XXX] ia32_handler_exception(%u, %u)\n", nr, code);

  assert(ia32_context_ring0_stack() == ERROR_OK);

  if (event_exist(id) == ERROR_TRUE)
    {
      assert(event_get(id, &o) == ERROR_OK);

      /* XXX ici, pour ne pas perdre le code, il faudrait le stocker dans
       * une variable locale du thread.
      if (o->type == EVENT_TYPE_FUNCTION)
	IA32_CALL_HANDLER(o->handler, id, o->data, code);
      else
      */

      i_thread		ith;
      o_thread*		oth;

      assert(thread_current(&ith) == ERROR_OK);
      assert(thread_get(ith, &oth) == ERROR_OK);

      oth->machine.error = code;

      assert(event_notify(id) == ERROR_OK);
    }
  else
    {
      spurious_interrupt(id);
    }

  assert(thread_current(&current) == ERROR_OK);

  //printf("[XXX] /ia32_handler_exception(%u, %u)\n", nr, code);
}

/*
 * this is the second handler called on irq.  it sends the EOI and
 * call the user-defined handler.
 */

void			ia32_handler_irq(t_uint32			nr)
{
  o_event*		o;
  i_event		id = IA32_IDT_IRQ_BASE + nr;
  i_thread		current;

  //printf("[XXX] ia32_handler_irq()\n");

  assert(ia32_context_ring0_stack() == ERROR_OK);

  if (event_exist(id) == ERROR_TRUE)
    {
      assert(event_get(id, &o) == ERROR_OK);

      /* XXX a virer
      if (o->type == EVENT_TYPE_FUNCTION)
	{
	  IA32_CALL_HANDLER(o->handler, id, o->data);

	}
      else
      */

      assert(event_notify(id) == ERROR_OK);

      // XXX rien a foutre la!
      platform_pic_acknowledge(nr);
    }
  else
    {
      spurious_interrupt(id);
    }

  assert(thread_current(&current) == ERROR_OK);

  //printf("[XXX] /ia32_handler_irq() -> future thread %qd\n", current);
}

/*
 * this is the second handler called on ipi.  it sends the EOI and
 * call the user-defined handler.
 */

void			ia32_handler_ipi(t_uint32			nr)
{
  o_event*		o;
  i_event		id = IA32_IDT_IPI_BASE + nr;
  i_thread		current;

  //printf("[XXX] ia32_handler_ipi(%u)\n", nr);

  assert(ia32_context_ring0_stack() == ERROR_OK);

  ia32_ipi_acknowledge();

  if (event_exist(id) == ERROR_TRUE)
    {
      assert(event_get(id, &o) == ERROR_OK);

      /* XXX
      if (o->type == EVENT_TYPE_FUNCTION)
	IA32_CALL_HANDLER(o->handler, id, o->data);
      else
      */

      assert(event_notify(id) == ERROR_OK);
    }
  else
    {
      spurious_interrupt(id);
    }

  assert(thread_current(&current) == ERROR_OK);

  //printf("[XXX] /ia32_handler_ipi(%u)\n", nr);
}

/*
 * this is the second handler called on syscall.  it calls the
 * user-defined handler.
 */

void			ia32_handler_syscall(t_uint32			nr)
{
  o_event*		o;
  i_event		id = IA32_IDT_SYSCALL_BASE + nr;
  i_thread		current;

  assert(ia32_context_ring0_stack() == ERROR_OK);

  if (event_exist(id) == ERROR_TRUE)
    {
      assert(event_get(id, &o) == ERROR_OK);

      /* XXX
      if (o->type == EVENT_TYPE_FUNCTION)
	IA32_CALL_HANDLER(o->handler, id, o->data);
      else
      */

      assert(event_notify(id) == ERROR_OK);
    }
  else
    {
      spurious_interrupt(id);
    }

  assert(thread_current(&current) == ERROR_OK);
}

/*
 * exception pre-handlers definitions
 */

IA32_EXCEPTION_PREHANDLER_NOCODE(0);
IA32_EXCEPTION_PREHANDLER_NOCODE(1);
IA32_EXCEPTION_PREHANDLER_NOCODE(2);
IA32_EXCEPTION_PREHANDLER_NOCODE(3);
IA32_EXCEPTION_PREHANDLER_NOCODE(4);
IA32_EXCEPTION_PREHANDLER_NOCODE(5);
IA32_EXCEPTION_PREHANDLER_NOCODE(6);
IA32_EXCEPTION_PREHANDLER_NOCODE(7);
IA32_EXCEPTION_PREHANDLER_CODE(8);
IA32_EXCEPTION_PREHANDLER_NOCODE(9);
IA32_EXCEPTION_PREHANDLER_CODE(10);
IA32_EXCEPTION_PREHANDLER_CODE(11);
IA32_EXCEPTION_PREHANDLER_CODE(12);
IA32_EXCEPTION_PREHANDLER_CODE(13);
IA32_EXCEPTION_PREHANDLER_CODE(14);
IA32_EXCEPTION_PREHANDLER_NOCODE(15);
IA32_EXCEPTION_PREHANDLER_NOCODE(16);
IA32_EXCEPTION_PREHANDLER_CODE(17);
IA32_EXCEPTION_PREHANDLER_NOCODE(18);
IA32_EXCEPTION_PREHANDLER_NOCODE(19);
IA32_EXCEPTION_PREHANDLER_NOCODE(20);
IA32_EXCEPTION_PREHANDLER_NOCODE(21);
IA32_EXCEPTION_PREHANDLER_NOCODE(22);
IA32_EXCEPTION_PREHANDLER_NOCODE(23);
IA32_EXCEPTION_PREHANDLER_NOCODE(24);
IA32_EXCEPTION_PREHANDLER_NOCODE(25);
IA32_EXCEPTION_PREHANDLER_NOCODE(26);
IA32_EXCEPTION_PREHANDLER_NOCODE(27);
IA32_EXCEPTION_PREHANDLER_NOCODE(28);
IA32_EXCEPTION_PREHANDLER_NOCODE(29);
IA32_EXCEPTION_PREHANDLER_NOCODE(30);
IA32_EXCEPTION_PREHANDLER_NOCODE(31);

/*
 * irq pre-handlers definitions
 */

IA32_IRQ_PREHANDLER(0);
IA32_IRQ_PREHANDLER(1);
IA32_IRQ_PREHANDLER(2);
IA32_IRQ_PREHANDLER(3);
IA32_IRQ_PREHANDLER(4);
IA32_IRQ_PREHANDLER(5);
IA32_IRQ_PREHANDLER(6);
IA32_IRQ_PREHANDLER(7);
IA32_IRQ_PREHANDLER(8);
IA32_IRQ_PREHANDLER(9);
IA32_IRQ_PREHANDLER(10);
IA32_IRQ_PREHANDLER(11);
IA32_IRQ_PREHANDLER(12);
IA32_IRQ_PREHANDLER(13);
IA32_IRQ_PREHANDLER(14);
IA32_IRQ_PREHANDLER(15);

/*
 * ipi prehandlers
 */

IA32_IPI_PREHANDLER(0);
IA32_IPI_PREHANDLER(1);
IA32_IPI_PREHANDLER(2);
IA32_IPI_PREHANDLER(3);
IA32_IPI_PREHANDLER(4);
IA32_IPI_PREHANDLER(5);
IA32_IPI_PREHANDLER(6);
IA32_IPI_PREHANDLER(7);

/*
 * syscall prehandlers
 */

IA32_SYSCALL_PREHANDLER(0);
IA32_SYSCALL_PREHANDLER(1);
IA32_SYSCALL_PREHANDLER(2);
IA32_SYSCALL_PREHANDLER(3);
IA32_SYSCALL_PREHANDLER(4);
IA32_SYSCALL_PREHANDLER(5);
IA32_SYSCALL_PREHANDLER(6);
IA32_SYSCALL_PREHANDLER(7);
IA32_SYSCALL_PREHANDLER(8);
IA32_SYSCALL_PREHANDLER(9);
