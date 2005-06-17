/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * init.h
 * 
 * path          /home/mycure/kaneton/core/include/arch/machdep
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:11 2005   mycure
 * last update   Fri Jun 17 14:56:18 2005   mycure
 */

#ifndef KANETON_INIT_H
#define KANETON_INIT_H		1

/*
 * includes
 */

#include <kaneton/segment.h>

/*
 * defines
 */

#define INIT_KSTACKSZ		8192

/*
 * modules
 */

typedef struct			s_module
{
  char*				name;
  t_psize			size;
}				t_module;

typedef struct			s_modules
{
  t_uint32			nmodules;
}				t_modules;

/*
 * regions
 */

typedef struct			s_region /* XXX to move the region.h */
{
  t_vaddr			address;
  t_segid			segid;
}				t_region;

typedef struct			s_regions
{
  t_uint32			nregions;
  t_region*			regions;
}				t_regions;

/*
 * the init structure
 *
 * note that concerning the segments and the regions, the kernel waits
 * for sorted segments and regions the segment[0] being the lowest in
 * memory.
 */

typedef struct			s_init
{
  t_paddr			mem;
  t_psize			memsz;

  t_paddr			kcode;
  t_psize			kcodesz;

  t_paddr			init;
  t_psize			initsz;

  t_modules*			modules;
  t_psize			modulessz;

  t_segments*			segments;
  t_psize			segmentssz;

  t_regions*			regions;
  t_psize			regionssz;

  t_paddr			kstack;
  t_psize			kstacksz;

  t_paddr			segmng;
  t_psize			segmngsz;

  t_paddr			asmng;
  t_psize			asmngsz;

  machdep_include(init);
}				t_init;

#endif
