/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * types.h
 * 
 * path          /home/mycure/kaneton/libs/klibc/libsys
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:40:57 2005   mycure
 * last update   Mon Jun 13 17:37:10 2005   mycure
 */

#ifndef LIBSYS_TYPES_H
#define LIBSYS_TYPES_H		1

/*
 * posix types
 */

typedef unsigned char		u_char;
typedef unsigned short		u_short;
typedef unsigned int		u_int;
typedef unsigned long		u_long;

typedef unsigned long		paddr_t;
typedef unsigned long		psize_t;
typedef unsigned long		vaddr_t;
typedef unsigned long		vsize_t;

typedef char			int8_t;
typedef unsigned char		u_int8_t;
typedef short			int16_t;
typedef unsigned short		u_int16_t;
typedef int			int32_t;
typedef unsigned int		u_int32_t;
typedef long long		int64_t;
typedef unsigned long long	u_int64_t;

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
