/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/library/include/types.h
 *
 * created       julien quintard   [sun jun 10 17:37:19 2007]
 * updated       julien quintard   [mon dec 13 15:42:59 2010]
 */

#ifndef LIBRARY_TYPES_H
#define LIBRARY_TYPES_H		1

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * XXX
 *
 * here #ifdef should appear to enable types depending on the underlying
 * architecture
 *
 * more generally, this file should be reviewed!
 */

typedef unsigned char		u_char;
typedef unsigned short		u_short;
typedef unsigned int		u_int;
typedef unsigned long long int	u_long;

typedef u_int			paddr_t;
typedef u_int			psize_t;
typedef u_int			vaddr_t;
typedef u_int			vsize_t;

typedef char			int8_t;
typedef u_char			u_int8_t;
typedef short			int16_t;
typedef u_short			u_int16_t;
typedef int			int32_t;
typedef u_int			u_int32_t;
typedef long long int		int64_t;
typedef u_long			u_int64_t;

typedef u_int8_t		uint8_t;
typedef u_int16_t		uint16_t;
typedef u_int32_t		uint32_t;
typedef u_int64_t		uint64_t;


typedef u_int			register_t;

typedef int64_t			blkcnt_t;	/* fs block count */
typedef u_int32_t		blksize_t;	/* fs optimal block size */
typedef char *			caddr_t;	/* core address */
typedef int32_t			daddr_t;	/* disk address */
typedef u_int32_t		dev_t;		/* device number */
typedef u_int32_t		gid_t;		/* group id */
typedef u_int32_t		ino_t;		/* inode number */
typedef u_int32_t		mode_t;		/* permissions */
typedef u_int16_t		umode_t;	/* extended permissions */
typedef u_int32_t		nlink_t;	/* link count */
typedef u_long			off_t;		/* file offset */
typedef int64_t			quad_t;		/* signed quad */
typedef u_int64_t		u_quad_t;	/* unsigned quad */
typedef int64_t			pid_t;		/* process id */
typedef u_long			rlim_t;		/* resource limit */
typedef int32_t			segsz_t;	/* segment size */
typedef u_int32_t		uid_t;		/* user id */

typedef unsigned int		size_t;		/* sizeof */
typedef signed int		ssize_t;	/* byte count or error */

typedef long			time_t;

typedef long long		fpos_t;

#endif
