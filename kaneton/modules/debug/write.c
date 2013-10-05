/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          kaneton/kaneton/modules/debug/write.c
 *
 * created       julio guerra   [mon sept  15 13:55:27 2013]
 * updated       julio guerra   [fri sept  20 23:15:14 2013]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the write functions to the transmission buffer.
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

e_dbg_error dbg_write_uint32(t_uint32   x)
{
  assert(_dbg.io.tx.length + 9 <= DBG_TX_MAX_SZ);

  uint32_to_hstr(x, &_dbg.io.tx.buffer[_dbg.io.tx.length]);
  _dbg.io.tx.length += 8;

  _dbg.io.tx.buffer[_dbg.io.tx.length] = 0;

  return E_NONE;
}

e_dbg_error dbg_write_uint8(t_uint8   x)
{
  assert(_dbg.io.tx.length + 2 <= DBG_TX_MAX_SZ);

  uint8_to_hstr(x, &_dbg.io.tx.buffer[_dbg.io.tx.length]);
  _dbg.io.tx.length += 2;

  _dbg.io.tx.buffer[_dbg.io.tx.length] = 0;

  return E_NONE;
}

e_dbg_error dbg_write_str(t_uint8*      str)
{
  while (*str)
    _dbg.io.tx.buffer[_dbg.io.tx.length++] = *(str++);

  _dbg.io.tx.buffer[_dbg.io.tx.length] = 0;

  return E_NONE;
}

e_dbg_error dbg_write_start(void)
{
  _dbg.io.tx.length = 1;
  _dbg.io.tx.buffer[0] = DBG_TK_START;
  _dbg.io.tx.buffer[1] = 0;
  return E_NONE;
}

e_dbg_error dbg_write_terminate(void)
{
  t_dbg_checksum checksum = dbg_packet_checksum(&_dbg.io.tx.buffer[1],
                                                _dbg.io.tx.length);

  _dbg.io.tx.buffer[_dbg.io.tx.length++] = DBG_TK_TERMINATE;

  uint8_to_hstr(checksum, &_dbg.io.tx.buffer[_dbg.io.tx.length]);

  _dbg.io.tx.length += 2;

  _dbg.io.tx.buffer[_dbg.io.tx.length] = 0;

  return E_NONE;
}

e_dbg_error dbg_write_data(t_uint8* b, t_uint32 len)
{
  unsigned int i;
  assert(_dbg.io.tx.length + len <= DBG_TX_MAX_SZ);

  for (i = 0; i < len; ++i)
  {
    uint8_to_hstr(b[i], &_dbg.io.tx.buffer[_dbg.io.tx.length]);
    _dbg.io.tx.length += 2;
  }

  _dbg.io.tx.buffer[_dbg.io.tx.length] = 0;

  return E_NONE;
}
