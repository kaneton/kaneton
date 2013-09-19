/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          kaneton/kaneton/modules/debug/gdbstubs.h
 *
 * created       julio guerra   [mon sept  15 13:49:44 2013]
 * updated       julio guerra   [web sept  18 21:41:17 2013]
 */

#ifndef MODULES_DEBUG_GDBSTUBS_H
# define MODULES_DEBUG_GDBSTUBS_H 1

/*
 * ---------- includes --------------------------------------------------------
 */

# include "kaneton.h"

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * Maximum packet size (supported)
 */
# define DBG_PACKET_MAX_SZ      255

/*
 * GDB packet tokens
 */
# define DBG_TK_START           ((t_uint8) '$')
# define DBG_TK_TERMINATE       ((t_uint8) '#')

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * Parser helper :
 *   - the read packet
 *   - pointer to the packet, incremented as it is parsed
 */
typedef struct
{
    t_uint8     packet[DBG_PACKET_MAX_SZ];
    t_uint8*    current;
}               s_dbg_parser;

/*
 * GDB packet checksum
 * Sum of packet's characters module 256, hence an unsigned byte to
 * use the HW overflow.
 */
typedef t_uint8        t_dbg_checksum;

/*
 * GDB command handler.
 * The handler is both the command parser and executer.
 * New commands simply need to be added to the command handler array.
 */
typedef struct
{
    t_uint8*    command;
    e_dbg_error (*handler)(void);
}               s_dbg_command;

/*
 * ---------- prototypes ------------------------------------------------------
 */

/*
 * gdbstubs.c
 */

e_dbg_error dbg_parse(void);

e_dbg_error dbg_ack(t_boolean flag);

e_dbg_error dbg_packet_send(const t_uint8* response);

/*
 * gdbcommands.c
 */

e_dbg_error dbg_handler_H(void);

/*
 * eop
 */

#endif /* !MODULES_DEBUG_GDBSTUBS_H */
