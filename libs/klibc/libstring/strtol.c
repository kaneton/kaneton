/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * strtol.c
 * 
 * path          /home/mycure/kaneton/libs/klibc/libstring
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:56:18 2005   mycure
 * last update   Wed Aug 31 16:32:42 2005   mycure
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>

/*
 * ---------- functions -------------------------------------------------------
 */

long			strtol(const char*			nptr,
			       char**				endptr,
			       int				base)
{
  const char*		s = nptr;
  long			cutoff;
  int			cutlim;
  long			acc;
  int			neg;
  int			any;
  int			c;

  /*
   * XXX no check, assume that the string is correct
   */

  do
    {
      c = (unsigned char) *s++;
    } while (isspace(c));

  if (c == '-')
    {
      neg = 1;
      c = *s++;
    }
  else
    {
      neg = 0;

      if (c == '+')
	c = *s++;
    }

  if ((base == 0 || base == 16) &&
      (c == '0' && (*s == 'x' || *s == 'X')))
    {
      c = s[1];
      s += 2;
      base = 16;
    }

  if (base == 0)
    base = c == '0' ? 8 : 10;

  cutoff = neg ? SLONG_MIN : SLONG_MAX;
  cutlim = (int) (cutoff % base);
  cutoff /= base;

  if (neg == 1)
    {
      if (cutlim > 0)
	{
	  cutlim -= base;
	  cutoff += 1;
	}
      cutlim = -cutlim;
    }

  for (acc = 0, any = 0; ; c = (unsigned char) *s++)
    {
      if (isdigit(c))
	c -= '0';
      else if (isalpha(c))
	c -= isupper(c) ? 'A' - 10 : 'a' - 10;
      else
	break;

      if (c >= base)
	break;
      if (any < 0)
	continue;
      if (neg == 1)
	{
	  if (acc < cutoff || (acc == cutoff && c > cutlim))
	    {
	      any = -1;
	      acc = SLONG_MIN;

	      /* XXX
	      errno = E2BIG;
	      suberrno = ERANGE;
	      */
	    }
	  else
	    {
	      any = 1;
	      acc *= base;
	      acc -= c;
	    }
	}
      else
	{
	  if (acc > cutoff || (acc == cutoff && c > cutlim))
	    {
	      any = -1;
	      acc = SLONG_MAX;

	      /* XXX
	      errno = E2BIG;
	      suberrno = ERANGE;
	      */
	    }
	  else
	    {
	      any = 1;
	      acc *= base;
	      acc += c;
	    }
	}
    }

  if (endptr != 0)
    *endptr = (char*) (any ? s - 1 : nptr);

  return (acc);
}
