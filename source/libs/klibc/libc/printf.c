/*
 * kaneton
 *
 * printf.c
 *
 * path          /home/mycure/kaneton/libs/klibc
 *
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 *
 * started on    Thu May  6 14:37:44 2004   mycure
 * last update   Fri Jun 10 00:11:30 2005   mycure
 */

#include <arch/machdep/machdep.h>
#include <klibc/include/klibc.h>

void			printf_attr(char	attr)
{
  cons_attr(attr);
}

int			printf_char(char	c)
{
  cons_print_char(c);

  return (1);
}

int			printf_string(char	*string,
				      int	ladjust,
				      int	padchar,
				      int	padlen)
{
  unsigned int		written;
  int			pad;
  int			i;

  written = 0;

  if (string == NULL)
    return (printf_string("(null)", 0, 0, 0));

  for (i = 0; string[i]; i++)
    ;

  pad = padlen - i;

  if (pad < 0)
    pad = 0;

  if (ladjust)
    pad = -pad;

  for (; pad > 0; pad--)
    {
      if (padchar)
	written += printf_char(padchar);
      else
	written += printf_char(' ');
    }

  for (i = 0; string[i]; i++)
    written += printf_char(string[i]);

  for (; pad < 0; pad++)
    {
      if (padchar)
	written += printf_char(padchar);
      else
	written += printf_char(' ');
    }

  return (written);
}

int			printf_long(long	value,
				    int		base,
				    int		hdl_sign,
				    int		ladjust,
				    int		padchar,
				    int		padlen)
{
  char			convert[256 + 1];
  unsigned int		written;
  int			caps;
  char			sign;
  int			pad;
  long			v;
  int			i;

  written = 0;
  caps = 0;
  sign = 0;
  v = value;

  if (hdl_sign)
    if (v < 0)
      {
	sign = '-';
	v = -v;
      }

  if (base < 0)
    {
      caps = 1;
      base = -base;
    }

  for (i = 0; (v != 0) || (i == 0); i++)
    {
      convert[i] = (caps ? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" :
		    "0123456789abcdefghijklmnopqrstuvwxyz"
		    )[v % (unsigned int) base];
      v = (v / (unsigned int) base);
    }
  convert[i] = 0;

  pad = padlen - i;
  if (pad < 0)
    pad = 0;

  if (ladjust)
    pad = -pad;

  if (pad > 0)
    {
      if (sign)
	{
	  written += printf_char(sign);
	  sign = 0;
	  pad--;
	}

      for (; pad > 0; pad--)
	{
	  if (padchar)
	    written += printf_char(padchar);
	  else
	    written += printf_char(' ');
	}
    }

  if (sign)
    written += printf_char(sign);

  for (i--; i >= 0; i--)
    written += printf_char(convert[i]);

  for (; pad < 0; pad++)
    {
      if (padchar)
	written += printf_char(padchar);
      else
	written += printf_char(' ');
    }

  return (written);
}

int			vprintf(const char	*fmt,
				va_list		args)
{
  int			longflag;
  int			ladjust;
  unsigned int		written;
  char			padchar;
  int			padlen;
  long			value;
  char			c;
  int			i;

  for (written = 0, i = 0; fmt[i]; i++)
    {
      padchar = 0;
      padlen = 0;
      longflag = 0;

      switch (fmt[i])
	{
	case '%':
	  {
	    ladjust = 0;

	  _next_char_:

	    c = fmt[++i];

	    switch (c)
	      {
	      case 0:
		{
		  return (written);
		  break;
		}
	      case '-':
		{
		  ladjust = 1;
		  goto _next_char_;
		}
	      case '0':
		if (padlen == 0)
		  padchar = '0';
	      case '1': case '2': case '3':
	      case '4': case '5': case '6':
	      case '7': case '8': case '9':
		padlen = padlen * 10 + c - '0';
		goto _next_char_;
	      case 'l':
		longflag = 1;
		goto _next_char_;
	      case '#':
		{
		  value = (long) va_arg(args, int);

		  printf_attr(value);

		  break;
		}
	      case 'b':
		{
		  if (longflag)
		    value = va_arg(args, long);
		  else
		    value = (long) va_arg(args, int);

		  written += printf_long(value, 2, 1, ladjust,
					 padchar, padlen);

		  break;
		}
	      case 'd': case 'i':
		{
		  if (longflag)
		    value = va_arg(args, long);
		  else
		    value = (long) va_arg(args, int);

		  written += printf_long(value, 10, 1, ladjust,
					 padchar, padlen);

		  break;
		}
	      case 'u':
		{
		  if (longflag)
		    value = va_arg(args, long);
		  else
		    value = (long) va_arg(args, int);

		  written += printf_long(value, 10, 0, ladjust,
					 padchar, padlen);

		  break;
		}
	      case 'o':
		{
		  if (longflag)
		    value = va_arg(args, long);
		  else
		    value = (long) va_arg(args, int);

		  written += printf_long(value, 8, 0, ladjust,
					 padchar, padlen);

		  break;
		}
	      case 'x':
		{
		  if (longflag)
		    value = va_arg(args, long);
		  else
		    value = (long) va_arg(args, int);

		  written += printf_long(value, 16, 0, ladjust,
					 padchar, padlen);

		  break;
		}
	      case 'X':
		{
		  if (longflag)
		    value = va_arg(args, long);
		  else
		    value = (long) va_arg(args, int);

		  written += printf_long(value, -16, 0, ladjust,
					 padchar, padlen);

		  break;
		}
	      case 's':
		{
		  value = (long) va_arg(args, char *);

		  written += printf_string((char *) value, ladjust,
					   padchar, padlen);

		  break;
		}
	      case 'c':
		{
		  value = (long) va_arg(args, int);

		  written += printf_char(value);

		  break;
		}
	      case '%':
		{
		  written += printf_char('%');

		  break;
		}
	      default:
		{
		  return (written);
		}
	      }

	    break;
	  }
	default:
	  {
	    written += printf_char(fmt[i]);
	    break;
	  }
	}
    }

  va_end(args);

  return (written);
}

int			printf(char		*fmt,
			       ...)
{
  unsigned int		written;
  va_list		args;

  va_start(args, fmt);

  written = vprintf(fmt, args);

  va_end(args);

  return (written);
}
