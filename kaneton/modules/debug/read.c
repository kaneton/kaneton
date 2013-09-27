/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          kaneton/kaneton/modules/debug/read.c
 *
 * created       julio guerra   [mon sept  15 13:55:27 2013]
 * updated       julio guerra   [fri sept  22 16:30:27 2013]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the read functions to the transmission buffer.
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
 * ---------- functions -------------------------------------------------------
 */

e_dbg_error dbg_read_char(t_uint8* c)
{
  assert(c);
  assert(_dbg.io.rx.length + 1 <= DBG_RX_MAX_SZ);

  _dbg.io.read(c, 1);

  if (*c == DBG_TK_TERMINATE)
    _dbg.io.rx.buffer[_dbg.io.rx.length] = 0;
  else
  {
    _dbg.io.rx.buffer[_dbg.io.rx.length] = *c;
    _dbg.io.rx.buffer[++_dbg.io.rx.length] = 0;
  }

  return E_NONE;
}

e_dbg_error dbg_read_start(void)
{
  t_uint8 tmp;

  do
  {
    _dbg.io.read(&tmp, 1);
  } while (tmp != DBG_TK_START);

  _dbg.io.rx.length = 0;
  _dbg.io.rx.buffer[0] = 0;

  return E_NONE;
}

e_dbg_error dbg_read_checksum(void)
{
  t_uint8 checksum[3] = { 0, 0, 0 };

  _dbg.io.read(checksum, 2);

  if (dbg_packet_checksum(_dbg.io.rx.buffer,
                          _dbg.io.rx.length) != hstr_to_uint8(checksum))
    return E_DENY;

  return E_NONE;
}
