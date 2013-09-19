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
 * Default IO communication functions
 */
s_dbg_io_interface _dbg_io =
{
  .read  = dbg_serial_read,
  .write = dbg_serial_write
};

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function loads the module.
 */
t_error                 module_debug_load(void)
{
  module_call(console, message,
              '+', "loading the 'debug' module\n");

  MODULE_LEAVE();
}

/*
 * this function unloads the module.
 */
t_error                 module_debug_unload(void)
{
  module_call(console, message,
              '+', "unloading the 'debug' module\n");

  MODULE_LEAVE();
}

static void debug_exception_bp(i_event n, t_data d)
{
  module_call(console, message, '+', "bp exception :)\n");
}

static void debug_exception_db(i_event n, t_data d)
{
  module_call(console, message, '+', "DB exception\n");
}

/*
 * Start the gdb server
 */
t_error                 module_debug_start(void)
{
  event_reserve(ARCHITECTURE_IDT_EXCEPTION_BP,
                EVENT_TYPE_FUNCTION,
                EVENT_ROUTINE(debug_exception_bp),
                0);

  event_reserve(ARCHITECTURE_IDT_EXCEPTION_DB,
                EVENT_TYPE_FUNCTION,
                EVENT_ROUTINE(debug_exception_db),
                0);

  /* asm volatile ("movl %0, %%eax;"					\ */
  /*       	"movl %%eax, %%dr0"					\ */
  /*               :							\ */
  /*               : "r" (&f)); */

  /* asm volatile ("movl %0, %%eax;"					\ */
  /*       	"movl %%eax, %%dr7"					\ */
  /*               :							\ */
  /*               : "i" (0x00000401)); */

  for (;;)
    dbg_parse();

  MODULE_LEAVE();
}


/*
 * IO communication functions setter.
 * Serial driver by default
 */
void module_debug_set_io_cfg(const s_dbg_io_interface* cfg)
{
  assert(cfg);

  _dbg_io.read = cfg->read;
  _dbg_io.write = cfg->write;
}
