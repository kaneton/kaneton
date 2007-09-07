/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...cture/ia32/educational/include/virtual.h
 *
 * created       julien quintard   [wed jun  6 16:25:05 2007]
 * updated       matthieu bucchianeri   [sat jun 16 17:41:12 2007]
 */

#ifndef ARCHITECTURE_VIRTUAL_H
#define ARCHITECTURE_VIRTUAL_H	1

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../virtual.c
 */

/*
 * ../virtual.c
 */

t_error			ia32_kernel_as_init(i_as	asid);

t_error			ia32_kernel_as_finalize(void);

t_error			ia32_task_as_init(i_as		asid);

t_error			ia32_segmentation_init(void);

t_error			ia32_segment_read(i_region		segid,
					  t_paddr		offs,
					  void*			buff,
					  t_psize		sz);

t_error			ia32_segment_write(i_region		segid,
					   t_paddr		offs,
					   const void*		buff,
					   t_psize		sz);

t_error			ia32_segment_copy(i_region		dst,
					  t_paddr		offsd,
					  i_region		src,
					  t_paddr		offss,
					  t_psize		sz);

t_error			ia32_map_chunk(t_vaddr		v,
				       t_paddr		p,
				       void*		alloc);

t_error			ia32_unmap_chunk(t_vaddr	v);

t_error			ia32_map_pd(t_ia32_directory*	pd);

t_error			ia32_map_pt(t_ia32_table*	pt);

t_error			ia32_map_region(i_as		asid,
					i_segment	segid,
					t_paddr		offset,
					t_opts		opts,
					t_vaddr		address,
					t_vsize		size);

t_error			ia32_unmap_region(i_as		asid,
					  t_vaddr	address,
					  t_vsize	size);


/*
 * eop
 */

#endif
