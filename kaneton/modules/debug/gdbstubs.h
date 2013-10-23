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
 * Maximum Number of SW Breakpoints
 */
# define DBG_BREAKPOINTS_NB     255

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
# define DBG_TK_COMMA           ((t_uint8) ',')
# define DBG_TK_SEMICOLON       ((t_uint8) ';')
# define DBG_TK_COLON           ((t_uint8) ':')
# define DBG_TK_ESCAPE          ((t_uint8) '}')
# define DBG_TK_EQUAL           ((t_uint8) '=')

/*
 * Escaped Character xor operand
 */
# define DBG_ESCAPE_XOR          0x20

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
    f_dbg_io                            read;
    f_dbg_io                            write;
    struct
    {
        t_uint8  buffer[DBG_RX_MAX_SZ];
        t_uint32 length;
        t_uint32 cursor;
    }                                   rx;
    struct
    {
        t_uint8  buffer[DBG_TX_MAX_SZ];
        t_uint32 length;
    }                                   tx;
}                                       s_dbg_com;

enum e_dbg_register
{
  REG_EAX,
  REG_ECX,
  REG_EDX,
  REG_EBX,
  REG_ESP,
  REG_EBP,
  REG_ESI,
  REG_EDI,
  REG_EIP,
  REG_EFLAGS,
  REG_CS,
  REG_SS,
  REG_DS,
  REG_ES,
  REG_FS,
  REG_GS,
  REG_ST0,
  REG_ST1,
  REG_ST2,
  REG_ST3,
  REG_ST4,
  REG_ST5,
  REG_ST6,
  REG_ST7,
  REG_FCTRL,
  REG_FSTAT,
  REG_FTAG,
  REG_FISEG,
  REG_FIOFF,
  REG_FOSEG,
  REG_FOOFF,
  REG_FOP,
  REG_XMM0,
  REG_XMM1,
  REG_XMM2,
  REG_XMM3,
  REG_XMM4,
  REG_XMM5,
  REG_XMM6,
  REG_XMM7,
  REG_MXCSR
};

/*
 * SW Breakpoint
 * Save instruction opcode before replacing them by the SW breakpoint
 * opcode int3
 */
typedef struct
{
    t_uint8* address;
    t_uint8  opcode;
} s_dbg_breakpoint;

/*
 * Debug Server Manager Structure
 */
typedef struct
{
    enum e_dbg_stop_reason      stop_reason;
    s_dbg_com                   io;
    o_thread*                   thread;
    t_boolean                   release;
    s_dbg_breakpoint            bp[DBG_BREAKPOINTS_NB];
}                               s_dbg_manager;

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
 * server.c
 */

e_dbg_error dbg_server(void);

e_dbg_error dbg_ack(t_boolean flag);

e_dbg_error dbg_packet_send(void);

t_dbg_checksum dbg_packet_checksum(const t_uint8* data, t_uint32 len);

/*
 * write.c
 */

e_dbg_error dbg_write_uint32(t_uint32   x);

e_dbg_error dbg_write_data(t_uint8* b, t_uint32 len);

e_dbg_error dbg_write_start(void);

e_dbg_error dbg_write_terminate(void);

e_dbg_error dbg_write_str(t_uint8*      str);

e_dbg_error dbg_write_uint8(t_uint8   x);

/*
 * read.c
 */

e_dbg_error dbg_read_checksum(void);

e_dbg_error dbg_read_start(void);

e_dbg_error dbg_read_char(t_uint8* c);

/*
 * parse.c
 */

e_dbg_error dbg_parse_uint32_hstr(t_uint32* x);

e_dbg_error dbg_parse_comma(void);

e_dbg_error dbg_parse_uint32(t_uint32* x);

e_dbg_error dbg_parse_colon(void);

e_dbg_error dbg_parse_equal(void);

e_dbg_error dbg_parse_uint8_bin(t_uint8* byte);

e_dbg_error dbg_parse_data_hstr(t_uint8* val, t_uint32 len);

/*
 * eop
 */

#endif /* !MODULES_DEBUG_GDBSTUBS_H */
