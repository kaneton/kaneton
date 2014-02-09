/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          kaneton/kaneton/modules/debug/include/debug.h
 *
 * created       julio guerra   [sun sept  8 13:49:44 2013]
 * updated       julio guerra   [sun sept  8 13:49:44 2013]
 */

#ifndef MODULES_DEBUG_DEBUG_H
#define MODULES_DEBUG_DEBUG_H	1

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * IO function
 * f(t_uint8* buffer*, t_uint32 size)
 */
typedef void (*f_dbg_io)(t_uint8*, t_uint32);

/*
 * IO communication driver interface
 */
typedef struct
{
    f_dbg_io    read;
    f_dbg_io    write;
}               s_dbg_io_interface;

/*
 * Module internal errors
 */
typedef enum
{
  E_NONE = 0,
  E_PARSE,
  E_DENY,       /* Bad checksum */
  E_NOACCESS,
  E_BPFULL,
  E_NOBP
} e_dbg_error;

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../debug.c
 */

/*
 * ../debug.c
 */

t_statusmodule_debug_load(void);

t_statusmodule_debug_unload(void);

t_status module_debug_start(void);

void    module_debug_set_io_cfg(const s_dbg_io_interface* cfg);

/*
 * eop
 */

#endif
