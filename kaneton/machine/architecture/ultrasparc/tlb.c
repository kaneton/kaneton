/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libsun4u/mmu/tlb.c
 *
 * created       matthieu bucchianeri   [mon aug 21 19:54:01 2006]
 * updated       matthieu bucchianeri   [mon aug 28 18:46:59 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

t_sparc64_tlb_entry	itlb[N_ITLB_ENTRY];
t_sparc64_tlb_entry	dtlb[N_DTLB_ENTRY];

/*
 * ---------- functions -------------------------------------------------------
 */

static t_sint64		size_to_k(t_uint64		size)
{
  if (!size)
    return 8;

  return 8 * size_to_k(size - 1);
}

/*
 * this function dumps TLB state.
 */

void			tlb_dump(void)
{
  t_uint64		i;

  printf("%lu I-TLB entries :\n", N_ITLB_ENTRY);

  for (i = 0; i < N_ITLB_ENTRY; i++)
    {
      if (itlb[i].valid)
	printf("I-%lu: %lp -> %lp (%ld k) %s%s%s%s (context = %ld)\n", i,
	       itlb[i].virtual, itlb[i].physical, size_to_k(itlb[i].size),
	       itlb[i].global ? "G" : "",
	       itlb[i].locked ? "L" : "",
	       itlb[i].writable ? "W" : "",
	       itlb[i].privileged ? "P" : "",
	       itlb[i].context);
    }

  printf("%lu D-TLB entries :\n", N_DTLB_ENTRY);

  for (i = 0; i < N_DTLB_ENTRY; i++)
    {
      if (dtlb[i].valid)
	printf("D-%lu: %lp -> %lp (%ld k) %s%s%s (context = %ld)\n", i,
	       dtlb[i].virtual, dtlb[i].physical, size_to_k(dtlb[i].size),
	       dtlb[i].global ? "G" : "",
	       dtlb[i].locked ? "L" : "",
	       dtlb[i].privileged ? "P" : "",
	       dtlb[i].context);
    }
}

/*
 * this function initialize internal structures for both I-TLB and D-TLB.
 */

void			tlb_init(void)
{
  t_uint64		i;
  t_uint64		tag;
  t_uint64		data;

  memset(itlb, 0, sizeof (itlb));

  for (i = 0; i < N_ITLB_ENTRY; i++)
    {
      tag = LDXA(ASI_IMMU_TAG_READ, i << 3);
      data = LDXA(ASI_IMMU_DATA_ACCESS, i << 3);

      itlb[i].valid = !!(data & (1 << 63));
      if (itlb[i].valid)
	{
	  itlb[i].virtual = tag & (~0xFFF);
	  itlb[i].physical = data & 0xFFFFFFF000;
	  itlb[i].size = (data & ((1 << 62) | (1 << 61))) >> 61;
	  itlb[i].global = !!(data & (1 << 0));
	  if (!itlb[i].global)
	    itlb[i].context = tag & 0xFFF;
	  itlb[i].locked = !!(data & (1 << 6));
	  itlb[i].privileged = !!(data & (1 << 2));
	}
    }

  memset(dtlb, 0, sizeof (dtlb));

  for (i = 0; i < N_DTLB_ENTRY; i++)
    {
      tag = LDXA(ASI_DMMU_TAG_READ, i << 3);
      data = LDXA(ASI_DMMU_DATA_ACCESS, i << 3);

      dtlb[i].valid = !!(data & (1 << 63));
      if (dtlb[i].valid)
	{
	  dtlb[i].virtual = tag & (~0xFFF);
	  dtlb[i].physical = data & 0xFFFFFFF000;
	  dtlb[i].size = (data & ((1 << 62) | (1 << 61))) >> 61;
	  dtlb[i].writable = !!(data & (1 << 1));
	  dtlb[i].global = !!(data & (1 << 0));
	  if (!dtlb[i].global)
	    dtlb[i].context = tag & 0xFFF;
	  dtlb[i].locked = !!(data & (1 << 6));
	  dtlb[i].privileged = !!(data & (1 << 2));
	}
    }
}

/*
 * this function adds an entry to the translation cache.
 *
 * XXX support for anonymous add (index = -1).
 */

t_status		tlb_add(t_sint64			index,
				t_sparc64_tlb_type		type,
				t_sparc64_tlb_entry*	       	entry)
{
  t_uint64		tag;
  t_uint64		data;

  if (type == sparc64_tlb_itlb || type == sparc64_tlb_both)
    {
      if (index < 0 || index >= N_ITLB_ENTRY || !itlb[index].valid)
	return (STATUS_UNKNOWN_ERROR);

      memcpy(&itlb[index], entry, sizeof (t_sparc64_tlb_entry));
      itlb[index].valid = 1;

      tag = (entry->context & 0xFFF) | (entry->virtual & ~(0xFFF));
      data = (1 << 63) | (1 << 5);
      data |= (entry->global ? (1 << 0): 0);
      data |= (entry->locked ? (1 << 6): 0);
      data |= (entry->privileged ? (1 << 2): 0);
      data |= (entry->size << 61);

      STXA(ASI_IMMU, REG_IMMU_TAG_ACCESS, tag);
      STXA(ASI_IMMU_DATA_ACCESS, index << 3, data);
    }

  if (type == sparc64_tlb_dtlb || type == sparc64_tlb_both)
    {
      if (index < 0 || index >= N_DTLB_ENTRY || !dtlb[index].valid)
	return (STATUS_UNKNOWN_ERROR);

      memcpy(&dtlb[index], entry, sizeof (t_sparc64_tlb_entry));
      dtlb[index].valid = 1;

      tag = (entry->context & 0xFFF) | (entry->virtual & ~(0xFFF));
      data = (1 << 63) | (1 << 5) | (1 << 4);
      data |= (entry->global ? (1 << 0): 0);
      data |= (entry->writable ? (1 << 1): 0);
      data |= (entry->locked ? (1 << 6): 0);
      data |= (entry->privileged ? (1 << 2): 0);
      data |= (entry->size << 61);

      STXA(ASI_DMMU, REG_DMMU_TAG_ACCESS, tag);
      STXA(ASI_DMMU_DATA_ACCESS, index << 3, data);
    }

  return (STATUS_OK);
}

/*
 * this function remove a cache entry.
 */

t_status		tlb_remove(t_sint64			index,
				   t_sparc64_tlb_type		type)
{
  if (type == sparc64_tlb_itlb || type == sparc64_tlb_both)
    {
      if (index < 0 || index >= N_ITLB_ENTRY || !itlb[index].valid)
	return (STATUS_UNKNOWN_ERROR);

      itlb[index].valid = 0;

      STXA(ASI_IMMU, REG_IMMU_TAG_ACCESS, 0);
      STXA(ASI_IMMU_DATA_ACCESS, index << 3, 0);
    }

  if (type == sparc64_tlb_dtlb || type == sparc64_tlb_both)
    {
      if (index < 0 || index >= N_DTLB_ENTRY || !dtlb[index].valid)
	return (STATUS_UNKNOWN_ERROR);

      dtlb[index].valid = 0;

      STXA(ASI_DMMU, REG_DMMU_TAG_ACCESS, 0);
      STXA(ASI_DMMU_DATA_ACCESS, index << 3, 0);
    }

  return (STATUS_OK);
}

/*
 * this function read a tlb entry.
 */

t_status		tlb_get(t_sint64			index,
				t_sparc64_tlb_type		type,
				t_sparc64_tlb_entry*   		entry)
{
  switch (type)
    {
      case sparc64_tlb_itlb:
	if (index < 0 || index >= N_ITLB_ENTRY || !itlb[index].valid)
	  return (STATUS_UNKNOWN_ERROR);

	memcpy(entry, &itlb[index], sizeof (t_sparc64_tlb_entry));

	return (STATUS_OK);
      case sparc64_tlb_dtlb:
	if (index < 0 || index >= N_DTLB_ENTRY || !dtlb[index].valid)
	  return (STATUS_UNKNOWN_ERROR);

	memcpy(entry, &dtlb[index], sizeof (t_sparc64_tlb_entry));

	return (STATUS_OK);
      case sparc64_tlb_both:
      default:
	return (STATUS_UNKNOWN_ERROR);
    }
}
