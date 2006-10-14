/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libsun4u/include/libsun4u.h
 *
 * created       matthieu bucchianeri   [thu aug 10 15:20:03 2006]
 * updated       matthieu bucchianeri   [sat aug 26 19:57:44 2006]
 */

#ifndef LIBSUN4U_LIBSUN4U_H
#define LIBSUN4U_LIBSUN4U_H	1

/*
 * ---------- includes --------------------------------------------------------
 */

#include "misc/types.h"
#include "misc/stdarg.h"
#include "misc/asi.h"
#include "mmu/mmu.h"
#include "mmu/tlb.h"
#include "ofw/ofw.h"
#include "traps/trap.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../misc/tick.c
 *      ../mmu/mmu.c
 *      ../mmu/tlb.c
 *      ../mmu/tsb.c
 *      ../ofw/ofw.c
 *      ../traps/handler.c
 *      ../traps/trap.c
 */

/*
 * ../misc/tick.c
 */

void			tick_calibrate(void);

void			tick_interval(t_uint64			interval);


/*
 * ../mmu/mmu.c
 */

void			mmu_enable(void);

void			mmu_disable(void);

void			mmu_context(t_sparc64_context		context);

void			mmu_bypass_memcpy_r(void*		dst,
					    const void*		src,
					    t_uint64		size);

void			mmu_bypass_memcpy_w(void*		dst,
					    const void*		src,
					    t_uint64		size);

void			mmu_bypass_memcpy_rw(void*		dst,
					    const void*		src,
					    t_uint64		size);


/*
 * ../mmu/tlb.c
 */

void			tlb_dump(void);

void			tlb_init(void);

t_error			tlb_add(t_sint64			index,
				t_sparc64_tlb_type		type,
				t_sparc64_tlb_entry*	       	entry);

t_error			tlb_remove(t_sint64			index,
				   t_sparc64_tlb_type		type);

t_error			tlb_get(t_sint64			index,
				t_sparc64_tlb_type		type,
				t_sparc64_tlb_entry*   		entry);


/*
 * ../mmu/tsb.c
 */


/*
 * ../ofw/ofw.c
 */

int			ofw_wrap_init(t_ofw_args*		args);

t_error		ofw_call(t_ofw_args*		args);

t_error		ofw_init(void);

t_error		ofw_child(t_ofw_node		node,
			  t_ofw_node*		child);

t_error		ofw_sibling(t_ofw_node		node,
			    t_ofw_node*		sibling);

t_error		ofw_finddevice(const char*	dev,
			       t_ofw_handle*	handle);

t_error		ofw_getprop(t_ofw_handle	handle,
			    const char*		prop,
			    void*		buf,
			    t_uint64		size,
			    t_uint64*		read);

t_error		ofw_open(const char*		name,
			 t_ofw_handle*		handle);

t_error		ofw_close(t_ofw_handle		handle);

t_error		ofw_read(t_ofw_handle		handle,
			 char*			string,
			 t_uint64		len,
			 t_uint64*		read);

t_error		ofw_write(t_ofw_handle		handle,
			  const char*		string,
			  t_uint64		len,
			  t_uint64*		written);

t_error		ofw_exit(void);

t_error		ofw_enter(void);

t_error		ofw_interpret(const char*	cmd);


/*
 * ../traps/handler.c
 */

void			handler_spill(t_uint64			trap);

void			handler_fill(t_uint64			trap);


/*
 * ../traps/trap.c
 */

void			trap_init(void);

void			trap_add(t_uint64			trap,
				 t_sparc64_handler		handler);

void			trap_remove(t_uint64			trap);


/*
 * eop
 */

#endif
