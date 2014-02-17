/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          kaneton/kaneton/modules/debug/debug.c
 *
 * created       julio guerra   [sun sep 8 13:17:33 2013]
 * updated       julio guerra   [wed sep 18 21:29:01 2013]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * kernel debugger implementation
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "serial.h"
#include "gdbstubs.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * Debug Server Manager Structure
 */
s_dbg_manager _dbg;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function loads the module.
 */
t_status                 module_debug_load(void)
{
  module_call(console, message,
              '+', "loading the 'debug' module\n");

  memset(&_dbg, 0, sizeof (_dbg));

  _dbg.io.read = dbg_serial_read;
  _dbg.io.write = dbg_serial_write;

  MODULE_LEAVE();
}

/*
 * this function unloads the module.
 */
t_status                 module_debug_unload(void)
{
  module_call(console, message,
              '+', "unloading the 'debug' module\n");

  MODULE_LEAVE();
}

static void debug_exception_bp(i_event n, t_data d)
{
  i_thread		current;
  as_context*           ctx;

  _dbg.stop_reason = DBG_STOP_TRAP;

  // TODO : use a debugger's trap ISR and save the context in a debugger's
  // object
  thread_current(&current);
  thread_get(current, &_dbg.thread);

  ctx = (as_context*) _dbg.thread->machine.context;
  ctx->eflags &= ~ARCHITECTURE_REGISTER_EFLAGS_TF;

  dbg_server();
}

static void debug_exception_db(i_event n, t_data d)
{
  i_thread		current;
  as_context*           ctx;

  _dbg.stop_reason = DBG_STOP_TRAP;

  // TODO : use a debugger's trap ISR and save the context in a debugger's
  // object
  thread_current(&current);
  thread_get(current, &_dbg.thread);

  ctx = (as_context*) _dbg.thread->machine.context;
  ctx->eflags &= ~ARCHITECTURE_REGISTER_EFLAGS_TF;

  dbg_server();
}

/*
 * Start the gdb server
 */
t_status                 module_debug_start(void)
{
  if (set_reserve(bpt,
                  SET_OPTION_ALLOCATE | SET_OPTION_SORT,
                  sizeof (s_dbg_breakpoint),
                  1024,
                  &_dbg.bp) != STATUS_OK)
    MODULE_ESCAPE("unable to reserve the segments set");


  event_reserve(ARCHITECTURE_IDT_EXCEPTION_BP,
                EVENT_TYPE_FUNCTION,
                EVENT_ROUTINE(debug_exception_bp),
                0);

  event_reserve(ARCHITECTURE_IDT_EXCEPTION_DB,
                EVENT_TYPE_FUNCTION,
                EVENT_ROUTINE(debug_exception_db),
                0);

  module_call(console, message,
	      '+', "starting GDB server\n");

  ARCHITECTURE_INT3();

  /* asm volatile ("movl %0, %%eax;"					\ */
  /*       	"movl %%eax, %%dr0"					\ */
  /*               :							\ */
  /*               : "r" (&f)); */

  /* asm volatile ("movl %0, %%eax;"					\ */
  /*       	"movl %%eax, %%dr7"					\ */
  /*               :							\ */
  /*               : "i" (0x00000401)); */

  MODULE_LEAVE();
}


/*
 * IO communication functions setter.
 * Serial driver by default
 */
void module_debug_set_io_cfg(const s_dbg_io_interface* cfg)
{
  assert(cfg);

  _dbg.io.read = cfg->read;
  _dbg.io.write = cfg->write;
}
