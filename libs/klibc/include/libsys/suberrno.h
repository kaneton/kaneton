/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/libs/klibc/include/libsys/suberrno.h
 *
 * created       julien quintard   [fri feb 11 02:40:57 2005]
 * updated       julien quintard   [fri mar 10 03:46:54 2006]
 */

#ifndef LIBSYS_SUBERRNO_H
#define LIBSYS_SUBERRNO_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * ranges:
 *	0 - 64		contain general subernos
 *	65 - ...	contain services specific suberrnos
 */

/*
 * EPERM			100	operation not permitted
 */

#define EACCESS			101	/* permission denied */
#define EEXIST			102	/* file exists */
#define ENOTBLK			103	/* block device required */
#define EXDEV			104	/* cross-device link */
#define ESPIPE			105	/* illegal seek */
#define EROFS			106	/* read-only file system */

/*
 * ENOENT			200	no entity
 */

#define EPROC			201	/* no such process */
#define EBADF			202	/* bad file descriptor */
#define ECHILD			203	/* no child process */
#define ENOMEM			204	/* cannot allocate memory */

/*
 * EIO				300	input/output error
 */

#define ENXIO			301	/* device not configured */
#define ENODEV			302	/* operation not supported by device */
#define ENOTTY			303	/* inappropriate ioctl for device */
#define ENOSPC			304	/* no space left on device */

/*
 * E2BIG			400	element too long
 */

#define ENFILE			401	/* too many open files in system */
#define EMFILE			402	/* too many open files */
#define EFBIG			403	/* file too large */
#define EMLINK			404	/* too many links */
#define ERANGE			405	/* result too large */

/*
 * ENOEXEC			500	exec format error
 */

/*
 * EBAD				600	bad element
 */

#define	EFAULT			601	/* bad address */
#define ENOTDIR			602	/* not a directory */
#define EISDIR			603	/* is a directory */
#define EINVAL			604	/* invalid argument */
#define EPIPE			605	/* broken pipe */
#define EDOM			606	/* numerical argument out of domain */

/*
 * EBUSY			700	object busy
 */

#define ETXTBSY			701	/* text file busy */

#endif
