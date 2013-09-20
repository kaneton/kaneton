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
 * Maximum request packet size (supported)
 */
# define DBG_RX_MAX_SZ          4096

/*
 * Maximum response packet size (supported)
 */
# define DBG_TX_MAX_SZ          1024

/*
 * GDB packet tokens
 */
# define DBG_TK_START           ((t_uint8) '$')
# define DBG_TK_TERMINATE       ((t_uint8) '#')

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * Stop reason numeric values
 */
enum e_dbg_stop_reason
{
  DBG_STOP_TRAP = 5 /* Breakpoints */
};

/*
 * IO Buffers
 */
typedef struct
{
    struct
    {
        t_uint8  buffer[DBG_RX_MAX_SZ];
        t_uint32 length;
    }                                   rx;
    struct
    {
        t_uint8  buffer[DBG_TX_MAX_SZ];
        t_uint32 length;
    }                                   tx;
}                                       s_dbg_com;

/*
 * Debug Server Manager Structure
 */
typedef struct
{
    enum e_dbg_stop_reason      stop_reason;
    s_dbg_com                   io;
}                               s_dbg_manager;


/*
 * Parser helper :
 *   - the read packet
 *   - pointer to the packet, incremented as it is parsed
 */
typedef struct
{
    t_uint8     packet[DBG_RX_MAX_SZ];
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

e_dbg_error dbg_server(void);

e_dbg_error dbg_ack(t_boolean flag);

e_dbg_error dbg_packet_send(void);

t_dbg_checksum dbg_packet_checksum(const t_uint8* data);

/*
 * write.c
 */

e_dbg_error dbg_write_uint32(t_uint32   x);

e_dbg_error dbg_write_start(void);

e_dbg_error dbg_write_terminate(void);

e_dbg_error dbg_write_str(t_uint8*      str);

/*
 * eop
 */

#endif /* !MODULES_DEBUG_GDBSTUBS_H */
