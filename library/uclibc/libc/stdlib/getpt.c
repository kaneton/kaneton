/* Copyright (C) 1998, 1999, 2001 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Zack Weinberg <zack@rabi.phys.columbia.edu>, 1998.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <paths.h>

libc_hidden_proto(open)
libc_hidden_proto(close)

#if !defined __ASSUME_DEVPTS__
# include <sys/statfs.h>

/* Constant that identifies the `devpts' filesystem.  */
# define DEVPTS_SUPER_MAGIC	0x1cd1
/* Constant that identifies the `devfs' filesystem.  */
# define DEVFS_SUPER_MAGIC	0x1373
#endif

/* Path to the master pseudo terminal cloning device.  */
#define _PATH_DEVPTMX _PATH_DEV "ptmx"
/* Directory containing the UNIX98 pseudo terminals.  */
#define _PATH_DEVPTS _PATH_DEV "pts"

#if !defined __UNIX98PTY_ONLY__
/* Prototype for function that opens BSD-style master pseudo-terminals.  */
extern int __bsd_getpt (void) attribute_hidden;
#endif

/* Open a master pseudo terminal and return its file descriptor.  */
int
getpt (void)
{
#if !defined __UNIX98PTY_ONLY__
  static int have_no_dev_ptmx;
#endif
  int fd;

#if !defined __UNIX98PTY_ONLY__
  if (!have_no_dev_ptmx)
#endif
    {
      fd = open (_PATH_DEVPTMX, O_RDWR);
      if (fd != -1)
	{
#if defined __ASSUME_DEVPTS__
	  return fd;
#else
	  struct statfs fsbuf;
	  static int devpts_mounted;

	  /* Check that the /dev/pts filesystem is mounted
	     or if /dev is a devfs filesystem (this implies /dev/pts).  */
	  if (devpts_mounted
	      || (statfs (_PATH_DEVPTS, &fsbuf) == 0
		  && fsbuf.f_type == DEVPTS_SUPER_MAGIC)
	      || (statfs (_PATH_DEV, &fsbuf) == 0	
		  && fsbuf.f_type == DEVFS_SUPER_MAGIC))
	    {
	      /* Everything is ok.  */
	      devpts_mounted = 1;
	      return fd;
	    }

	  /* If /dev/pts is not mounted then the UNIX98 pseudo terminals
             are not usable.  */
	  close (fd);
#if !defined __UNIX98PTY_ONLY__
	  have_no_dev_ptmx = 1;
#endif
#endif
	}
      else
	{
#if !defined __UNIX98PTY_ONLY__
	  if (errno == ENOENT || errno == ENODEV)
	    have_no_dev_ptmx = 1;
	  else
#endif
	    return -1;
	}
    }

#if !defined __UNIX98PTY_ONLY__
  return __bsd_getpt ();
#endif
}

#if !defined __UNIX98PTY_ONLY__
# define PTYNAME1 "pqrstuvwxyzabcde";
# define PTYNAME2 "0123456789abcdef";

# define __getpt __bsd_getpt
# include "bsd_getpt.c"
#endif
