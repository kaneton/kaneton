/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ibm-pc.ia32/educational/include/region.h
 *
 * created       julien quintard   [wed jun  6 16:22:05 2007]
 * updated       julien quintard   [mon dec 13 11:51:08 2010]
 */

#ifndef GLUE_REGION_H
#define GLUE_REGION_H		1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * these macro-function redirect the calls from the core to the appropriate
 * glue function but also provide the machine-specific data to include
 * in the core managers, objects etc.
 */

#define		machine_include_region()				\
  extern d_region	glue_region_dispatch

#define		machine_call_region(_function_, _args_...)		\
  (									\
    {									\
      t_error	_r_ = ERROR_OK;						\
									\
      if (glue_region_dispatch.region_ ## _function_ != NULL)		\
        _r_ = glue_region_dispatch.region_ ## _function_(_args_);	\
									\
      _r_;								\
    }									\
  )

#define		machine_data_m_region()

#define		machine_data_o_region()

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../region.c
 */

/*
 * ../region.c
 */

t_error			glue_region_resize(i_as			as,
					   i_region		old,
					   t_vsize		size,
					   i_region*		new);

t_error			glue_region_reserve(i_as		asid,
					    i_segment		segid,
					    t_paddr		offset,
					    t_options		options,
					    t_vaddr		address,
					    t_vsize		size,
					    i_region*		regid);

t_error			glue_region_release(i_as		asid,
					    i_region		regid);

t_error			glue_region_initialize(t_vaddr		base,
					       t_vsize		size);

t_error			glue_region_clean(void);


/*
 * eop
 */

#endif
