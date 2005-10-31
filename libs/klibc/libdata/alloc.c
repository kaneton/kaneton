/*
 * copyright aubouy cedric
 *
 * kaneton
 *
 * alloc.c
 *
 * path          /home/mycure/kaneton/core/kaneton
 *
 * made by cedric
 *         Cedric Aubouy   [cedric.aubouy@gmail.com]
 *
 * started on    Sun Sep 25 19:57:33 2005   cedric
 * last update   Sun Oct 30 16:45:24 2005   mycure
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the memory allocator implemented here is a very very very simple version.
 *
 * indeed, the algorithm used is the first fit, the simplest algorithm.
 *
 * moreover, the free chuncks are not merge to form larger chuncks, increasing
 * the matches probability.
 *
 * finally the realloc() function is very basic. to further information take
 * a look to the function and especially to its description.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <libdata/alloc.h>
#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

t_alloc			alloc;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this inlined function aligns the size on a chunck size to simplify
 * the allocator internals.
 */

static inline size_t	alloc_align(size_t			size)
{
  return (size == 0 ?
	  sizeof(t_chunck) :
	  (size + ALLOC_ALIGN_MASK) & ~ALLOC_ALIGN_MASK);
}

/*
 * this function tries to find a free chunck big enough for the size
 * argument.
 */

static int		alloc_chunck(size_t			size,
				     t_chunck**			search)
{
  t_chunck*		tmp;

  for (tmp = alloc.free; tmp != NULL; tmp = tmp->nxt)
    if (tmp->size >= size)
      {
	*search = tmp;

	return (0);
      }

  return (-1);
}

/*
 * this function inserts the chunck new in the given list. the chuncks
 * are sorted by address.
 *
 * steps:
 *
 * 1) first tries to find a chunck with an higher address to inserts the new
 *    element before this one.
 * 2) finally, inserts the new chunk at the end of the list.
 */

static int		alloc_insert(t_chunck**			list,
				     t_chunck*			new)
{
  t_chunck*		prv;
  t_chunck*		tmp;

  /*
   * 1)
   */

  for (prv = NULL, tmp = *list; tmp != NULL; prv = tmp, tmp = tmp->nxt)
    if (tmp > new)
      {
	new->prv = tmp->prv;
	new->nxt = tmp;

	if (new->prv)
	  new->prv->nxt = new;

	if (new->nxt)
	  new->nxt->prv = new;

	if (prv == NULL)
	  *list = new;

	return (0);
      }

  /*
   * 2)
   */

  if (prv == NULL)
    {
      new->prv = NULL;
      new->nxt = NULL;

      *list = new;
    }
  else
    {
      new->prv = prv;
      new->nxt = NULL;

      if (new->prv)
	new->prv->nxt = new;
    }

  return (0);
}

/*
 * this function allocates memory.
 *
 * steps:
 *
 * 1) aligns the size on a chunck size.
 * 2) tries to find a free chunck big enough. if found no problem, otherwise
 *    this function will ask the system more physical and virtual memory.
 * 3) if the chunck's size does not exactly fit, then split the chunck
 *    into two, the first being the one returned to the user, the latter
 *    returning in the list of free chuncks.
 * 4) initialises the allocated chunck and puts it in the list of used chuncks.
 * 5) increases the allocate calls counter.
 */

void*			malloc(size_t				size)
{
  t_chunck*		search;
  t_chunck*		new;

  /*
   * 1)
   */

  size = alloc_align(size);

  /*
   * 2)
   */

  if (alloc_chunck(size, &search) != 0)
    {
      printf("malloc: fatal error during malloc(%u)\n", size);

      while (1)
	;

      /* XXX */
    }

  /*
   * 3)
   */

  if (search->size > (size + sizeof(t_chunck) + ALLOC_MINSZ))
    {
      new = (t_chunck*)((char*)search + size + sizeof(t_chunck));

      memset(new, 0x0, sizeof(t_chunck));

      new->state = ALLOC_STATE_FREE;
      new->size = search->size - size - sizeof(t_chunck);
      new->prv = search->prv;
      new->nxt = search->nxt;

      if (new->prv)
	new->prv->nxt = new;

      if (new->nxt)
	new->nxt->prv = new;

      if (alloc.free == search)
	alloc.free = new;
    }
  else
    {
      if (search->prv)
	search->prv->nxt = search->nxt;

      if (search->nxt)
	search->nxt->prv = search->prv;

      if (alloc.free == search)
	alloc.free = search->nxt;
    }

  /*
   * 4)
   */

  search->state = ALLOC_STATE_USE;
  search->size = size;
  search->prv = NULL;
  search->nxt = NULL;

  if (alloc_insert(&alloc.use, search) != 0)
    {
      printf("[!] fatal error during malloc()\n");

      return (NULL);
    }

  /*
   * 5)
   */

  alloc.nalloc++;

  return ((char*)search + sizeof(t_chunck));
}

/*
 * this fonction releases a memory area.
 *
 * steps:
 *
 * 1) checks whether the given pointer references a correct used chunck.
 * 2) reorganises the list of used chuncks by removing the chuncks of the
 *    list of used chuncks.
 * 3) reinitialises the fields of the chunck and puts it into the list
 *    of free chuncks.
 * 4) finally, increases the counter of free calls.
 */

