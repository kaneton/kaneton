/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibc/libdata/alloc.c
 *
 * created       cedric aubouy   [sun sep 25 19:57:33 2005]
 * updated       matthieu bucchianeri   [wed dec  6 00:31:11 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 * XXX
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <libdata/alloc.h>
#include <kaneton.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define AREA_LIMIT(_area_)						\
  (void*)((t_uint8*)(_area_) + sizeof(t_area) + (_area_)->size)

#define NEXT_CHUNK(_chunk_)						\
  (t_chunk*)((t_uint8*)(_chunk_) + sizeof(t_chunk) + (_chunk_)->size)

#define ROUND_SIZE(_size_)						\
  ((_size_) % sizeof(t_vaddr) ?						\
   (_size_) + sizeof(t_vaddr) - (_size_) % sizeof(t_vaddr) :		\
   (_size_))

#define PAGED_SIZE(_size_)						\
  ((_size_) % PAGESZ ?							\
   (_size_) + PAGESZ - (_size_) % PAGESZ :				\
   (_size_))

#define SIGN(_type_,_obj_)						\
  (_obj_)->signature = _type_##_SIGNATURE;

#define CHECK_SIGN(_type_,_obj_)					\
  ((_obj_)->signature == _type_##_SIGNATURE)

/*
 * ---------- globals ---------------------------------------------------------
 */

t_alloc			alloc;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function allocates memory.
 *
 * steps:
 *
 * 1) align the size correctly.
 * 2) loop through the areas.
 * 3) loop through the chunks.
 * 4) is the current chunk large enough ?
 *  a) split the chunk in two if necessary.
 *  b) compute the address to return.
 *  c) update the free-list.
 * 5) we failed searching into our available chunks.
 *  a) reserve a new area.
 *  b) build the area.
 *  c) build the one or two chunks.
 * 6) increment the call counter.
 * 7) update the lowest and highest block addresses.
 */

void*			malloc(size_t				size)
{
  static int		stucked = 0;
  t_area*		area;
  t_area*		prev_area = NULL;
  t_chunk*		chunk;
  t_chunk*		prev_free = NULL;
  t_chunk*		next_free;
  t_chunk*		splitted;
  t_vsize		pagesz;
  t_vaddr		addr;
  void*			allocated = NULL;
  t_error		err;

  /*
   * 1)
   */

  size = ROUND_SIZE(size);

  /*
   * 2)
   */

  for (area = alloc.areas;
       area != NULL && allocated == NULL;
       prev_area = area, area = area->next_area)
    {
      if (area->size < size || area->first_free_chunk == NULL)
	continue;

      /*
       * 3)
       */

      for (chunk = area->first_free_chunk;
	   chunk != NULL && allocated == NULL;
	   prev_free = chunk, chunk = chunk->next_free)
	{
	  /*
	   * 4)
	   */

	  if (chunk->size >= size)
	    {
	      /*
	       * a)
	       */

	      if (chunk->size > size)
		{
		  if (chunk->size - size < sizeof(t_chunk))
		    {
		      size = chunk->size;
		    }
		  else
		    {
		      splitted = (t_chunk*)((t_uint8*)chunk + sizeof(t_chunk) +
					    size);
		      splitted->size = chunk->size - size - sizeof(t_chunk);
		      splitted->next_free = chunk->next_free;
		      splitted->area = area;
		      SIGN(CHUNK, splitted);
		      chunk->size = size;
		      chunk->next_free = splitted;
		    }
		}

	      /*
	       * b)
	       */

	      allocated = chunk + 1;

	      /*
	       * c)
	       */

	      next_free = chunk->next_free;

	      if (prev_free == NULL)
		{
		  area->first_free_chunk = next_free;
		}
	      else
		{
		  prev_free->next_free = next_free;
		}
	    }
	}
    }

  /*
   * 5)
   */

  if (allocated == NULL)
    {
      if (alloc.map_reserve == NULL)
	{
	  printf("FATAL ERROR: survey area exhausted !\n");

	  while (1)
	    ;
	}

      if (stucked)
	{
	  printf("Warning: allocating the reserve.\n");

	  addr = alloc.reserve;

	  alloc.reserve = 0;

	  pagesz = PAGESZ;
	}
      else
	{

	  /*
	   * a)
	   */

	  pagesz = PAGED_SIZE(size + sizeof(t_area) + sizeof(t_chunk));

	  stucked = 1;

	  err = alloc.map_reserve(alloc.asid,
				  alloc.asid ?
					  MAP_OPT_USER : MAP_OPT_PRIVILEGED,
				  pagesz,
				  PERM_READ | PERM_WRITE,
				  &addr);

	  stucked = 0;

	  if (err != ERROR_NONE)
	    {
	      printf("FATAL ERROR: physical memory exhausted !\n");

	      while (1)
		;
	    }

	  if (alloc.reserve == 0)
	    {
	      err = alloc.map_reserve(alloc.asid,
				      alloc.asid ?
				        MAP_OPT_USER : MAP_OPT_PRIVILEGED,
				      PAGESZ,
				      PERM_READ | PERM_WRITE,
				      &alloc.reserve);

	      if (err != ERROR_NONE)
		{
		  printf("FATAL ERROR: physical memory exhausted !\n");

		  while (1)
		    ;
		}
	    }
	}

      /*
       * b)
       */

      area = (t_area*)addr;

      area->size = pagesz - sizeof(t_area);
      area->prev_area = prev_area;
      area->next_area = NULL;
      SIGN(AREA, area);

      if (prev_area != NULL)
	prev_area->next_area = area;
      else
	alloc.areas = area;

      /*
       * c)
       */

      chunk = (t_chunk*)(area + 1);
      chunk->area = area;
      chunk->next_free = NULL;
      SIGN(CHUNK, chunk);
      allocated = chunk + 1;

      if (2 * sizeof(t_chunk) + size < area->size)
	{
	  chunk->size = size;
	  area->first_free_chunk = NEXT_CHUNK(chunk);
	  area->first_free_chunk->area = area;
	  area->first_free_chunk->next_free = NULL;
	  area->first_free_chunk->size = area->size - size -
	    2 * sizeof(t_chunk);
	  SIGN(CHUNK, area->first_free_chunk);
	}
      else
	{
	  chunk->size = area->size - sizeof(t_chunk);
	  area->first_free_chunk = NULL;
	}
    }

  /*
   * 6)
   */

  alloc.nalloc++;

  /*
   * 7)
   */

  if (allocated < alloc.lowest)
    alloc.lowest = allocated;

  if (allocated > alloc.highest)
    alloc.highest = allocated;

  alloc_check_signatures();

  return (allocated);
}

