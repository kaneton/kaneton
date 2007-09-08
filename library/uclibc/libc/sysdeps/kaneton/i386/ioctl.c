/* vi: set sw=4 ts=4: */
/*
 * ioctl() for uClibc
 *
 * Copyright (C) 2000-2006 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#include <sys/syscall.h>
#include <stdarg.h>
#include <sys/ioctl.h>

libc_hidden_proto(ioctl)

int __libc_ioctl(int fd, unsigned long int request, ...)
{
  return (0);
}

weak_alias(__libc_ioctl,ioctl)
libc_hidden_def(ioctl)
