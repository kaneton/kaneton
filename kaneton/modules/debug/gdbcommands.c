/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          kaneton/kaneton/modules/debug/gdbcommands.c
 *
 * created       julio guerra   [mon sept  15 13:55:27 2013]
 * updated       julio guerra   [web sept  18 22:37:57 2013]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the gdb stubs building-block functions to run a gdb
 * server.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "gdbstubs.h"
#include "misc.h"

/*
 * ---------- extern ----------------------------------------------------------
 */

extern s_dbg_manager    _dbg;


/*
 * thread manager.
 */

extern m_thread		_thread;

/*
 * ---------- forward declarations --------------------------------------------
 */

static e_dbg_error dbg_handler_whystop(void);
static e_dbg_error dbg_handler_getregisters(void);

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * Associative array of command strings and their handlers
 */
const s_dbg_command _dbg_handler[] =
{
  { (t_uint8*) "?", dbg_handler_whystop },
  { (t_uint8*) "g", dbg_handler_getregisters }
};

/*
 * Debug handler associative array length
 */
const t_uint32 _dbg_handler_len =
  sizeof (_dbg_handler) / sizeof (*_dbg_handler);

/*
 * ---------- functions -------------------------------------------------------
 */

static e_dbg_error      dbg_handler_getregisters(void)
{
  i_thread		current;
  o_thread*		thread;
  as_context*           ctx;
  as_tss*               tss;

  dbg_ack(1);

  if (thread_current(&current) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the current thread identifier");

  if (thread_get(current, &thread) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the thread object");

  ctx = (as_context*) thread->machine.context;
  tss = (as_tss*) _thread.machine.tss;

  dbg_write_start();

  /* include/gdb/gdbreg.def */

  dbg_write_uint32(ctx->eax);
  dbg_write_uint32(ctx->ecx);
  dbg_write_uint32(ctx->edx);
  dbg_write_uint32(ctx->ebx);
  dbg_write_uint32(ctx->_esp);
  dbg_write_uint32(ctx->ebp);
  dbg_write_uint32(ctx->esi);
  dbg_write_uint32(ctx->edi);
  dbg_write_uint32(ctx->eip);
  dbg_write_uint32(ctx->eflags);
  dbg_write_uint32(ctx->cs);
  dbg_write_uint32(ctx->ss);
  dbg_write_uint32(tss->ds);
  dbg_write_uint32(tss->es);
  dbg_write_uint32(tss->fs);
  dbg_write_uint32(tss->gs);

  /* st0 */
  dbg_write_uint32(0);
  /* st1 */
  dbg_write_uint32(0);
  /* st2 */
  dbg_write_uint32(0);
  /* st3 */
  dbg_write_uint32(0);
  /* st4 */
  dbg_write_uint32(0);
  /* st5 */
  dbg_write_uint32(0);
  /* st6 */
  dbg_write_uint32(0);
  /* st7 */
  dbg_write_uint32(0);
  /* fctrl */
  dbg_write_uint32(0);
  /* fstat */
  dbg_write_uint32(0);
  /* ftag */
  dbg_write_uint32(0);
  /* fiseg */
  dbg_write_uint32(0);
  /* fioff */
  dbg_write_uint32(0);
  /* foseg */
  dbg_write_uint32(0);
  /* fooff */
  dbg_write_uint32(0);
  /* fop */
  dbg_write_uint32(0);
  /* smxmm0"); */
  dbg_write_uint32(0);
  /* xmm1 */
  dbg_write_uint32(0);
  /* xmm2 */
  dbg_write_uint32(0);
  /* xmm3 */
  dbg_write_uint32(0);
  /* xmm4 */
  dbg_write_uint32(0);
  /* xmm5 */
  dbg_write_uint32(0);
  /* xmm6 */
  dbg_write_uint32(0);
  /* xmm7 */
  dbg_write_uint32(0);
  /* mxcsr */
  dbg_write_uint32(0);

  dbg_write_terminate();

  dbg_packet_send();

  return E_NONE;
}

static e_dbg_error dbg_handler_whystop(void)
{
  dbg_ack(1);
  dbg_write_start();
  dbg_write_str((t_uint8*) "S05");
  dbg_write_terminate();
  dbg_packet_send();
  return E_NONE;
}
