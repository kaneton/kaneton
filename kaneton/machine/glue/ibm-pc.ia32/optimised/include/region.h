/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/machine/glue/ibm-pc.ia32/include/region.h
 *
 * created       julien quintard   [wed jun  6 16:22:05 2007]
 * updated       matthieu bucchianeri   [fri jun 15 09:52:34 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file supposes it is included by core.h so that PAGESZ is defined.
 */

#ifndef GLUE_REGION_H
#define GLUE_REGION_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machine_include_region()				\
  extern d_region		region_dispatch

#define		machine_call_region(_function_, _args_...)		\
  (									\
    {									\
      t_status _r_ = STATUS_OK;					\
									\
      if (region_dispatch._function_ != NULL)				\
        _r_ = region_dispatch._function_(_args_);			\
									\
      _r_;								\
    }									\
  )

#define		machine_data_m_region()

#define		machine_data_o_region()

/*
 * ---------- macros ----------------------------------------------------------
 */

#define REGION_VMEM_MIN		PAGESZ
#define REGION_VMEM_MAX		0xffffffffU

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../region.c
 */

/*
 * ../region.c
 */

t_status		glue_region_resize(i_as			as,
					   i_region		old,
					   t_vsize		size,
					   i_region*		new);

t_status		glue_region_reserve(i_as		asid,
					    i_segment		segid,
					    t_paddr		offset,
					    t_opts		opts,
					    t_vaddr		address,
					    t_vsize		size,
					    i_region*		regid);

t_status		glue_region_release(i_as		asid,
					    i_region		regid);

t_status		glue_region_initialize(t_vaddr		start,
					       t_vsize		size);

t_status		glue_region_clean(void);


/*
 * eop
 */

#endif
