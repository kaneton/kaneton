/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibrary/libstring/strtol.c
 *
 * created       julien quintard   [fri feb 11 02:56:44 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:56:25 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <library/library.h>
#include <library/limits.h>
#include <library/ctype.h>

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
