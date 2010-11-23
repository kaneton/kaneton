/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/include/pmode/pmode.h
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [tue feb  6 19:48:31 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * protected mode structures and macros.
 *
 */

#ifndef ARCHITECTURE_PMODE_H
#define ARCHITECTURE_PMODE_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * privilege levels
 */

#define IA32_PRIVILEGE_RING0		0
#define IA32_PRIVILEGE_RING1		1
#define IA32_PRIVILEGE_RING2		2
#define IA32_PRIVILEGE_RING3		3

/*
 * descriptor flags
 */

#define IA32_DESCRIPTOR_PRESENT		(1 << 7)

/*
 * descriptor DPL management
 */

#define IA32_DESCRIPTOR_SET_DPL(_privilege_)				\
  ((_privilege_) << 5)

#define IA32_DESCRIPTOR_GET_DPL(_type_)					\
  (((_type_) >> 5) & 0x3)

/*
 * segment selector defines
 */

/*
 * null segment selector index
 */

#define IA32_SEGMENT_SELECTOR_NULL	0

/*
 * segment selector table indicator
 */

#define IA32_SEGMENT_SELECTOR_TI_GDT	(0 << 2)
#define IA32_SEGMENT_SELECTOR_TI_LDT	(1 << 2)

/*
 * build a segment selector
 */

#define IA32_SEGMENT_SELECTOR(_index_, _rpl_)				\
  ((t_uint16)(((_index_) << 3) + (_rpl_)))

/*
 * segment selector requested privilege level
 */

#define IA32_SEGMENT_SELECTOR_RPL(_entry_)				\
  ((_entry_) << 0)

/*
 * segment selector index
 */

#define IA32_SEGMENT_SELECTOR_INDEX(_entry_)				\
  ((_entry_) << 3)

/*
 * gdt map for the four components: kaneton, driver, service, program
 */

#define IA32_PMODE_GDT_KANETON_CS	0x1
#define IA32_PMODE_GDT_KANETON_DS	0x2
#define IA32_PMODE_GDT_DRIVER_CS	0x3
#define IA32_PMODE_GDT_DRIVER_DS	0x4
#define IA32_PMODE_GDT_SERVICE_CS	0x5
#define IA32_PMODE_GDT_SERVICE_DS	0x6
#define IA32_PMODE_GDT_PROGRAM_CS	0x7
#define IA32_PMODE_GDT_PROGRAM_DS	0x8

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * privilege levels
 */

typedef enum
  {
    ia32_privilege_supervisor = IA32_PRIVILEGE_RING0,
    ia32_privilege_driver = IA32_PRIVILEGE_RING1,
    ia32_privilege_service = IA32_PRIVILEGE_RING2,
    ia32_privilege_user = IA32_PRIVILEGE_RING3,
  } t_ia32_privilege;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../pmode.c
 */

/*
 * ../pmode.c
 */

t_error		ia32_pmode_init(void);

t_error		ia32_pmode_set_segment_registers(t_uint16	seg_code,
						 t_uint16	seg_data);

t_error		ia32_pmode_clean(void);


/*
 * eop
 */

#endif
