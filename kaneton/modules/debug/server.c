
/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          kaneton/kaneton/modules/debug/server.c
 *
 * created       julio guerra   [mon sept  15 13:45:13 2013]
 * updated       julio guerra   [web sept  22 17:18:37 2013]
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

extern const t_uint32           _dbg_handler_len;
extern const s_dbg_command      _dbg_handler[];
extern s_dbg_manager            _dbg;

/*
 * ---------- functions -------------------------------------------------------
 */

static e_dbg_error dbg_packet_read(void)
{
  t_uint8       c;
  e_dbg_error   e;

  dbg_read_start();

  do
  {
    dbg_read_char(&c);
  } while (c != DBG_TK_TERMINATE);

  e = dbg_read_checksum();

  _dbg.io.rx.cursor = 0;

  return (e);
}

e_dbg_error dbg_packet_send(void)
{
  _dbg.io.write(_dbg.io.tx.buffer, _dbg.io.tx.length);

  return E_NONE;
}

e_dbg_error dbg_ack(t_boolean flag)
{
  _dbg.io.write((t_uint8*) (flag ? "+" : "-"), 1);
  return E_NONE;
}


/*
 * Releasing execution means continuing the execution. The client is thus
 * waiting for the Ack answer and the stop reason
 */
static void dbg_server_resume(void)
{
  _dbg.release = 0;

  dbg_ack(1);

  dbg_write_start();
  dbg_write_str((t_uint8*) "S");
  dbg_write_uint8(_dbg.stop_reason);
  dbg_write_terminate();

  dbg_packet_send();
}

e_dbg_error dbg_server(void)
{
  unsigned int  i;
  size_t        len;

  if (_dbg.release)
    dbg_server_resume();

  do
  {
    if (dbg_packet_read() == E_DENY)
    {
      dbg_ack(0);
      continue;
    }

    for (i = 0; i < _dbg_handler_len; ++i)
    {
      len = strlen((char*) _dbg_handler[i].command);
      if (!strncmp((char*) _dbg_handler[i].command,
                   (char*) _dbg.io.rx.buffer,
                   len))
      {
        _dbg.io.rx.cursor += len;
        _dbg_handler[i].handler();
        break;
      }
    }

    if (i == _dbg_handler_len)
    {
      dbg_ack(1);
      dbg_write_start();
      dbg_write_terminate();
      dbg_packet_send();
    }
  } while (!_dbg.release);

  return E_NONE;
}

t_dbg_checksum dbg_packet_checksum(const t_uint8* data, t_uint32 len)
{
  t_dbg_checksum checksum = 0;
  unsigned int i;

  /* unsigned overflow is a well defined behavior */
  for (i = 0; i < len; ++i)
    checksum += data[i];

  return (checksum);
}