/*
 * this fonction releases a memory area.
 *
 * steps:
 *
 * 1) test pointer validity.
 * 2) look for the previous free chunk in the same area.
 * 3) update chunks information.
 * 4) try to merge side-by-side free chunks.
 * 5) if the area is empty then free it.
 * 6) increment the free calls counter.
 */

void			free(void*				ptr)
{
  t_chunk*		chunk = (t_chunk*)ptr - 1;
  t_area*		area;
  t_chunk*		l;
  t_chunk*		prev_free = NULL;
  t_chunk*		next;

  /*
   * 1)
   */

  if (!ptr)
    return;

  if (ptr < alloc.lowest)
    {
      printf("warning: junk pointer, to low to make sense.\n");
      return;
    }

  if (ptr > alloc.highest)
    {
      printf("warning: junk pointer, to high to make sense.\n");
      return;
    }

  /*
   * 2)
   */

  area = chunk->area;

  for (l = area->first_free_chunk;
       l != NULL;
       prev_free = l, l = l->next_free)
    {
      if (l == chunk)
	{
	  printf("warning: chunk is already free.\n");
	  return;
	}
      if (l > chunk)
	break;
    }

  /*
   * 3)
   */

  if (prev_free)
    {
      chunk->next_free = prev_free->next_free;
      prev_free->next_free = chunk;
    }
  else
    {
      chunk->next_free = area->first_free_chunk;
      area->first_free_chunk = chunk;
    }

  /*
   * 4)
   */

  if (chunk->next_free == NEXT_CHUNK(chunk))
    {
      next = chunk->next_free;
      chunk->next_free = next->next_free;
      chunk->size += sizeof(t_chunk) + next->size;
    }

  if (prev_free != NULL && NEXT_CHUNK(prev_free) == chunk)
    {
      prev_free->next_free = chunk->next_free;
      prev_free->size += sizeof(t_chunk) + chunk->size;
    }

  /*
   * 5)
   */

  if (area != alloc.areas &&
      area->first_free_chunk->size == area->size - sizeof(t_chunk))
    {
      area->prev_area->next_area = NULL;

      if (alloc.map_release(alloc.asid, (t_vaddr)area) != ERROR_NONE)
	printf("warning: unable to release area.\n");
    }

  /*
   * 6)
   */

  alloc.nfree++;

  alloc_check_signatures();
}

/*
 * this function returns the number of allocation calls.
 */

u_int32_t		alloc_nalloc(void)
{
  return (alloc.nalloc);
}

/*
 * this function returns the number of release calls.
 */

u_int32_t		alloc_nfree(void)
{
  return (alloc.nfree);
}

/*
 * this function dumps the allocator state.
 *
 * steps:
 *
 * 1) display some general information.
 * 2) loop through areas.
 * 3) loop through chunks in an area.
 */

