/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/arch/ibm-pc.ia32-smp/cpu.c
 *
 * created       matthieu bucchianeri   [sat jul 29 18:04:01 2006]
 * updated       matthieu bucchianeri   [fri mar 16 23:06:32 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_cpu*	cpu;
extern t_init*	init;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the cpu manager dispatch.
 */

d_cpu		cpu_dispatch =
  {
    NULL,
    ia32_cpu_current,
    NULL,
    ia32_cpu_init,
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * identifies the  current cpu. since  non-mp, the current cpu  is the
 * boot processor.
 */

t_error			ia32_cpu_current(i_cpu*			cpuid)
{
  CPU_ENTER(cpu);

  *cpuid = init->bsp;

  CPU_LEAVE(cpu, ERROR_NONE);
}

/*
 * XXX
 */

static volatile t_uint32	cpu_ok = 1;

static SPIN_DECLARE(ia32_cpu_init_ap_lock);

t_error			ia32_cpu_init(void)
{
  t_ia32_gate		gate_mp_init;
  t_setsz		ncpus;

  if (set_size(cpu->cpus, &ncpus) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  gate_mp_init.type = ia32_type_gate_trap;
  gate_mp_init.privilege = 0;
  gate_mp_init.offset = (t_uint32)ia32_cpu_init_ap;
  gate_mp_init.segsel = PMODE_GDT_CORE_CS << 3;

  if (idt_add_gate(NULL, 56, gate_mp_init) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  ipi_send_vector(56, ipi_all_but_me, 0);

  while (cpu_ok != ncpus)
    ;

  // XXX send TLB flush

  printf("back to BSP\n");

  return (ERROR_NONE);
}

void			ia32_cpu_init_ap(void)
{
  static t_uint16	kcs;
  static t_uint16	kds;

  SPIN_LOCK(ia32_cpu_init_ap_lock);

  ipi_acknowledge();

  tlb_flush();

  gdt_build_selector(PMODE_GDT_CORE_CS, ia32_prvl_supervisor, &kcs);
  gdt_build_selector(PMODE_GDT_CORE_DS, ia32_prvl_supervisor, &kds);
  pmode_set_segment_registers(kcs, kds);

/*  if (tss_init(thread->machdep.tss) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);*/

  cpu_ok++;

  SPIN_UNLOCK(ia32_cpu_init_ap_lock);

//  HLT(); XXX why not working ?
  printf("Cpu %u ok\n", apic_id());
  while(1)
    ;
}
