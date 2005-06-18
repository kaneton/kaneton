/*
 * kaneton
 *
 * printf.c
 *
 * path          /home/mycure/kaneton/libs/klibc/libc
 *
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 *
 * started on    Thu May  6 14:37:44 2004   mycure
 * last update   Sat Jun 18 15:50:06 2005   mycure
 */

#include <arch/machdep/machdep.h>
#include <klibc/include/klibc.h>

t_printf_char_fn	printf_char = NULL;
t_printf_attr_fn	printf_attr = NULL;

int			printf_string(char*			string,
				      unsigned int		flags,
				      int			len1,
				      int			len2)
{
  unsigned int		written;
  int			padlen;
  int			i;

  written = 0;

  if (string == NULL)
    return (printf_string("(null)", 0, 0, 0));

  for (i = 0; string[i]; i++)
    ;

  if (!(flags & PRINTF_DOT))
    {
      padlen = len1 - i;

      if (padlen < 0)
	padlen = 0;

      if (flags & PRINTF_LADJUST)
	padlen = -padlen;

      for (; padlen > 0; padlen--)
	if (printf_char != NULL)
	  written += printf_char(' ');
    }

  for (i = 0; (string[i] != 0) && (i < len1 != -1 ? len1 : i + 1); i++)
    if (printf_char != NULL)
      written += printf_char(string[i]);

  if (!(flags & PRINTF_DOT))
    {
      for (; padlen < 0; padlen++)
	if (printf_char != NULL)
	  written += printf_char(' ');
    }

  return (written);
}

int			printf_quad(quad_t			value,
				    int				base,
				    int				hdl_sign,
				    unsigned int		flags,
				    int				len1,
				    int				len2)
{
  char			convert[256];
  unsigned int		written;
  int			padlen;
  int			caps;
  char			sign;
  u_quad_t		v;
  int			i;

  if (flags & PRINTF_DOT)
    PRINTF_SWAP(len1, len2);

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

  padlen = len1 - i;

  if (padlen < 0)
    padlen = 0;

  if (flags & PRINTF_LADJUST)
    padlen = -padlen;

  if ((flags & PRINTF_ZPAD) && (padlen > 0))
    {
      if (sign)
	{
	  if (printf_char != NULL)
	    written += printf_char(sign);
	  sign = 0;
	  padlen--;
	}

      for (; padlen > 0; padlen--)
	if (printf_char != NULL)
	  written += printf_char('0');
    }

  if (padlen > 0)
    {
      if (printf_char != NULL)
	written += printf_char(' ');
    }

  if (sign)
    if (printf_char != NULL)
      written += printf_char(sign);

  for (i--; i >= 0; i--)
    if (printf_char != NULL)
      written += printf_char(convert[i]);

  for (; padlen < 0; padlen++)
    {
      if (printf_char != NULL)
	written += printf_char(' ');
    }

  return (written);
}

