/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/libs/libia32/include/interrupt/idt.h
 *
 * created       renaud voltz   [fri feb 10 16:36:20 2006]
 * updated       renaud voltz   [fri feb 10 16:36:20 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * protected mode structures and defines.
 *
 */

#ifndef IA32_PMODE_IDT_H
#define IA32_PMODE_IDT_H	1

/*                                                                  [cut] k3 */

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "../misc/types.h"

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * misc
 */

#define IDT_MAX_ENTRIES		256

#define IDT_EXCEPTION_BASE	0
#define IDT_IRQ_BASE		32

/*
 * gate types (for idt entries)
 */

#define GATE_TYPE_TASK          ((1 << 3) | (1 << 2))
#define GATE_TYPE_TRAP          ((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0))
#define GATE_TYPE_INTERRUPT     ((1 << 3) | (1 << 2) | (1 << 1))

/*
 * gate type
 */

#define IDT_GET_TYPE(Type)						\
((Type) & ((1 << 4) | (1 << 3) | (1 << 2) | (1 << 1) | (1 <<  0)))

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * gates types
 */
typedef enum
  {
    type_gate_task = GATE_TYPE_TASK,
    type_gate_interrupt = GATE_TYPE_INTERRUPT,
    type_gate_trap = GATE_TYPE_TRAP,
  }		t_gate_type;

/*
 * abstract gate descriptor
 */

typedef struct
{
  t_uint32			offset;
  t_uint16			segsel;
  t_prvl			privilege;
  t_gate_type			type;
}				t_gate;

/*
 * idt entry
 */

typedef struct
{
  t_uint16			offset_00_15;
  t_uint16			segsel;
  t_uint8			reserved;
  t_uint8			type;
  t_uint16			offset_16_31;
}				__attribute__ ((packed)) t_idte;

/*
 * idt
 */

typedef struct
{
  t_idte			*descriptor;
  t_uint16			count;
}				t_idt;

/*
 * idt register
 */

typedef struct
{
  t_uint16			size;
  t_uint32			address;
}				__attribute__ ((packed)) t_idtr;

/*                                                                 [cut] /k3 */

#endif
