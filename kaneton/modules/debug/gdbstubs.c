/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          kaneton/kaneton/modules/debug/gdbstubs.c
 *
 * created       julio guerra   [mon sept  15 13:45:13 2013]
 * updated       julio guerra   [web sept  18 22:15:17 2013]
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

extern s_dbg_io_interface _dbg_io;
extern const t_uint32 _dbg_handler_len;
extern const s_dbg_command _dbg_handler[];

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * ---------- locals ----------------------------------------------------------
 */

/*
 * Parser helper object
 */
static s_dbg_parser _dbg_parser =
{
  .packet  = { 0 },
  .current = _dbg_parser.packet
};

/*
 * ---------- forward declarations --------------------------------------------
 */

static t_dbg_checksum dbg_packet_checksum(const t_uint8* data);

/*
 * ---------- functions -------------------------------------------------------
 */

static e_dbg_error dbg_packet_read(void)
{
  t_uint8       tmp;
  t_uint8       crc[3];

  do
  {
    _dbg_io.read(&tmp, 1);
  } while (tmp != DBG_TK_START);

  do
  {
    _dbg_io.read(&tmp, 1);
    if (tmp == DBG_TK_TERMINATE)
      break;
    *(_dbg_parser.current++) = tmp;
  } while (1);

  *_dbg_parser.current = 0;
  _dbg_parser.current = _dbg_parser.packet;

  _dbg_io.read(crc, 2);
  crc[2] = 0;
  if (dbg_packet_checksum(_dbg_parser.packet) != hex_to_uint8(crc))
    return E_PARSE;

  return E_NONE;
}

e_dbg_error dbg_packet_send(const t_uint8* response)
{
  static t_uint8 p[DBG_PACKET_MAX_SZ] = { DBG_TK_START };

  t_uint32       sz;

  for (sz = 0; response[sz]; ++sz)
    p[1 + sz] = response[sz];

  sz += 1;

  p[sz++] = DBG_TK_TERMINATE;

  uint8_to_str(dbg_packet_checksum(response), &p[sz]);

  sz += 2;

  _dbg_io.write(p, sz);

  return E_NONE;
}

e_dbg_error dbg_ack(t_boolean flag)
{
  _dbg_io.write((t_uint8*) (flag ? "+" : "-"), 1);
  return E_NONE;
}

e_dbg_error dbg_parse(void)
{
  unsigned int  i;
  size_t        len;

  dbg_packet_read();

  for (i = 0; i < _dbg_handler_len; ++i)
  {
    len = strlen((char*) _dbg_handler[i].command);
    if (!strncmp((char*) _dbg_handler[i].command, (char*) _dbg_parser.packet, len))
    {
      _dbg_handler[i].handler();
      break;
    }
  }

  if (i == _dbg_handler_len)
  {
    dbg_ack(1);
    dbg_packet_send((t_uint8*) "");
  }

  return E_NONE;
}

static t_dbg_checksum dbg_packet_checksum(const t_uint8* data)
{
  t_dbg_checksum crc = 0;
  unsigned int i;

  /* unsigned overflow is a well defined behavior */
  for (i = 0; data[i]; ++i)
    crc += data[i];

  return crc;
}
