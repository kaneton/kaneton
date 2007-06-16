
/*
 * this function sets the permissions of a segment.
 *
 * steps:
 *
 * 1) get the segment object.
 * 2) get the address space and the kernel address space object.
 * 3) search for regions mapping the segment.
 *  a) map the page directory if necessary.
 *  b) loop throught the page directory entries.
 *  c) map the page table temporarily.
 *  d) loop throught the page table entries.
 *  e) change the read/write flag.
 *  f) invalidate the TLB if necessary.
 *  g) unmap the page table.
 * 4) unmap the page directory if needed.
 */

t_error			ia32_segment_perms(i_segment		segid,
					   t_perms		perms)
{
  t_ia32_directory	pd = NULL;;
  t_ia32_table		pt;
  t_ia32_page		pg;
  t_ia32_pde		pde_start;
  t_ia32_pde		pde_end;
  t_ia32_pte		pte_start;
  t_ia32_pte		pte_end;
  t_ia32_pde		pde;
  t_ia32_pte		pte;
  o_region*		reg;
  o_as*			kas;
  t_state		statea;
  t_iterator		ita;
  t_state		state;
  t_iterator		it;
  t_vaddr		vaddr;
  t_vaddr		chunk;
  void*			tmp;
  o_as*			o;

  SEGMENT_ENTER(segment);

  /*
   * 2)
   */

  if (as_get(kasid, &kas) != ERROR_NONE)
    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

  /*
   * 3)
   */

  set_foreach(SET_OPT_FORWARD, as->ass, &ita, statea)
    {
      if (set_object(as->ass, ita, (void**)&o) != ERROR_NONE)
	SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

      set_foreach(SET_OPT_FORWARD, o->regions, &it, state)
	{
	  if (set_object(o->regions, it, (void**)&reg) != ERROR_NONE)
	    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	  if (reg->segid == segid)
	    {

	      /*
	       * a)
	       */

	      if (pd == NULL)
		{
		  if (o->asid == kasid)
		    {
		      pd = o->machdep.pd;
		    }
		  else
		    {
		      tmp = malloc(sizeof(o_region));

		      if (region_space(kas, PAGESZ, &chunk) != ERROR_NONE)
			SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

		      pd = (t_ia32_directory)(t_uint32)chunk;

		      if (ia32_region_map_chunk((t_vaddr)pd,
						(t_paddr)o->machdep.pd,
						tmp) !=
			  ERROR_NONE)
			SEGMENT_LEAVE(segment, ERROR_UNKNOWN);
		    }
		}

	      /*
	       * b)
	       */

	      vaddr = reg->address;

	      pde_start = PDE_ENTRY(vaddr);
	      pte_start = PTE_ENTRY(vaddr);
	      pde_end = PDE_ENTRY(vaddr + reg->size);
	      pte_end = PTE_ENTRY(vaddr + reg->size);

	      for (pde = pde_start; pde <= pde_end; pde++)
		{
		  if (pd_get_table(&pd, pde, &pt) != ERROR_NONE)
		    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

		  /*
		   * c)
		   */

		  tmp = malloc(sizeof(o_region));

		  if (region_space(kas, PAGESZ, &chunk) != ERROR_NONE)
		    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

		  if (ia32_region_map_chunk((t_vaddr)chunk,
					    (t_paddr)pt.entries,
					    tmp) != ERROR_NONE)
		    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

		  pt.entries = chunk;

		  /*
		   * d)
		   */

		  for (pte = (pde == pde_start ? pte_start : 0);
		       pte < (pde == pde_end ? pte_end : PT_MAX_ENTRIES);
		       pte++)
		    {
		      /*
		       * e)
		       */

		      if (pt_get_page(&pt, pte, &pg) != ERROR_NONE)
			SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

		      pg.rw = (perms & PERM_WRITE) ? PG_WRITABLE : PG_READONLY;

		      if (pt_add_page(&pt, pte, pg) != ERROR_NONE)
			SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

		      /*
		       * f)
		       */

		      if (o->asid == kasid)
			tlb_invalidate((t_vaddr)ENTRY_ADDR(pde, pte));
		    }

		  /*
		   * g)
		   */

		  if (ia32_region_unmap_chunk((t_vaddr)pt.entries) != ERROR_NONE)
		    SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

		}
	    }
	}

      /*
       * 4)
       */

      if (pd != NULL)
	{
	  if (o->asid != kasid)
	    if (ia32_region_unmap_chunk((t_vaddr)pd) != ERROR_NONE)
	      SEGMENT_LEAVE(segment, ERROR_UNKNOWN);

	  pd = NULL;
	}

    }

  SEGMENT_LEAVE(segment, ERROR_NONE);
}
