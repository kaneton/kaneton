/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibc/include/libsys/types.h
 *
 * created       julien quintard   [fri feb 11 02:40:57 2005]
 * updated       matthieu bucchianeri   [mon aug 28 17:47:02 2006]
 */

#ifndef LIBSYS_TYPES_H
#define LIBSYS_TYPES_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>

/*
 * ---------- types -----------------------------------------------------------
 */

typedef unsigned char		u_char;
typedef unsigned short		u_short;
typedef unsigned int		u_int;
typedef unsigned long		u_long;

typedef t_paddr			paddr_t;
typedef t_psize			psize_t;
typedef t_vaddr			vaddr_t;
typedef t_vsize			vsize_t;

typedef t_sint8			int8_t;
typedef t_uint8			u_int8_t;
typedef t_sint16		int16_t;
typedef t_uint16		u_int16_t;
typedef t_sint32		int32_t;
typedef t_uint32		u_int32_t;
typedef t_sint64		int64_t;
typedef t_uint64		u_int64_t;

typedef int			register_t;

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
