/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/.../boot/bootloader/qemu-mips.mips64/libc.c
 *
 * created       matthieu bucchianeri   [thu jun 28 16:14:31 2007]
 * updated       enguerrand raymond   [mon apr 27 07:26:42 2009]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "libc.h"

/*
 * ---------- macros ----------------------------------------------------------
 */

#define GET_VAL(Sign)							\
  if (quadflag)								\
    quadvalue = va_arg(args, t_sint64);					\
  else									\
    {									\
      if ((Sign))							\
	{								\
	  if (longflag)							\
	    quadvalue = (t_uint64) va_arg(args, long);			\
	  else								\
	    quadvalue = (t_uint64) va_arg(args, int);			\
	}								\
      else								\
	if (longflag)							\
	  quadvalue = (t_uint64) va_arg(args, unsigned long);		\
	else								\
	  quadvalue = (t_uint64) va_arg(args, unsigned int);		\
    }

/*
 * ---------- globals ---------------------------------------------------------
 */

t_printf_char_fn	printf_char = NULL;
t_printf_attr_fn	printf_attr = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

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
    return (printf_string("(null)", flags, len1, len2));

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

  for (i = 0; (string[i] != 0) && (i < (len1 != -1 ? len1 : i + 1)); i++)
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

int			printf_quad(t_sint64			value,
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
  t_uint64		v;
  int			i;

  if (flags & PRINTF_DOT)
    PRINTF_SWAP(len1, len2);

  written = 0;
  caps = 0;
  sign = 0;
  v = value;

  if (hdl_sign)
    if (value < 0)
      {
	sign = '-';
	v = -value;
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
      for (; padlen > 0; padlen--)
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
  t_sint64		quadvalue;
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
		  GET_VAL(0);
		  written += printf_quad(quadvalue, 2, 0, flags,
					 len1, len2);

		  break;
		}
	      case 'd': case 'i': case 'D':
		{
		  GET_VAL(1);
		  written += printf_quad(quadvalue, 10, 1, flags,
					 len1, len2);

		  break;
		}
	      case 'u': case 'U':
		{
		  GET_VAL(0);
		  written += printf_quad(quadvalue, 10, 0, flags,
					 len1, len2);

		  break;
		}
	      case 'o': case 'O':
		{
		  GET_VAL(0);
		  written += printf_quad(quadvalue, 8, 0, flags,
					 len1, len2);

		  break;
		}
	      case 'p': case 'x':
		{
		  GET_VAL(0);
		  written += printf_quad(quadvalue, 16, 0, flags,
					 len1, len2);

		  break;
		}
	      case 'X':
		{
		  GET_VAL(0);
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
		  if (printf_char != NULL)
		    {
		      written += printf_char('%');
		      written += printf_char(fmt[i]);
		    }
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

size_t			strlen(const char*			str)
{
  t_sint32		i;

  for (i = 0; str[i]; i++)
    ;

  return (i);
}

void*			memcpy(void*				dest,
			       const void*			src,
			       size_t				n)
{
  char*			d = dest;
  const char*		s = src;
  t_sint32		i;

  if ((n == 0) || (src == dest))
    return (NULL);

  /*
   * XXX bad performances
   */

  for (i = 0; i < n; i++)
    d[i] = s[i];

  return (d);
}

void*			memset(void*				s,
			       int				c,
			       size_t				n)
{
  char*			d = s;
  t_sint32		i;

  for (i = 0; i < n; i++)
    d[i] = c;

  return (d);
}

char*			strcpy(char*				to,
			       const char*			from)
{
  t_sint32	i;

  for (i = 0; from[i]; i++)
    to[i] = from[i];
  to[i] = 0;

  return (to);
}
