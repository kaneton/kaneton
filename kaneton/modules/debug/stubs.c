/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          kaneton/kaneton/modules/debug/stubs.c
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
static e_dbg_error dbg_handler_setregister(void);
static e_dbg_error dbg_handler_memread(void);
static e_dbg_error dbg_handler_binwrite(void);
static e_dbg_error dbg_handler_continue(void);
static e_dbg_error dbg_handler_singlestep(void);
static e_dbg_error dbg_handler_setbreakpoint(void);
static e_dbg_error dbg_handler_delbreakpoint(void);

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * Associative array of command strings and their handlers
 */
const s_dbg_command _dbg_handler[] =
{
  { (t_uint8*) "?", dbg_handler_whystop },
  { (t_uint8*) "g", dbg_handler_getregisters },
  { (t_uint8*) "m", dbg_handler_memread },
  { (t_uint8*) "X", dbg_handler_binwrite },
  { (t_uint8*) "c", dbg_handler_continue },
  { (t_uint8*) "s", dbg_handler_singlestep },
  { (t_uint8*) "P", dbg_handler_setregister },
  { (t_uint8*) "Z0", dbg_handler_setbreakpoint },
  { (t_uint8*) "z0", dbg_handler_delbreakpoint }
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
  as_context*           ctx;
  as_tss*               tss;

  dbg_ack(1);


  ctx = (as_context*) _dbg.thread->machine.context;
  tss = (as_tss*) _thread.machine.tss;

  dbg_write_start();

  /* gdb/gdb/regformats/i386/i386.dat */

  /* target byte order required (little endian) */

  dbg_write_data((t_uint8*) &ctx->eax, 4);
  dbg_write_data((t_uint8*) &ctx->ecx, 4);
  dbg_write_data((t_uint8*) &ctx->edx, 4);
  dbg_write_data((t_uint8*) &ctx->ebx, 4);
  dbg_write_data((t_uint8*) &ctx->_esp, 4);
  dbg_write_data((t_uint8*) &ctx->ebp, 4);
  dbg_write_data((t_uint8*) &ctx->esi, 4);
  dbg_write_data((t_uint8*) &ctx->edi, 4);
  dbg_write_data((t_uint8*) &ctx->eip, 4);
  dbg_write_data((t_uint8*) &ctx->eflags, 4);
  dbg_write_data((t_uint8*) &ctx->cs, 4);
  dbg_write_data((t_uint8*) &ctx->ss, 4);
  dbg_write_data((t_uint8*) &tss->ds, 4);
  dbg_write_data((t_uint8*) &tss->es, 4);
  dbg_write_data((t_uint8*) &tss->fs, 4);
  dbg_write_data((t_uint8*) &tss->gs, 4);

  /* st0 */
  dbg_write_str((t_uint8*) "xxxxxxxxxxxxxxxxxxxx");
  /* st1 */
  dbg_write_str((t_uint8*) "xxxxxxxxxxxxxxxxxxxx");
  /* st2 */
  dbg_write_str((t_uint8*) "xxxxxxxxxxxxxxxxxxxx");
  /* st3 */
  dbg_write_str((t_uint8*) "xxxxxxxxxxxxxxxxxxxx");
  /* st4 */
  dbg_write_str((t_uint8*) "xxxxxxxxxxxxxxxxxxxx");
  /* st5 */
  dbg_write_str((t_uint8*) "xxxxxxxxxxxxxxxxxxxx");
  /* st6 */
  dbg_write_str((t_uint8*) "xxxxxxxxxxxxxxxxxxxx");
  /* st7 */
  dbg_write_str((t_uint8*) "xxxxxxxxxxxxxxxxxxxx");
  /* fctrl */
  dbg_write_str((t_uint8*) "xxxxxxxx");
  /* fstat */
  dbg_write_str((t_uint8*) "xxxxxxxx");
  /* ftag */
  dbg_write_str((t_uint8*) "xxxxxxxx");
  /* fiseg */
  dbg_write_str((t_uint8*) "xxxxxxxx");
  /* fioff */
  dbg_write_str((t_uint8*) "xxxxxxxx");
  /* foseg */
  dbg_write_str((t_uint8*) "xxxxxxxx");
  /* fooff */
  dbg_write_str((t_uint8*) "xxxxxxxx");
  /* fop */
  dbg_write_str((t_uint8*) "xxxxxxxx");
  /* xmm0 */
  dbg_write_str((t_uint8*) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  /* xmm1 */
  dbg_write_str((t_uint8*) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  /* xmm2 */
  dbg_write_str((t_uint8*) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  /* xmm3 */
  dbg_write_str((t_uint8*) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  /* xmm4 */
  dbg_write_str((t_uint8*) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  /* xmm5 */
  dbg_write_str((t_uint8*) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  /* xmm6 */
  dbg_write_str((t_uint8*) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  /* xmm7 */
  dbg_write_str((t_uint8*) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  /* mxcsr */
  dbg_write_str((t_uint8*) "xxxxxxxx");

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

static e_dbg_error dbg_handler_memread(void)
{
  t_uint32  address;
  t_uint32  size;
  t_boolean is_mapped;

  dbg_ack(1);

  if (dbg_parse_uint32_hstr(&address) != E_NONE
      || dbg_parse_comma() != E_NONE
      || dbg_parse_uint32_hstr(&size) != E_NONE)
    return E_PARSE;

  dbg_write_start();

  // TODO : check access rights
  architecture_paging_test(address, &is_mapped);
  if (!is_mapped)
    dbg_write_str((t_uint8*) "E03");
  else
    dbg_write_data((t_uint8*) address, size);

  dbg_write_terminate();

  dbg_packet_send();

  return E_NONE;
}

static e_dbg_error dbg_handler_binwrite(void)
{
  t_uint8*      address;
  t_uint8       data;
  t_uint32      size;
  e_dbg_error   e = E_NONE;

  dbg_ack(1);

  if (dbg_parse_uint32_hstr((t_uint32*) &address) != E_NONE
      || dbg_parse_comma() != E_NONE
      || dbg_parse_uint32_hstr(&size) != E_NONE
      || dbg_parse_colon() != E_NONE)
    return E_PARSE;

  // TODO : check access rights

  for (; size; --size)
  {
    e = dbg_parse_uint8_bin(&data);
    if (e != E_NONE)
      break;

    *(address++) = data;
  }

  dbg_write_start();

  if (e != E_NONE)
    dbg_write_str((t_uint8*) "E01");
  else
    dbg_write_str((t_uint8*) "OK");

  dbg_write_terminate();

  dbg_packet_send();

  return e;
}

/*
 * Continue execution
 */
static e_dbg_error dbg_handler_continue(void)
{
  _dbg.release = 1;

  return E_NONE;
}

/*
 * Continue execution by a single step
 */
static e_dbg_error dbg_handler_singlestep(void)
{
  as_context* ctx = (as_context*) _dbg.thread->machine.context;

  ctx->eflags |= ARCHITECTURE_REGISTER_EFLAGS_TF;
  ctx->eflags |= ARCHITECTURE_REGISTER_EFLAGS_RF;

  _dbg.release = 1;

  return E_NONE;
}

/*
 * Continue execution
 */
static e_dbg_error dbg_handler_setregister(void)
{
  as_context*   ctx;
  t_uint32      reg;
  t_uint32      val;

  dbg_ack(1);

  if (dbg_parse_uint32_hstr(&reg) != E_NONE
      || dbg_parse_equal() != E_NONE
      || dbg_parse_data_hstr((t_uint8*) &val, sizeof (val)) != E_NONE)
    return E_PARSE;

  ctx = (as_context*) _dbg.thread->machine.context;

  if (reg == 8)
    ctx->eip = val;

  dbg_write_start();
  dbg_write_str((t_uint8*) "OK");
  dbg_write_terminate();
  dbg_packet_send();

  return E_NONE;
}

static e_dbg_error dbg_handler_setbreakpoint(void)
{
  e_dbg_error  e = E_NONE;
  unsigned int i;
  t_uint8*     address = 0;

  dbg_ack(1);

  if (dbg_parse_comma() != E_NONE
      || dbg_parse_uint32_hstr((t_uint32*) &address) != E_NONE)
    return E_PARSE;

  // TODO : check access rights

  for (i = 0; i < DBG_BREAKPOINTS_NB && _dbg.bp[i].address != 0; ++i)
    ;

  if (i == DBG_BREAKPOINTS_NB)
    e = E_BPFULL;
  else
  {
    _dbg.bp[i].opcode = *address;
    _dbg.bp[i].address = address;
    *address = 0xCC;
  }

  // TODO : invalidate icache address

  dbg_write_start();

  if (e != E_NONE)
    dbg_write_str((t_uint8*) "E01");
  else
    dbg_write_str((t_uint8*) "OK");

  dbg_write_terminate();

  dbg_packet_send();

  return e;
}

static e_dbg_error dbg_handler_delbreakpoint(void)
{
  t_uint8*      address;
  e_dbg_error   e = E_NONE;
  unsigned int  i;
  unsigned int  j;

  dbg_ack(1);

  if (dbg_parse_comma() != E_NONE
      || dbg_parse_uint32_hstr((t_uint32*) &address) != E_NONE)
    return E_PARSE;

  // TODO : check access rights

  for (i = 0; i < DBG_BREAKPOINTS_NB && _dbg.bp[i].address != address; ++i)
    ;

  if (i == DBG_BREAKPOINTS_NB)
    e = E_NOBP;
  else
  {
    *address = _dbg.bp[i].opcode;
    _dbg.bp[i].address = 0;
  }

  // TODO : invalidate icache address

  dbg_write_start();

  if (e != E_NONE)
    dbg_write_str((t_uint8*) "E01");
  else
    dbg_write_str((t_uint8*) "OK");

  dbg_write_terminate();

  dbg_packet_send();

  return e;
}