void			free(void*				ptr)
{
  t_chunck*		chunck = (t_chunck*)((char*)ptr - sizeof(t_chunck));

  /*
   * 1)
   */

  if (chunck->state != ALLOC_STATE_USE)
    return ;

  /*
   * 2)
   */

  if (chunck->prv)
    chunck->prv->nxt = chunck->nxt;
  else
    alloc.use = chunck->nxt;

  if (chunck->nxt)
    chunck->nxt->prv = chunck->prv;

  /*
   * 3)
   */

  chunck->state = ALLOC_STATE_FREE;
  chunck->prv = NULL;
  chunck->nxt = NULL;

  if (alloc_insert(&alloc.free, chunck) != 0)
    {
      printf("[!] fatal error during free()\n");

      return ;
    }

  /*
   * 4)
   */

  alloc.nfree++;
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
 */

void			alloc_dump(void)
{
  t_chunck*		tmp;

  printf("[#] chunck size: %u bytes\n", sizeof(t_chunck));

  printf("[#] calls: %u alloc(), %u free()\n", alloc.nalloc, alloc.nfree);

  printf("[#] dumping free chuncks:\n");

  for (tmp = alloc.free; tmp != NULL; tmp = tmp->nxt)
    printf("[#]  [<0x%x, 0x%x, 0x%x>] %u bytes\n",
	   tmp->prv, tmp, tmp->nxt, tmp->size);

  printf("[#] dumping used chuncks:\n");

  for (tmp = alloc.use; tmp != NULL; tmp = tmp->nxt)
    printf("[#]  [<0x%x, 0x%x, 0x%x] %u bytes\n",
	   tmp->prv, tmp, tmp->nxt, tmp->size);
}

/*
 * this fonction reallocates memory, meaning this function resizes a memory
 * area.
 *
 * the algorithm used is the simplest existing. the function first allocates
 * memory, then copies the data from the previous area to the new one and
 * finally releases the old pointer.
 *
 * steps:
 *
 * 1) calls malloc() if the pointer is null.
 * 2) checks if the chunck is well used.
 * 3) reallocates memory.
 * 4) copies the data from the old area to the new one.
 * 5) releases the previous area.
 */

void*			realloc(void* 				ptr,
				size_t				size)
{
  t_chunck*		chunck = (t_chunck*)((char*)ptr - sizeof(t_chunck));
  void*			new;
  size_t		s;

  /*
   * 1)
   */

  if (ptr == NULL)
    return (malloc(size));

  /*
   * 2)
   */

  if (chunck->state != ALLOC_STATE_USE)
    return (NULL);

  /*
   * 3)
   */

  if ((new = malloc(size)) == NULL)
    return (NULL);

  /*
   * 4)
   */

  s = size < chunck->size ? size : chunck->size;

  memcpy(new, ptr, s);

  /*
   * 5)
   */

  free(ptr);

  return (new);
}

/*
 * this function initialises the allocator.
 *
 * the arguments addr and size specify a kind of survey area. the kernel
 * will call this allocator to dynamically reserve memory while no
 * kernel memory manager is installed yet. so this area will be used in a
 * very special way.
 *
 * the argument fit specify the algorithm to use. for the moment, the only
 * fit implemented is the first fit.
 *
 * steps:
 *
 * 1) checks for the given algorithm.
 * 2) initialises the very first static area, the survey area.
 * 3) initialises the global allocator information.
 * 4) if needed, performs the allocator tests.
 */

int			alloc_init(t_vaddr			addr,
				   t_size			size,
				   t_fit			fit)
{
  t_chunck*		init;

  /*
   * 1)
   */

  if (fit != FIT_FIRST)
    return (-1);

  /*
   * 2)
   */

  init = (t_chunck*)addr;

  memset(init, 0x0, sizeof(t_alloc));

  init->state = ALLOC_STATE_FREE;
  init->size = size - sizeof(t_chunck);
  init->prv = NULL;
  init->nxt = NULL;

  /*
   * 3)
   */

  memset(&alloc, 0x0, sizeof(t_alloc));

  alloc.use = NULL;
  alloc.free = init;
  alloc.nalloc = 0;
  alloc.nfree = 0;

  /*
   * 4)
   */

#ifdef ALLOC_DEBUG
  alloc_test();
#endif

  return 0;
}

/*
 * this function performs some tests.
 */

void			alloc_test(void)
{
  void*			ptr;

  if ((ptr = malloc(7)) == NULL)
    printf("error: malloc()\n");

  if ((ptr = malloc(0)) == NULL)
    printf("error: malloc()\n");

  if ((ptr = malloc(1)) == NULL)
    printf("error: malloc()\n");

  if ((ptr = malloc(70)) == NULL)
    printf("error: malloc()\n");

  if ((ptr = malloc(200)) == NULL)
    printf("error: malloc()\n");

  if ((ptr = malloc(2)) == NULL)
    printf("error: malloc()\n");

  if ((ptr = malloc(1000)) == NULL)
    printf("error: malloc()\n");

  free(ptr);

  if ((ptr = malloc(3)) == NULL)
    printf("error: malloc()\n");

  free(ptr);

  if ((ptr = malloc(2000)) == NULL)
    printf("error: malloc()\n");

  if ((ptr = realloc(ptr, 15)) == NULL)
    printf("error: malloc()\n");

  alloc_dump();

  while (1)
    ;
}
