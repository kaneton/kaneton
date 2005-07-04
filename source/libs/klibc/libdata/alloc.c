/*
 * copyright quintard julien
 *
 * kaneton
 *
 * malloc.c
 *
 * path          /home/mycure/kaneton
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
#define __align(Addr)  (Addr) + sizeof (t_vaddr) - (((t_vaddr) Addr + 1) % sizeof (t_vaddr) - 1)

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX
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

# define __next_chunk(Chunk) (void *) ((t_vaddr) Chunk + Chunk->size)
# define __prev_chunk(Chunk) (void *) (Chunk->prv)

/*!
** @brief 	Segments a contiguous zone of allocation
**
**   Here we take a chunk, make it of size "size", and make a new
**   chunk right after it. It is the contrary of merge.
**
** @param chunk		The chunk we want to fragment
** @param size		The size we want it to be
**
** @return 0 if all went smoothly, 1 otherwise.
*/
static int		fragment(t_chunk *chunk, size_t size)
{
  t_chunk		*new = NULL;
  size_t		size_save = size;

  if (!(chunk->flags & ALLOC_FREE))
    return 1;
  chunk->size = __align(size);
  new = __next_chunk(chunk);
  new->size = size_save - sizeof (t_chunk) - size;
  new->flags = ALLOC_FREE;
  if (chunk->flags & ALLOC_LAST)
  {
    chunk->flags &= ~(ALLOC_LAST);
    new->flags |= ALLOC_LAST;
  }
  return 0;
}

/*!
** @brief Merge free chunks
**
** @param chunk		The starting chunk we want to merge.
*/
static void		merge(t_chunk chunk)
{

}

void*			malloc(size_t				size)
{

}

/*!
** @brief Initialiaze the allocation structure.
**
** The initialisation is done in two passes.
** 1) It inits the main structure to contain : the address of the
**   first block, the whole size, the adress of the whole free space,
**   and the adress of the heap (useful if we want to modify the space later on)
**
** 2) It initialises the first (and last) block, filling it with the size of
**   the space it contains, the flags, and the location of the previous block,
**   which is zero, since there isn't any yet.
**
**   Now, we're ready to rock.
**   Folks, remember that for the sake of optimization, we try to keep all the
**   addresses aligned, so don't mind the funky pointer arithmetics.
**
** @param addr	The adress of the beginning of the heap
** @param size	The size of it all
**
** @return	zero. I don't even understand why I should say that it didn't
** 		went smooth.
*/
int			alloc_init(t_vaddr			addr,
				   t_size			size)
{
  memset(&alloc, 0x0, sizeof(t_alloc));

  /*
   * 1)
   */
  alloc.addr = addr;
  alloc.first_blk = (t_chunk*) __align(addr);
  alloc.size = size;
  alloc.heap = addr + sizeof (t_chunk);

  /*
   * 2)
   */
  alloc.first_blk->size = size - sizeof (t_chunk) -
    (addr - (t_vaddr) alloc.first_blk);
  alloc.first_blk->flags = ALLOC_FREE | ALLOC_LAST;
  alloc.first_blk->prv = NULL;

  return 0;
}

#endif
