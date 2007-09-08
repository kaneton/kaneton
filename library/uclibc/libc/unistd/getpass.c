/* Copyright (C) 1992-1999,2001,2003,2004,2005 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

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

#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#if defined __USE_BSD || (defined __USE_XOPEN && !defined __USE_XOPEN2K)

libc_hidden_proto(strlen)
libc_hidden_proto(tcsetattr)
libc_hidden_proto(tcgetattr)
libc_hidden_proto(setvbuf)
libc_hidden_proto(fopen)
libc_hidden_proto(fclose)
libc_hidden_proto(fileno)
libc_hidden_proto(fflush)
libc_hidden_proto(fgets)
libc_hidden_proto(fputs)
libc_hidden_proto(fputc)
libc_hidden_proto(putc)
libc_hidden_proto(__fputc_unlocked)

/* It is desirable to use this bit on systems that have it.
   The only bit of terminal state we want to twiddle is echoing, which is
   done in software; there is no need to change the state of the terminal
   hardware.  */

#ifndef TCSASOFT
#define TCSASOFT 0
#endif
#define PWD_BUFFER_SIZE 256

char *
getpass (prompt)
     const char *prompt;
{
  FILE *in, *out;
  struct termios s, t;
  int tty_changed;
  static char buf[PWD_BUFFER_SIZE];
  int nread;

  /* Try to write to and read from the terminal if we can.
     If we can't open the terminal, use stderr and stdin.  */

  in = fopen ("/dev/tty", "r+");
  if (in == NULL)
    {
      in = stdin;
      out = stderr;
    }
  else
    out = in;

  /* Turn echoing off if it is on now.  */

  if (tcgetattr (fileno (in), &t) == 0)
    {
      /* Save the old one. */
      s = t;
      /* Tricky, tricky. */
      t.c_lflag &= ~(ECHO|ISIG);
      tty_changed = (tcsetattr (fileno (in), TCSAFLUSH|TCSASOFT, &t) == 0);
      if (in != stdin) {
	/* Disable buffering for read/write FILE to prevent problems with
	 * fseek and buffering for read/write auto-transitioning. */
	setvbuf(in, NULL, _IONBF, 0);
      }
    }
  else
    tty_changed = 0;

  /* Write the prompt.  */
  fputs(prompt, out);
  fflush(out);

  /* Read the password.  */
  fgets (buf, PWD_BUFFER_SIZE-1, in);
  if (buf != NULL)
    {
      nread = strlen(buf);
      if (nread < 0)
	buf[0] = '\0';
      else if (buf[nread - 1] == '\n')
	{
	  /* Remove the newline.  */
	  buf[nread - 1] = '\0';
	  if (tty_changed)
	    /* Write the newline that was not echoed.  */
	    putc('\n', out);
	}
    }

  /* Restore the original setting.  */
  if (tty_changed) {
    (void) tcsetattr (fileno (in), TCSAFLUSH|TCSASOFT, &s);
  }

  if (in != stdin)
    /* We opened the terminal; now close it.  */
    fclose (in);

  return buf;
}
#endif
