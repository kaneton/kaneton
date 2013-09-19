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

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * Associative array of command strings and their handlers
 */
const s_dbg_command _dbg_handler[] =
{
  { (t_uint8*) "H", dbg_handler_H }
};

/*
 * Debug handler associative array length
 */
const t_uint32 _dbg_handler_len =
  sizeof (_dbg_handler) / sizeof (*_dbg_handler);

/*
 * ---------- functions -------------------------------------------------------
 */

e_dbg_error dbg_handler_H(void)
{
  dbg_ack(1);
  dbg_packet_send((t_uint8*) "OK");

  return E_NONE;
}
