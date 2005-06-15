/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * as.h
 * 
 * path          /home/mycure/kaneton/core/include
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:19:44 2005   mycure
 * last update   Tue Jun 14 19:24:11 2005   mycure
 */

#ifndef KANETON_AS_H
#define KANETON_AS_H		1

/*
 * defines
 */

/* XXX */

/*
 * types
 */

typedef struct			s_as
{
  t_asid			asid;
}				t_segment;

typedef struct			s_ass
{
  t_uint32			nass;

  t_as*				ass;
}				t_ass;

/*
 * prototypes:     ../../kaneton/as/as.c
 */

/*
 * ../../kaneton/as/as.c
 */

int			as_init(void);

/*
 * eop
 */

#endif