void			alloc_dump(void)
{
  t_area*		area;
  t_chunk*		chunk;
  void*			limit;
  t_chunk*		next;
  t_chunk*		next_free;

  /*
   * 1)
   */

  printf("allocator dump\n");

  printf("calls: %u alloc(), %u free()\n", alloc.nalloc, alloc.nfree);

  printf("dumping all chunks:\n");

  /*
   * 2)
   */

  for (area = alloc.areas;
       area != NULL;
       area = area->next_area)
    {
      printf("area: %p, size: %u\n", area, area->size);

      limit = AREA_LIMIT(area);
      next_free = area->first_free_chunk;

      /*
       * 3)
       */

      for (chunk = (t_chunk*)(area + 1);
	   (void*)chunk < limit;
	   chunk = next)
	{
	  printf("  chunk: %p, user address: %p, size: %u", chunk,
		 chunk + 1, chunk->size);
	  if (chunk == next_free)
	    {
	      next_free = chunk->next_free;
	      printf(" FREE\n");
	    }
	  else
	    printf("\n");

	  next = NEXT_CHUNK(chunk);
	}
    }
}

/*
 * this function checks all the signatures.
 *
 * steps:
 *
 * 1) loop through areas.
 * 2) loop through chunks in an area.
 */

void			alloc_check_signatures(void)
{
  t_area*		area;
  t_chunk*		chunk;
  void*			limit;
  t_chunk*		next;
  t_chunk*		next_free;

  /*
   * 2)
   */

  for (area = alloc.areas;
       area != NULL;
       area = area->next_area)
    {
      limit = AREA_LIMIT(area);

      if (!CHECK_SIGN(AREA, area))
	printf("warning: area %p signature is broken\n", area);

      for (chunk = (t_chunk*)(area + 1);
	   (void*)chunk < limit;
	   chunk = next)
	{
	  if (!CHECK_SIGN(CHUNK, chunk))
	    printf("warning: chunk %p signature is broken\n", area);

	  next = NEXT_CHUNK(chunk);
	}
    }
}

/*
 * this fonction reallocates memory, meaning this function resizes a memory
 * area.
 *
 * steps:
 *
 */

void*			realloc(void* 				ptr,
				size_t				size)
{
  t_chunk*		chunk = (t_chunk*)ptr - 1;
  void*			new;
  size_t		s;

  if (ptr == NULL)
    return malloc(size);

  if ((new = malloc(size)) == NULL)
    return (NULL);

  if (chunk->size > size)
    s = size;
  else
    s = chunk->size;

  memcpy(new, ptr, s);

  free(ptr);

  return (new);
}

/*
 * this function setup the map_reserve/map_release pointers.
 */

void			alloc_setup(t_pfn_map_reserve		freserve,
				    t_pfn_map_release		frelease,
				    i_as			asid)
{
  t_error		err;

  alloc.map_reserve = freserve;
  alloc.map_release = frelease;
  alloc.asid = asid;

  err = alloc.map_reserve(alloc.asid,
			  alloc.asid ?
				  MAP_OPT_USER : MAP_OPT_PRIVILEGED,
			  PAGESZ,
			  PERM_READ | PERM_WRITE,
			  &alloc.reserve);

  if (err != ERROR_NONE)
    {
      printf("FATAL ERROR: physical memory exhausted !\n");

      while (1)
	;
    }
}

/*
 * this function initialises the allocator.
 *
 * the arguments addr and size specify a kind of survival area. the kernel
 * will call this allocator to dynamically reserve memory while no
 * kernel memory manager is installed yet. so this area will be used in a
 * very special way.
 *
 * steps:
 *
 * 1) initialize the allocator global structure.
 * 2) create the first chunk.
 * 3) create the first area and put the first chunk in.
 */

int			alloc_init(t_vaddr			addr,
				   t_size			size)
{
  t_area*		first_area;
  t_chunk*		first_chunk;

  /*
   * 1)
   */

  memset(&alloc, 0x0, sizeof(t_alloc));

  first_area = alloc.areas = (t_area*)addr;
  alloc.nalloc = 0;
  alloc.nfree = 0;
  alloc.lowest = (void*)((t_vaddr)-1);
  alloc.highest = (void*)0;
  alloc.map_reserve = NULL;
  alloc.map_release = NULL;
  alloc.reserve = 0;

  /*
   * 2)
   */

  first_chunk = (t_chunk*)(first_area + 1);
  first_chunk->size = size - sizeof(t_area) - sizeof(t_chunk);
  first_chunk->next_free = NULL;
  first_chunk->area = first_area;
  SIGN(CHUNK, first_chunk);

  /*
   * 3)
   */

  first_area->size = size - sizeof(t_area);
  first_area->first_free_chunk = first_chunk;
  first_area->prev_area = NULL;
  first_area->next_area = NULL;
  SIGN(AREA, first_area);

  return (0);
}
