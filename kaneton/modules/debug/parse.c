/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          kaneton/kaneton/modules/debug/parse.c
 *
 * created       julio guerra   [sun sept  22 19:14:18 2013]
 * updated       julio guerra   [sun sept  22 19:16:33 2013]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements parsing functions of read data.
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

static t_boolean dbg_parse_is_separator(t_uint8 c)
{
  return (c == DBG_TK_COMMA || c == DBG_TK_SEMICOLON || c == DBG_TK_COLON
          || c == DBG_TK_EQUAL);
}

/*
 * Parse an hexadecimal string of an uint32
 */
e_dbg_error dbg_parse_uint32_hstr(t_uint32* x)
{
  unsigned int  i = 0;
  t_uint8*      hstr = &_dbg.io.rx.buffer[_dbg.io.rx.cursor];
  t_uint8       separator;

  for (i = 0;
       i + _dbg.io.rx.cursor < _dbg.io.rx.length
         && !dbg_parse_is_separator(hstr[i]);
       ++i)
    ;

  separator = hstr[i];
  hstr[i] = 0;
  *x = hstr_to_uint32(hstr);
  hstr[i] = separator;

  _dbg.io.rx.cursor += i;

  return E_NONE;
}

/*
 * Parse a string of an uint32
 */
e_dbg_error dbg_parse_uint32(t_uint32* x)
{
  unsigned int  i = 0;
  t_uint8*      hstr = &_dbg.io.rx.buffer[_dbg.io.rx.cursor];
  t_uint8       separator;

  for (i = 0;
       i + _dbg.io.rx.cursor < _dbg.io.rx.length
         && !dbg_parse_is_separator(hstr[i]);
       ++i)
    ;

  separator = hstr[i];
  hstr[i] = 0;
  *x = str_to_uint32(hstr);
  hstr[i] = separator;

  _dbg.io.rx.cursor += i;

  return E_NONE;
}

/*
 * Parse a comma
 */
e_dbg_error dbg_parse_comma(void)
{
  if (_dbg.io.rx.buffer[_dbg.io.rx.cursor] != DBG_TK_COMMA)
    return E_PARSE;

  ++_dbg.io.rx.cursor;

  return E_NONE;
}

/*
 * Parse a colon
 */
e_dbg_error dbg_parse_colon(void)
{
  if (_dbg.io.rx.buffer[_dbg.io.rx.cursor] != DBG_TK_COLON)
    return E_PARSE;

  ++_dbg.io.rx.cursor;

  return E_NONE;
}

/*
 * Parse an equal token
 */
e_dbg_error dbg_parse_equal(void)
{
  if (_dbg.io.rx.buffer[_dbg.io.rx.cursor] != DBG_TK_EQUAL)
    return E_PARSE;

  ++_dbg.io.rx.cursor;

  return E_NONE;
}

/*
 * Parse binary byte
 * May be escaped.
 */
e_dbg_error dbg_parse_uint8_bin(t_uint8* byte)
{
  assert(byte);

  *byte = _dbg.io.rx.buffer[_dbg.io.rx.cursor];

  assert(_dbg.io.rx.cursor < _dbg.io.rx.length);

  if (*byte == DBG_TK_ESCAPE)
  {
    ++_dbg.io.rx.cursor;
    *byte = _dbg.io.rx.buffer[_dbg.io.rx.cursor] ^ DBG_ESCAPE_XOR;

    assert(_dbg.io.rx.cursor < _dbg.io.rx.length);
  }

  ++_dbg.io.rx.cursor;

  return E_NONE;
}

/*
 * Parse hexadecimal data (XXXXXXXX) into the value in byte order
 */
e_dbg_error dbg_parse_data_hstr(t_uint8* val, t_uint32 len)
{
  unsigned int i;

  assert(val);
  assert(_dbg.io.rx.cursor + len <= _dbg.io.rx.length);

  for (i = 0; i < len; ++i)
  {
    val[i] = hstr_to_uint8(&_dbg.io.rx.buffer[_dbg.io.rx.cursor]);
    _dbg.io.rx.cursor += 2;
  }

  return E_NONE;
}