int			vprintf(const char*			fmt,
				va_list				args)
{
  quad_t		quadvalue;
  char			charvalue;
  char*			strvalue;
  int			quadflag;
  int			longflag;
  int			starflag;
  unsigned int		written;
  unsigned int		flags;
  int			len1;
  int			len2;
  int			i;
  char			c;

  starflag = 0;
  flags = 0;
  for (written = 0, i = 0; fmt[i]; i++)
    {
      quadflag = 0;
      longflag = 0;

      switch (fmt[i])
	{
	case '%':
	  {
	    starflag = 0;
	    flags = 0;
	    len1 = -1;
	    len2 = -1;

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
		  flags |= PRINTF_LADJUST;
		  goto _next_char_;
		}
	      case '*':
		{
		  len1 = va_arg(args, int);
		  starflag = 1;
		  goto _next_char_;
		}
	      case '.':
		{
		  len2 = len1;
		  len1 = -1;
		  flags |= PRINTF_DOT;

		  goto _next_char_;
		}
	      case '0':
		{
		  if (starflag)
		    {
		      flags |= PRINTF_ZPAD;
		      goto _next_char_;
		    }

		  if (len1 == -1)
		    flags |= PRINTF_ZPAD;
		}
	      case '1': case '2': case '3':
	      case '4': case '5': case '6':
	      case '7': case '8': case '9':
		{
		  if (len1 == -1)
		    len1 = 0;
		  len1 = len1 * 10 + c - '0';
		  goto _next_char_;
		}
	      case 'q':
		{
		  quadflag = 1;
		  goto _next_char_;
		}
	      case 'l':
		{
		  longflag = 1;
		  goto _next_char_;
		}
	      case '#':
		{
		  charvalue = (char) va_arg(args, int);

		  if (printf_attr != NULL)
		    printf_attr(charvalue);

		  break;
		}
	      case 'b':
		{
		  if (quadflag)
		    quadvalue = va_arg(args, quad_t);
		  else if (longflag)
		    quadvalue = (u_quad_t) va_arg(args, long);
		  else
		    quadvalue = (u_quad_t) va_arg(args, int);

		  written += printf_quad(quadvalue, 2, 0, flags,
					 len1, len2);

		  break;
		}
	      case 'd': case 'i': case 'D':
		{
		  if (quadflag)
		    quadvalue = va_arg(args, quad_t);
		  else if (longflag)
		    quadvalue = (quad_t) va_arg(args, long);
		  else
		    quadvalue = (quad_t) va_arg(args, int);

		  written += printf_quad(quadvalue, 10, 1, flags,
					 len1, len2);

		  break;
		}
	      case 'u': case 'U':
		{
		  if (quadflag)
		    quadvalue = va_arg(args, quad_t);
		  else if (longflag)
		    quadvalue = (u_quad_t) va_arg(args, long);
		  else
		    quadvalue = (u_quad_t) va_arg(args, int);

		  written += printf_quad(quadvalue, 10, 0, flags,
					 len1, len2);

		  break;
		}
	      case 'o': case 'O':
		{
		  if (quadflag)
		    quadvalue = va_arg(args, quad_t);
		  else if (longflag)
		    quadvalue = (u_quad_t) va_arg(args, long);
		  else
		    quadvalue = (u_quad_t) va_arg(args, int);

		  written += printf_quad(quadvalue, 8, 0, flags,
					 len1, len2);

		  break;
		}
	      case 'p': case 'x':
		{
		  if (quadflag)
		    quadvalue = va_arg(args, quad_t);
		  else if (longflag)
		    quadvalue = (u_quad_t) va_arg(args, long);
		  else
		    quadvalue = (u_quad_t) va_arg(args, int);

		  written += printf_quad(quadvalue, 16, 0, flags,
					 len1, len2);

		  break;
		}
	      case 'X':
		{
		  if (quadflag)
		    quadvalue = va_arg(args, quad_t);
		  else if (longflag)
		    quadvalue = (u_quad_t) va_arg(args, long);
		  else
		    quadvalue = (u_quad_t) va_arg(args, int);

		  written += printf_quad(quadvalue, -16, 0, flags,
					 len1, len2);

		  break;
		}
	      case 's':
		{
		  strvalue = va_arg(args, char*);

		  written += printf_string(strvalue, flags,
					   len1, len2);

		  break;
		}
	      case 'c':
		{
		  charvalue = va_arg(args, int);

		  if (printf_char != NULL)
		    written += printf_char(charvalue);

		  break;
		}
	      case '%':
		{
		  if (printf_char != NULL)
		    written += printf_char(c);

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
	    if (printf_char != NULL)
	      written += printf_char(fmt[i]);
	    break;
	  }
	}
    }

  va_end(args);

  return (written);
}

int			printf_init(t_printf_char_fn		pc,
				    t_printf_attr_fn		pa)
{
  printf_char = pc;
  printf_attr = pa;

  return (0);
}

int			printf(char*				fmt,
			       ...)
{
  unsigned int		written;
  va_list		args;

  va_start(args, fmt);

  written = vprintf(fmt, args);

  va_end(args);

  return (written);
}
