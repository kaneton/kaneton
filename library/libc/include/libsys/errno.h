/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/library/libc/include/libsys/errno.h
 *
 * created       julien quintard   [sun jun 10 17:52:54 2007]
 * updated       julien quintard   [sun jun 10 17:53:02 2007]
 */

#ifndef LIBC_LIBSYS_ERRNO_H
#define LIBC_LIBSYS_ERRNO_H		1

/*
 * ---------- macros ----------------------------------------------------------
 */

#define EZERO			0	/* no error */

#define EPERM			100	/* operation not permitted */
#define ENOENT			200	/* no entity */
#define EIO			300	/* input/output error */
#define E2BIG			400	/* element too long */
#define ENOEXEC			500	/* exec format error */

#define EBAD			600	/* bad element */
#define EBUSY			700	/* object busy */

#endif
