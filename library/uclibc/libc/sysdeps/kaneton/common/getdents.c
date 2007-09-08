/*
 * Copyright (C) 2000-2006 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#include <alloca.h>
#include <assert.h>
#include <errno.h>
#include <dirent.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <bits/kernel_types.h>

/* With newer versions of linux, the getdents syscall returns d_type
 * information after the name field.  Someday, we should add support for
 * that instead of always calling getdents64 ...
 *
 * See __ASSUME_GETDENTS32_D_TYPE in glibc's kernel-features.h for specific
 * version / arch details.
 */

#ifndef offsetof
# define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

struct kernel_dirent
{
	long int d_ino;
	__kernel_off_t d_off;
	unsigned short int d_reclen;
	char d_name[256];
};

ssize_t __getdents (int fd, char *buf, size_t nbytes) attribute_hidden;

#if ! defined __UCLIBC_HAS_LFS__ || ! defined __NR_getdents64

libc_hidden_proto(memcpy)
libc_hidden_proto(lseek)

#define __NR___syscall_getdents __NR_getdents
static inline _syscall3(int, __syscall_getdents, int, fd, unsigned char *, kdirp, size_t, count);

ssize_t __getdents (int fd, char *buf, size_t nbytes)
{
    struct dirent *dp;
    off_t last_offset = -1;
    ssize_t retval;
    size_t red_nbytes;
    struct kernel_dirent *skdp, *kdp;
    const size_t size_diff = (offsetof (struct dirent, d_name)
	    - offsetof (struct kernel_dirent, d_name));

    red_nbytes = MIN (nbytes - ((nbytes / 
		    (offsetof (struct dirent, d_name) + 14)) * size_diff), 
	    nbytes - size_diff);

    dp = (struct dirent *) buf;
    skdp = kdp = alloca (red_nbytes);

    retval = __syscall_getdents(fd, (unsigned char *)kdp, red_nbytes);
    if (retval == -1)
	return -1;

    while ((char *) kdp < (char *) skdp + retval) {
	const size_t alignment = __alignof__ (struct dirent);
	/* Since kdp->d_reclen is already aligned for the kernel structure
	   this may compute a value that is bigger than necessary.  */
	size_t new_reclen = ((kdp->d_reclen + size_diff + alignment - 1)
		& ~(alignment - 1));
	if ((char *) dp + new_reclen > buf + nbytes) {
	    /* Our heuristic failed.  We read too many entries.  Reset
	       the stream.  */
	    assert (last_offset != -1);
	    lseek(fd, last_offset, SEEK_SET);

	    if ((char *) dp == buf) {
		/* The buffer the user passed in is too small to hold even
		   one entry.  */
		__set_errno (EINVAL);
		return -1;
	    }
	    break;
	}

	last_offset = kdp->d_off;
	dp->d_ino = kdp->d_ino;
	dp->d_off = kdp->d_off;
	dp->d_reclen = new_reclen;
	dp->d_type = DT_UNKNOWN;
	memcpy (dp->d_name, kdp->d_name,
		kdp->d_reclen - offsetof (struct kernel_dirent, d_name));
	dp = (struct dirent *) ((char *) dp + new_reclen);
	kdp = (struct kernel_dirent *) (((char *) kdp) + kdp->d_reclen);
    }
    return (char *) dp - buf;
}

#if defined __UCLIBC_HAS_LFS__ && ! defined __NR_getdents64
attribute_hidden strong_alias(__getdents,__getdents64)
#endif

#elif __WORDSIZE == 32

libc_hidden_proto(memmove)

extern __typeof(__getdents) __getdents64 attribute_hidden;
ssize_t __getdents (int fd, char *buf, size_t nbytes)
{
    struct dirent *dp;
    struct dirent64 *dp64;
    ssize_t ret = __getdents64 (fd, buf, nbytes);

    if (ret <= 0)
	return ret;

    dp64 = (struct dirent64 *) buf;
    buf += ret;
    while ((void *) dp64 < (void *) buf) {
	dp = (struct dirent *) dp64;
	dp->d_ino = dp64->d_ino;
	dp->d_off = dp64->d_off;
	dp->d_reclen = dp64->d_reclen;
	dp->d_type = dp64->d_type;
	memmove (dp->d_name, dp64->d_name, dp->d_reclen - offsetof (struct dirent64, d_name));
	memmove (dp64, dp, dp->d_reclen);
	dp64 = ((void *) dp64) + dp->d_reclen;
    }

    return ret;
}

#endif
