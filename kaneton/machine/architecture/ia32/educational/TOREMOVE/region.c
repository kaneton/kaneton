
/*
 * ---------- functions -------------------------------------------------------
 */


/*
 * this function resizes a region.
 *
 * steps:
 *
 * A) region shrinking.
 *  1) create and inject a fake region corresponding to the part to unmap.
 *  2) calls directly the IA-32 code to unmap the fake region.
 * B) region enlarging.
 *  1) create and add a fake region corresponding to the new part to map.
 *  2) calls directly the dependent code to do the mapping.
 */

t_error			ia32_region_resize(i_as			as,
					   i_region		old,
					   t_vsize		size,
					   i_region*		new)
{
  i_region		useless;
  o_region*		o;
  o_as*			oas;
  o_region*		tmp;

  REGION_ENTER(region);

  if (region_get(as, old, &o) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if (as_get(as, &oas) != ERROR_NONE)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  if ((tmp = malloc(sizeof(o_region))) == NULL)
    REGION_LEAVE(region, ERROR_UNKNOWN);

  /*
   * A)
   */

  if (size < o->size)
    {
      /*
       * 1)
       */

      tmp->regid = o->regid + size;
      tmp->segid = o->segid;
      tmp->address = o->address + size;
      tmp->offset = o->offset + size;
      tmp->size = o->size - size;
      tmp->opts = o->opts;

      if (region_inject(as, tmp, &useless) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      /*
       * 2)
       */

      if (ia32_region_release(as, tmp->regid) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      if (set_remove(oas->regions, tmp->regid) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);
    }
  else

  /*
   * B)
   */

    {
      /*
       * 1)
       */

      tmp->regid = o->regid + o->size;
      tmp->segid = o->segid;
      tmp->address = o->address + o->size;
      tmp->offset = o->offset + o->size;
      tmp->size = size - o->size;
      tmp->opts = o->opts;

      if (region_inject(as, tmp, &useless) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      /*
       * 2)
       */

      if (ia32_region_reserve(as,
			      tmp->segid,
			      tmp->offset,
			      tmp->opts,
			      tmp->address,
			      tmp->size,
			      &tmp->regid) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);

      if (set_remove(oas->regions, tmp->regid) != ERROR_NONE)
	REGION_LEAVE(region, ERROR_UNKNOWN);
    }

  REGION_LEAVE(region, ERROR_NONE);
}

