/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * init.h
 * 
 * path          /home/mycure/kaneton/core/include/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:11 2005   mycure
 * last update   Sun Jun 12 17:42:23 2005   mycure
 */

#ifndef KANETON_INIT_H
#define KANETON_INIT_H		1

#define MOD_NAMESZ		32 /* XXX to put into the mod manager */

#define INIT_KSTACKSZ		8192

/*
 * modules
 */

typedef struct			s_module
{
  char				name[MOD_NAMESZ + 1];
  t_psize			size;
}				t_module;

typedef struct			s_modules
{
  t_uint32			nmodules;
}				t_modules;

/*
 * segments
 */

typedef struct			s_segment /* XXX to move to segment.h */
{
  t_paddr			address;
  t_psize			size;
}				t_segment;

typedef struct			s_segments
{
  t_uint32			nsegments;
  t_segment*			segments;
}				t_segments;

/*
 * regions
 */

typedef struct			s_region /* XXX to move the region.h */
{
  t_vaddr			address;
  t_vsize			size;
}				t_region;

typedef struct			s_regions
{
  t_uint32			nregions;
  t_region*			regions;
}				t_regions;

/*
 * the init structure
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

  machdep_include(init);
}				t_init;

#endif
