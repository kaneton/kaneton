/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * segment.c
 * 
 * path          /home/mycure/kaneton/core/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Sun Jun 19 14:43:10 2005   mycure
 */

#include <libc.h>
#include <kaneton.h>

/*
 * the variable init.
 */

extern t_init*		init;

/*
 * the stack of free segment structures, in other words the freelist.
 */

t_segments		stack;

/*
 * the list of segments.
 */

t_segments		segments;

/*
 * this function just returns an identfier.
 *
 * for the moment, this function justs increments a number and never
 * recycles identifiers.
 */

t_segid			segment_id(void)
{
  static t_segid	segid = 0;

  return (segid++);
}

/*
 * this function dumps the segments.
 */

void			segment_dump(void)
{
  t_segment*		s;

  cons_msg('#', "dumping segments:\n");

  cons_msg('#', " stack: %u\n",
	   stack.nsegments);

  cons_msg('#', " segments: %u\n",
	   segments.nsegments);

  /* XXX
  for (s = segments.segments; s != NULL; s = s->nxt)
    cons_msg('#', "  [%qu] 0x%x - %u bytes\n",
	     s->segid,
	     s->address,
	     s->size);
  */
}

/*
 * this function adds a segment structure to the freelist.
 */

void			segment_push(t_segment*			push)
{
  /* XXX
  push->segid = 0;
  push->address = 0;
  push->size = 0;

  if (stack.segments == NULL)
    {
      push->prv = NULL;
      push->nxt = NULL;

      stack.segments = push;
    }
  else
    {
      push->prv = NULL;
      push->nxt = stack.segments;

      stack.segments->prv = push;

      stack.segments = push;
    }

  stack.nsegments++;
  */
}

/*
 * this function pops a segment structure from the stack.
 */

t_segment*		segment_pop(void)
{
  t_segment*		pop;

  /* XXX */

  // XXX if (stack.nsegments < 2)
    {
      /* XXX reserver une page de memoire physique, et la decoupee
       * pour faire des segment_push()
       */

      /* XXX */
      cons_msg('!', "error: not enough memory\n");
      // XXX kaneton_error();
    }
    /*  XXX
  pop = stack.segments;

  stack.segments = pop->nxt;
  stack.segments->prv = NULL;

  pop->prv = NULL;
  pop->nxt = NULL;

  stack.nsegments--;

  return (pop);
    */
}

/*
 * this functions initializes the segment manager from the init
 * variable containing segments to keep safe.
 *
 * steps:
 *
 * 1) initializes the stack.
 * 2) initializes the segments inserting the segments specified by the
 *    init variable.
 */

int			segment_init(void)
{
  t_uint32		n;
  t_sint32		i;

  /*
   * XXX
   */
  /* XXX 
  stack.nsegments = 0;
  stack.segments = 0;

  n = init->segmngsz / sizeof(t_segment);
  for (i = 0; i < n; i++)
    segment_push((t_segment*)(init->segmng + i * sizeof(t_segment)));
  */
  /*
   * XXX
   */
  /* XXX
  segments.nsegments = 0;
  segments.segments = NULL;
  */
  /*
   * XXX
   */

  /*
   * XXX revoir le bootloader pour qu il mette les permissions a mettre
   * sur chaque zone genre RX sur les modules.
   */
  /* XXX
  for (i = 0; i < init->segments->nsegments; i++)
    {
      t_segment		s;

      s.segid = segment_id();
      s.address = init->segments->segments[i].address;
      s.size = init->segments->segments[i].size;

      // XXX segment_add(AS_KERNEL, &s);
    }
  */
  /*
   * XXX
   */

#if (KANETON_DEBUG & KANETON_DEBUG_SEGMENT)
  segment_dump();
#endif

  return (0);
}
