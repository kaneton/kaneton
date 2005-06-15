/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * segment.h
 * 
 * path          /home/mycure/kaneton/core/include
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:44 2005   mycure
 * last update   Tue Jun 14 19:22:15 2005   mycure
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

  /* t_ownid			ownid; */

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
 * ../../kaneton/segment/segment.c
 */

t_segid			segment_id(void);


void			segment_dump(void);


void			segment_push(t_segment*			push);


t_segment*		segment_pop(void);


int			segment_init(void);

/*
 * eop
 */

#endif
