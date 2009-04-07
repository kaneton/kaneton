/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...ootloader/ibm-pc.ia32/educational/libc.c
 *
 * created       matthieu bucchianeri   [thu jun 28 16:14:31 2007]
 * updated       matthieu bucchianeri   [thu jun 28 17:41:15 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "libc.h"

/*
 * ---------- macros ----------------------------------------------------------
 */

#define QUAD_B		((long)1 << QUAD_HALF_BITS)	/* digit base */

#define QUAD_COMBINE(a, b)						\
  (((t_uint32)(a) << QUAD_HALF_BITS) | (b))

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

t_uint64		__umoddi3(t_uint64			a,
				  t_uint64			b)
{
  t_uint64		r;

  (void)__qdivrem(a, b, &r);

  return (r);
}

t_uint64		__udivdi3(t_uint64			a,
				  t_uint64			b)
{
  return (__qdivrem(a, b, (t_uint64 *)0));
}

typedef t_uint32	digit;

static void		shl(digit				*p,
			    int					len,
			    int					sh);

t_uint64		__qdivrem(t_uint64			uq,
				  t_uint64			vq,
				  t_uint64*			arq)
{
  union uu		tmp;
  digit*		u;
  digit*		v;
  digit*		q;
  digit			v1;
  digit			v2;
  t_uint32		qhat;
  t_uint32		rhat;
  t_uint32		t;
  int			m;
  int			n;
  int			d;
  int			j;
  int			i;
  digit			uspace[5];
  digit			vspace[5];
  digit			qspace[5];

  /*
   * take care of special cases: divide by zero, and u < v.
   */

  if (vq == 0)
    {
      /*
       * divide by zero.
       */

      static volatile const unsigned int zero = 0;

      tmp.ul[QUAD_H] = tmp.ul[QUAD_L] = 1 / zero;

      if (arq)
	*arq = uq;

      return (tmp.q);
    }

  if (uq < vq)
    {
      if (arq)
	*arq = uq;

      return (0);
    }

  u = &uspace[0];
  v = &vspace[0];
  q = &qspace[0];

  /*
   * break dividend and divisor into digits in base B, then
   * count leading zeros to determine m and n. when done, we
   * will have:
   *
   * u = (u[1]u[2]...u[m+n]) sub B
   * v = (v[1]v[2]...v[n]) sub B
   * v[1] != 0
   *
   * 1 < n <= 4 (if n = 1, we use a different division algorithm)
   * m >= 0 (otherwise u < v, which we already checked)
   * m + n = 4
   * and thus
   * m = 4 - n <= 2
   */

  tmp.uq = uq;

  u[0] = 0;
  u[1] = (digit)QUAD_HHALF(tmp.ul[QUAD_H]);
  u[2] = (digit)QUAD_LHALF(tmp.ul[QUAD_H]);
  u[3] = (digit)QUAD_HHALF(tmp.ul[QUAD_L]);
  u[4] = (digit)QUAD_LHALF(tmp.ul[QUAD_L]);

  tmp.uq = vq;

  v[1] = (digit)QUAD_HHALF(tmp.ul[QUAD_H]);
  v[2] = (digit)QUAD_LHALF(tmp.ul[QUAD_H]);
  v[3] = (digit)QUAD_HHALF(tmp.ul[QUAD_L]);
  v[4] = (digit)QUAD_LHALF(tmp.ul[QUAD_L]);

  for (n = 4; v[1] == 0; v++)
    {
      if (--n == 1)
	{
	  t_uint32	rbj;		/* r*B+u[j] (not root boy jim) */
	  digit		q1;
	  digit		q2;
	  digit		q3;
	  digit		q4;

	  /*
	   * change of plan, per exercise 16.
	   * r = 0;
	   * for j = 1..4:
	   * q[j] = floor((r*B + u[j]) / v),
	   * r = (r*B + u[j]) % v;
	   * we unroll this completely here.
	   */

	  t = v[2];			/* nonzero, by definition */
	  q1 = (digit)(u[1] / t);
	  rbj = QUAD_COMBINE(u[1] % t, u[2]);
	  q2 = (digit)(rbj / t);
	  rbj = QUAD_COMBINE(rbj % t, u[3]);
	  q3 = (digit)(rbj / t);
	  rbj = QUAD_COMBINE(rbj % t, u[4]);
	  q4 = (digit)(rbj / t);

	  if (arq)
	    *arq = rbj % t;

	  tmp.ul[QUAD_H] = QUAD_COMBINE(q1, q2);
	  tmp.ul[QUAD_L] = QUAD_COMBINE(q3, q4);

	  return (tmp.q);
	}
    }

  /*
   * by adjusting q once we determine m, we can guarantee that
   * there is a complete four-digit quotient at &qspace[1] when
   * we finally stop.
   */

  for (m = 4 - n; u[1] == 0; u++)
    m--;

  for (i = 4 - m; --i >= 0;)
    q[i] = 0;

  q += 4 - m;

  /*
   * here we run Program D, translated from MIX to C and acquiring
   * a few minor changes.
   *
   * D1: choose multiplier 1 << d to ensure v[1] >= B/2.
   */

  d = 0;

  for (t = v[1]; t < QUAD_B / 2; t <<= 1)
    d++;

  if (d > 0)
    {
      shl(&u[0], m + n, d);		/* u <<= d */
      shl(&v[1], n - 1, d);		/* v <<= d */
    }

  /*
   * D2: j = 0.
   */

  j = 0;

  v1 = v[1];			/* for D3 -- note that v[1..n] are constant */
  v2 = v[2];			/* for D3 */

  do
    {
      digit		uj0;
      digit		uj1;
      digit		uj2;

      /*
       * D3: calculate qhat (\^q, in TeX notation).
       * let qhat = min((u[j]*B + u[j+1])/v[1], B-1), and
       * let rhat = (u[j]*B + u[j+1]) mod v[1].
       * while rhat < B and v[2]*qhat > rhat*B+u[j+2],
       * decrement qhat and increase rhat correspondingly.
       * note that if rhat >= B, v[2]*qhat < rhat*B.
       */

      uj0 = u[j + 0];		/* for D3 only -- note that u[j+...] change */
      uj1 = u[j + 1];		/* for D3 only */
      uj2 = u[j + 2];		/* for D3 only */

      if (uj0 == v1)
	{
	  qhat = QUAD_B;
	  rhat = uj1;

	  goto qhat_too_big;
	}
      else
	{
	  t_uint32	nn = QUAD_COMBINE(uj0, uj1);

	  qhat = nn / v1;
	  rhat = nn % v1;
	}

      while (v2 * qhat > QUAD_COMBINE(rhat, uj2))
	{
	qhat_too_big:

	  qhat--;
	  if ((rhat += v1) >= QUAD_B)
	    break;
	}

      /*
       * D4: multiply and subtract.
       * the variable `t' holds any borrows across the loop.
       * we split this up so that we do not require v[0] = 0,
       * and to eliminate a final special case.
       */

      for (t = 0, i = n; i > 0; i--)
	{
	  t = u[i + j] - v[i] * qhat - t;
	  u[i + j] = (digit)QUAD_LHALF(t);
	  t = (QUAD_B - QUAD_HHALF(t)) & (QUAD_B - 1);
	}

      t = u[j] - t;
      u[j] = (digit)QUAD_LHALF(t);

      /*
       * D5: test remainder.
       * there is a borrow if and only if HHALF(t) is nonzero;
       * in that (rare) case, qhat was too large (by exactly 1).
       * fix it by adding v[1..n] to u[j..j+n].
       */

      if (QUAD_HHALF(t))
	{
	  qhat--;

	  for (t = 0, i = n; i > 0; i--)	/* D6: add back. */
	    {
	      t += u[i + j] + v[i];
	      u[i + j] = (digit)QUAD_LHALF(t);
	      t = QUAD_HHALF(t);
	    }

	  u[j] = (digit)QUAD_LHALF(u[j] + t);
	}

      q[j] = (digit)qhat;

    } while (++j <= m);				/* D7: loop on j. */

  /*
   * if caller wants the remainder, we have to calculate it as
   * u[m..m+n] >> d (this is at most n digits and thus fits in
   * u[m+1..m+n], but we may need more source digits).
   */

  if (arq)
    {
      if (d)
	{
	  for (i = m + n; i > m; --i)
	    u[i] = (digit)(((t_uint32)u[i] >> d) |
			   QUAD_LHALF((t_uint32)u[i - 1] <<
				      (QUAD_HALF_BITS - d)));
	  u[i] = 0;
	}

      tmp.ul[QUAD_H] = QUAD_COMBINE(uspace[1], uspace[2]);
      tmp.ul[QUAD_L] = QUAD_COMBINE(uspace[3], uspace[4]);
      *arq = tmp.q;
    }

  tmp.ul[QUAD_H] = QUAD_COMBINE(qspace[1], qspace[2]);
  tmp.ul[QUAD_L] = QUAD_COMBINE(qspace[3], qspace[4]);

  return (tmp.q);
}

/*
 * shift p[0]..p[len] left `sh' bits, ignoring any bits that
 * `fall out' the left (there never will be any such anyway).
 * we may assume len >= 0. NOTE THAT THIS WRITES len+1 DIGITS.
 */

static void		shl(digit				*p,
			    int					len,
			    int					sh)
{
  int			i;

  for (i = 0; i < len; i++)
    p[i] = (digit)(QUAD_LHALF((t_uint32)p[i] << sh) |
		   ((t_uint32)p[i + 1] >> (QUAD_HALF_BITS - sh)));

  p[i] = (digit)(QUAD_LHALF((t_uint32)p[i] << sh));
}
