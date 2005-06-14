/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * segment.h
 * 
 * path          /home/mycure/kaneton/core/include/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:44 2005   mycure
 * last update   Mon Jun 13 15:34:44 2005   mycure
 */

#ifndef KANETON_SEGMENT_H
#define KANETON_SEGMENT_H	1

/*
 * defines
 */

/* XXX */

/*
 * types
 */

typedef struct			s_segment
{
  t_segid			segid;

  t_paddr			address;
  t_psize			size;

  struct s_segment*		prv;
  struct s_segment*		nxt;
}				t_segment;

typedef struct			s_segments
{
  t_uint32			nsegments;

  t_segment*			segments;
}				t_segments;

/*
 * prototypes:     ../../kaneton/segment/segment.c
 */

/*
 * eop
 */

#endif
