/*
 * copyright quintard julien
 *
 * kaneton
 *
 * malloc.c
 *
 * path          /home/mycure/data/research/projects/kaneton/source
 *
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 *
 * started on    Fri Feb 11 02:50:21 2005   mycure
** Last update Sun Jul  3 23:35:23 2005 Christophe Eymard
 */

/*
 * This macro aligns an address to a multiple of the size of a long int.
 * it is used to ensure the processor uses the minimal number of cycles
 * to fetch the adress.
 * This macro shall be used in both implementations.
 */

#define __align(Addr)  (Addr % sizeof (t_vaddr) ? ((Addr) + sizeof (t_vaddr) -\
    (((t_vaddr) Addr + 1) % sizeof (t_vaddr) - 1)) : Addr)

/*
 * ---------- information -----------------------------------------------------
 *
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc/include/klibc.h>
#include <klibc/include/libdata/alloc.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

t_alloc			alloc;

/*
 * ---------- functions -------------------------------------------------------
 */

#ifdef ALLOC_FIRST_FIT

/*
 * Here follows a really basic implementation of the first fit algorithm, known
 * for its extreme slowness. The only reason it was implemented here was to
 * have a malloc to pursue developpment.
 * Later, a buddy system algorithm shall be implemented.
 */

# define __next_chunk(Chunk) (void *) ((t_vaddr) Chunk + Chunk->size + \
    sizeof (t_chunk))
# define __prev_chunk(Chunk) (void *) (Chunk->prv)
# define __chunk_size(Chunk) (Chunk->size + sizeof (t_chunk))
# define __chunk_ptr(Chunk) ((void *) ((t_vaddr) Chunk + sizeof (t_chunk)))

/*
 * This value is used to specify when we agree to fragment a chunk :
 * for example, it would be useless to fragment a chunk when there
 * is just enough space
 */
# define __TOLERANCE  (3 * sizeof (t_chunk))

/*!
 * @brief 	Segments a contiguous zone of allocation
 *
 *   Here we take a chunk, make it of size "size", and make a new
 *   chunk right after it. It is the contrary of merge.
 *
 * @param chunk		The chunk we want to fragment
 * @param size		The size we want it to be
 *
 * @return 0 if all went smoothly, 1 otherwise.
 */
static int		fragment(t_chunk *chunk, size_t size)
{
  t_chunk		*new = NULL;
  size_t		size_save = chunk->size;

  /*if (!(chunk->flags & ALLOC_FREE))
    return 1;*/
  chunk->size = __align(size);
  new = __next_chunk(chunk);
  new->size = size_save - sizeof (t_chunk) - size;
  new->flags = ALLOC_FREE;
  new->prv = chunk;
  if (chunk->flags & ALLOC_LAST)
  {
    chunk->flags &= ~(ALLOC_LAST);
    new->flags |= ALLOC_LAST;
  }
  return 0;
}

/*!
 * @brief Merge free chunks
 *
 *   There are two steps in the merging process :
 *     1) We merge all the previous free blocks
 *     2) We merge all the next free blocks with this one.
 *
 * @param chunk		The starting chunk we want to merge.
 */
static t_chunk		*merge(t_chunk *chunk)
{
  t_chunk		*runner = NULL;

  /*
   * 1)
   */
  for (runner = __prev_chunk(chunk);
      runner && runner->flags & ALLOC_FREE;
      runner = __prev_chunk(runner))
  {
    runner->size += __chunk_size(chunk);
    chunk = runner;
  }
  /*
   * 2)
   */
  if (!(chunk->flags & ALLOC_LAST))
    for (runner = __next_chunk(chunk);
	runner->flags & ALLOC_FREE && !(runner->flags & ALLOC_LAST);
	)
    {
      chunk->size += __chunk_size(runner);
      runner = __next_chunk(runner);
      runner->prv = (void *) chunk;
    }
  if (runner->flags == (ALLOC_LAST | ALLOC_FREE))
  {
    chunk->size += __chunk_size(runner);
    runner = __next_chunk(runner);
    runner->prv = (void *) chunk;
    chunk->flags |= ALLOC_LAST;
  }
  return chunk;
}

/*!
 * @brief Allocate memory
 *
 * @param size		The size to allocate
 *
 * @return a pointer to the newly allocated space
 */
void*			malloc(size_t				size)
{
  t_chunk		*runner = NULL;

  for (runner = alloc.first_blk;
      !(runner->flags & ALLOC_FREE) || __align(size) > runner->size ||
      !(runner->flags & ALLOC_LAST);
      runner = __next_chunk(runner))
    ;
  if (runner->flags & ALLOC_LAST && !(runner->flags & ALLOC_FREE))
    return NULL; /* ENOMEM */
  if (runner->size - __align(size) > __TOLERANCE)
    fragment(runner, __align(size));
  runner->flags &= ~ALLOC_FREE;
  return __chunk_ptr(runner);
}

/*!
 * @brief Free a previously allocated area.
 *
 * @param ptr		the pointer to the area.
 */
void			free(void				*ptr)
{
  t_chunk		*chunk;

  chunk = ptr;
  chunk -= 1;
  chunk->flags |= ALLOC_FREE;
  merge(chunk);
}


static void		dump_chunk(t_chunk *chunk)
{
  printf("%x : %d bytes, flags : %x \n", (t_vaddr) chunk,
      chunk->size,
      chunk->flags);
}

void			alloc_dump()
{
  t_chunk		*runner = NULL;

  printf("-- Dumping allocation state : \n");
  for (runner = alloc.first_blk;
      !(runner->flags & ALLOC_LAST);
      runner = __next_chunk(runner))
    dump_chunk(runner);
  dump_chunk(runner);
}

/*!
 * @brief Initialiaze the allocation structure.
 *
 * The initialisation is done in two passes.
 * 1) It inits the main structure to contain : the address of the
 *   first block, the whole size, the adress of the whole free space,
 *   and the adress of the heap (useful if we want to modify the space later)
 *
 * 2) It initialises the first (and last) block, filling it with the size of
 *   the space it contains, the flags, and the location of the previous block,
 *   which is zero, since there isn't any yet.
 *
 *   Now, we're ready to rock.
 *   Folks, remember that for the sake of optimization, we try to keep all the
 *   addresses aligned, so don't mind the funky pointer arithmetics.
 *
 * @param addr	The adress of the beginning of the heap
 * @param size	The size of it all
 *
 * @return	zero. I don't even understand why I should say that it didn't
 * 		went smooth.
 */
int			alloc_init(t_vaddr			addr,
				   t_size			size)
{
  memset(&alloc, 0x0, sizeof(t_alloc));

  /*
   * 1)
   */
  alloc.addr = addr;
  alloc.first_blk = (t_chunk *) __align(addr);
  alloc.size = size;
  alloc.heap = addr + sizeof (t_chunk);

  /*
   * 2)
   */
  alloc.first_blk->size = size - sizeof (t_chunk) -
    (addr - (t_vaddr) alloc.first_blk);
  alloc.first_blk->flags = ALLOC_FREE | ALLOC_LAST;
  alloc.first_blk->prv = NULL;


  /* test block follows */
  /*{
    void		*test, *toto, *tata, *titi;

    test = malloc(100);
    toto = malloc(200);
    tata = malloc(300);
    titi = malloc(400);
    alloc_dump();
    free(toto);
    free(titi);
    free(tata);
    alloc_dump();
    free(test);
    alloc_dump();
    while (1)
      ;
  }*/

  return 0;
}

#endif
