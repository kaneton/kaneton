/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/bootloader/arch/sgi-o2.mips64/init.h
 *
 * created       enguerrand raymond   [sun nov 19 16:17:50 2006]
 * updated       enguerrand raymond   [sun jan  7 00:31:23 2007]
 */

#ifndef INIT_H
#define INIT_H		1

/*
 * ---------- macros ----------------------------------------------------------
 */

#define nbr_page(_size_)			\
(						\
{						\
  t_uint32	_nbr_page_ = _size_ / PAGESZ;	\
						\
  if ((_size_ % PAGESZ) != 0)			\
    ++_nbr_page_;				\
_nbr_page_;					\
}						\
						)

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	init.c
 */

/*
 * init.c
 */

t_uint32	bootloader_alloc(t_uint64	size);

t_uint32		bootloader_memory_size(void);

void	bootloader_segments_init(void);

void			bootloader_init(void);


/*
 * eop
 */

#endif
