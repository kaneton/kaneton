/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/libs/klibrary/libstring/printf.c
 *
 * created       julien quintard   [thu may  6 14:37:44 2004]
 * updated       julien quintard   [fri mar 10 03:49:08 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <library/library.h>

/*
 * ---------- functions -------------------------------------------------------
 */

int			format_string(t_format_character_fn	character,
				      t_format_attribute_fn	attribute,
				      char*			string,
				      unsigned int		flags,
				      int			len1,
				      int			len2)
{
  unsigned int		written;
  int			padlen;
  int			i;

  written = 0;

  if (string == NULL)
    return (format_string(character, attribute,
			  "(null)", flags, len1, len2));

  for (i = 0; string[i]; i++)
    ;

  if (!(flags & FORMAT_DOT))
    {
      padlen = len1 - i;

      if (padlen < 0)
	padlen = 0;

      if (flags & FORMAT_LADJUST)
	padlen = -padlen;

      for (; padlen > 0; padlen--)
	if (character != NULL)
	  {
	    character(' ');

	    written++;
	  }
    }

  for (i = 0; (string[i] != 0) && (i < (len1 != -1 ? len1 : i + 1)); i++)
    if (character != NULL)
      {
	character(string[i]);

	written++;
      }

  if (!(flags & FORMAT_DOT))
    {
      for (; padlen < 0; padlen++)
	if (character != NULL)
	  {
	    character(' ');

	    written++;
	  }
    }

  return (written);
}

int			format_quad(t_format_character_fn	character,
				    t_format_attribute_fn	attribute,
				    quad_t			value,
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

  if (flags & FORMAT_DOT)
    FORMAT_SWAP(len1, len2);

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

  if (flags & FORMAT_LADJUST)
    padlen = -padlen;

  if ((flags & FORMAT_ZPAD) && (padlen > 0))
    {
      if (sign)
	{
	  if (character != NULL)
	    {
	      character(sign);

	      written++;
	    }
	  sign = 0;
	  padlen--;
	}

      for (; padlen > 0; padlen--)
	if (character != NULL)
	  {
	    character('0');

	    written++;
	  }
    }

  if (padlen > 0)
    {
      for (; padlen > 0; padlen--)
	if (character != NULL)
	  {
	    character(' ');

	    written++;
	  }
    }

  if (sign)
    if (character != NULL)
      {
	character(sign);

	written++;
      }

  for (i--; i >= 0; i--)
    if (character != NULL)
      {
	character(convert[i]);

	written++;
      }

  for (; padlen < 0; padlen++)
    {
      if (character != NULL)
	{
	  character(' ');

	  written++;
	}
    }

  return (written);
}

int			format(t_format_character_fn		character,
			       t_format_attribute_fn		attribute,
			       const char*			fmt,
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
		  flags |= FORMAT_LADJUST;
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
		  flags |= FORMAT_DOT;

		  goto _next_char_;
		}
	      case '0':
		{
		  if (starflag)
		    {
		      flags |= FORMAT_ZPAD;
		      goto _next_char_;
		    }

		  if (len1 == -1)
		    flags |= FORMAT_ZPAD;
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

		  if (attribute != NULL)
		    attribute(charvalue);

		  break;
		}
	      case 'b':
		{
		  FORMAT_VALUE(0);
		  written += format_quad(character, attribute,
					 quadvalue, 2, 0, flags,
					 len1, len2);

		  break;
		}
	      case 'd': case 'i': case 'D':
		{
		  FORMAT_VALUE(1);
		  written += format_quad(character, attribute,
					 quadvalue, 10, 1, flags,
					 len1, len2);

		  break;
		}
	      case 'u': case 'U':
		{
		  FORMAT_VALUE(0);
		  written += format_quad(character, attribute,
					 quadvalue, 10, 0, flags,
					 len1, len2);

		  break;
		}
	      case 'o': case 'O':
		{
		  FORMAT_VALUE(0);
		  written += format_quad(character, attribute,
					 quadvalue, 8, 0, flags,
					 len1, len2);

		  break;
		}
	      case 'p': case 'x':
		{
		  FORMAT_VALUE(0);
		  written += format_quad(character, attribute,
					 quadvalue, 16, 0, flags,
					 len1, len2);

		  break;
		}
	      case 'X':
		{
		  FORMAT_VALUE(0);
		  written += format_quad(character, attribute,
					 quadvalue, -16, 0, flags,
					 len1, len2);

		  break;
		}
	      case 's':
		{
		  strvalue = va_arg(args, char*);

		  written += format_string(character, attribute,
					   strvalue, flags,
					   len1, len2);

		  break;
		}
	      case 'c':
		{
		  charvalue = va_arg(args, int);

		  if (character != NULL)
		    {
		      character(charvalue);

		      written++;
		    }

		  break;
		}
	      case '%':
		{
		  if (character != NULL)
		    {
		      character(c);

		      written++;
		    }

		  break;
		}
	      default:
		{
		  if (character != NULL)
		    {
		      character('%');
		      character(fmt[i]);

		      written += 2;
		    }
		}
	      }

	    break;
	  }
	default:
	  {
	    if (character != NULL)
	      {
		character(fmt[i]);

		written++;
	      }
	    break;
	  }
	}
    }

  va_end(args);

  return (written);
}
