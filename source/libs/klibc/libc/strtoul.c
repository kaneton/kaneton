/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * strtoul.c
 * 
 * path          /home/mycure/kaneton/src/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:56:28 2005   mycure
 * last update   Sat Apr 23 17:00:37 2005   mycure
 */

#include <klibc/include/klibc.h>

u_long			strtoul(const char	*nptr,
				char		**endptr,
				int		base)
{
  const char		*s = nptr;
  unsigned long		cutoff;
  int			cutlim;
  unsigned long		acc;
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

  cutoff = ULONG_MAX / (unsigned long) base;
  cutlim = (int) (ULONG_MAX % (unsigned long) base);

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

      if (acc > cutoff || (acc == cutoff && c > cutlim))
	{
	  any = -1;
	  acc = ULONG_MAX;

	  errno = E2BIG;
	  suberrno = ERANGE;
	}
      else
	{
	  any = 1;
	  acc *= (unsigned long) base;
	  acc += c;
	}
    }

  if (neg != 0 && any > 0)
    acc = -acc;

  if (endptr != 0)
    *endptr = (char *) (any ? s - 1 : nptr);

  return (acc);
}
