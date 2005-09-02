/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * init.h
 * 
 * path          /home/mycure/kaneton/core/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:11 2005   mycure
 * last update   Tue Aug 30 15:41:06 2005   mycure
 */

#ifndef KANETON_INIT_H
#define KANETON_INIT_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>
#include <kaneton/segment.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

#define INIT_KSTACKSZ		(2 * PAGESZ)

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * modules
 */

typedef struct
{
  char*				name;
  t_psize			size;
}				t_module;

typedef struct
{
  t_uint32			nmodules;
}				t_modules;

/*
 * regions
 */

typedef struct			/* XXX to move the region.h */
{
  t_vaddr			address;
  t_segid			segid;
}				o_region;

/*
 * the init structure
 *
 * note that concerning the segments and the regions, the kernel waits
 * for sorted segments and regions the segment[0] being the lowest in
 * memory.
 */

typedef struct
{
  t_paddr			mem;
  t_psize			memsz;

  t_paddr			kcode;
  t_psize			kcodesz;

  t_paddr			init;
  t_psize			initsz;

  t_modules*			modules;
  t_psize			modulessz;

  t_uint32			nsegments;
  o_segment*			segments;
  t_psize			segmentssz;

  t_uint32			nregions;
  o_region*			regions;
  t_psize			regionssz;

  t_paddr			kstack;
  t_psize			kstacksz;

  t_paddr			alloc;
  t_psize			allocsz;

  machdep_data(init);
}				t_init;

#endif
