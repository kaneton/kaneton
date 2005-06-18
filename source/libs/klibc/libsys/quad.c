/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * quad.c
 * 
 * path          /home/mycure/kaneton/libs/klibc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:08:53 2005   mycure
 * last update   Fri Jun 17 16:15:22 2005   mycure
 */

#include <klibc/include/klibc.h>

/*
 * add two quads.  this is trivial since a one-bit carry from a single
 * u_long addition x+y occurs if and only if the sum x+y is less than
 * either x or y (the choice to compare with x or y is arbitrary).
 */

quad_t			__adddi3(quad_t				a,
				 quad_t				b)
{
  union uu		aa;
  union uu		bb;
  union uu		sum;

  aa.q = a;
  bb.q = b;

  sum.ul[QUAD_L] = aa.ul[QUAD_L] + bb.ul[QUAD_L];
  sum.ul[QUAD_H] = aa.ul[QUAD_H] + bb.ul[QUAD_H] +
    (sum.ul[QUAD_L] < bb.ul[QUAD_L]);

  return (sum.q);
}

/*
 * return a & b, in quad.
 */

quad_t			__anddi3(quad_t				a,
				 quad_t				b)
{
  union uu		aa;
  union uu		bb;

  aa.q = a;
  bb.q = b;

  aa.ul[0] &= bb.ul[0];
  aa.ul[1] &= bb.ul[1];

  return (aa.q);
}

/*
 * shift a (signed) quad value left (arithmetic shift left).
 * this is the same as logical shift left!
 */

quad_t			__ashldi3(quad_t			a,
				  qshift_t			shift)
{
  union uu		aa;

  if (shift == 0)
    return(a);

  aa.q = a;

  if (shift >= QUAD_LONG_BITS)
    {
      aa.ul[QUAD_H] = aa.ul[QUAD_L] << (shift - QUAD_LONG_BITS);
      aa.ul[QUAD_L] = 0;
    }
  else
    {
      aa.ul[QUAD_H] = (aa.ul[QUAD_H] << shift) |
	(aa.ul[QUAD_L] >> (QUAD_LONG_BITS - shift));
      aa.ul[QUAD_L] <<= shift;
    }

  return (aa.q);
}

/*
 * shift a (signed) quad value right (arithmetic shift right).
 */
quad_t			__ashrdi3(quad_t			a,
				  qshift_t			shift)
{
  union uu		aa;

  if (shift == 0)
    return(a);

  aa.q = a;

  if (shift >= QUAD_LONG_BITS)
    {
      long s;

      /*
       * smear bits rightward using the machine's right-shift
       * method, whether that is sign extension or zero fill,
       * to get the `sign word' s.  note that shifting by
       * LONG_BITS is undefined, so we shift (LONG_BITS-1),
       * then 1 more, to get our answer.
       */

      /* LINTED inherits machine dependency */

      s = (aa.sl[QUAD_H] >> (QUAD_LONG_BITS - 1)) >> 1;

      /* LINTED inherits machine dependency */

      aa.ul[QUAD_L] = aa.sl[QUAD_H] >> (shift - QUAD_LONG_BITS);
      aa.ul[QUAD_H] = s;
    }
  else
    {
      aa.ul[QUAD_L] = (aa.ul[QUAD_L] >> shift) |
	(aa.ul[QUAD_H] << (QUAD_LONG_BITS - shift));

      /* LINTED inherits machine dependency */

      aa.sl[QUAD_H] >>= shift;
    }

  return (aa.q);
}

/*
 * return 0, 1, or 2 as a <, =, > b respectively.
 * both a and b are considered signed---which means only the high word is
 * signed.
 */

int			__cmpdi2(quad_t				a,
				 quad_t				b)
{
  union uu		aa;
  union uu		bb;

  aa.q = a;
  bb.q = b;

  return (aa.sl[QUAD_H] < bb.sl[QUAD_H] ? 0 :
	  aa.sl[QUAD_H] > bb.sl[QUAD_H] ? 2 :
	  aa.ul[QUAD_L] < bb.ul[QUAD_L] ? 0 :
	  aa.ul[QUAD_L] > bb.ul[QUAD_L] ? 2 : 1);
}

/*
 * divide two signed quads.
 * ??? if -1/2 should produce -1 on this machine, this code is wrong
 */

quad_t			__divdi3(quad_t				a,
				 quad_t				b)
{
  u_quad_t		ua;
  u_quad_t		ub;
  u_quad_t		uq;
  int			neg = 0;

  ua = a;
  ub = b;

  if (a < 0)
    ua = -ua, neg ^= 1;

  if (b < 0)
    ub = -ub, neg ^= 1;

  uq = __qdivrem(ua, ub, (u_quad_t *)0);

  if (neg)
    uq = - uq;

  return (uq);
}

/*
 * return a | b, in quad.
 */

quad_t			__iordi3(quad_t				a,
				 quad_t				b)
{
  union uu		aa;
  union uu		bb;

  aa.q = a;
  bb.q = b;

  aa.ul[0] |= bb.ul[0];
  aa.ul[1] |= bb.ul[1];

  return (aa.q);
}

/*
 * shift an (unsigned) quad value left (logical shift left).
 * this is the same as arithmetic shift left!
 */

quad_t			__lshldi3(quad_t			a,
				  qshift_t			shift)
{
  union uu		aa;

  if (shift == 0)
    return(a);

  aa.q = a;

  if (shift >= QUAD_LONG_BITS)
    {
      aa.ul[QUAD_H] = aa.ul[QUAD_L] << (shift - QUAD_LONG_BITS);
      aa.ul[QUAD_L] = 0;
    }
  else
    {
      aa.ul[QUAD_H] = (aa.ul[QUAD_H] << shift) |
	(aa.ul[QUAD_L] >> (QUAD_LONG_BITS - shift));
      aa.ul[QUAD_L] <<= shift;
    }

  return (aa.q);
}

/*
 * shift an (unsigned) quad value right (logical shift right).
 */

quad_t			__lshrdi3(quad_t			a,
				  qshift_t			shift)
{
  union uu		aa;

  if (shift == 0)
    return(a);

  aa.q = a;

  if (shift >= QUAD_LONG_BITS)
    {
      aa.ul[QUAD_L] = aa.ul[QUAD_H] >> (shift - QUAD_LONG_BITS);
      aa.ul[QUAD_H] = 0;
    }
  else
    {
      aa.ul[QUAD_L] = (aa.ul[QUAD_L] >> shift) |
	(aa.ul[QUAD_H] << (QUAD_LONG_BITS - shift));
      aa.ul[QUAD_H] >>= shift;
    }

  return (aa.q);
}

/*
 * return remainder after dividing two signed quads.
 *
 * XXX we assume a % b < 0 iff a < 0, but this is actually machine-dependent.
 */

quad_t			__moddi3(quad_t				a,
				 quad_t				b)
{
  u_quad_t		ua;
  u_quad_t		ub;
  u_quad_t		ur;
  int			neg = 0;

  ua = a;
  ub = b;

  if (a < 0)
    ua = -ua, neg ^= 1;

  if (b < 0)
    ub = -ub;

  (void)__qdivrem(ua, ub, &ur);

  if (neg)
    ur = -ur;

  return (ur);
}

/*
 * multiply two quads.
 *
 * our algorithm is based on the following.  split incoming quad values
 * u and v (where u,v >= 0) into
 *
 * u = 2^n u1  *  u0(n = number of bits in `u_long', usu. 32)
 *
 * and 
 *
 * v = 2^n v1  *  v0
 *
 * then
 *
 * uv = 2^2n u1 v1  +  2^n u1 v0  +  2^n v1 u0  +  u0 v0
 *    = 2^2n u1 v1  +     2^n (u1 v0 + v1 u0)   +  u0 v0
 *
 * bow add 2^n u1 v1 to the first term and subtract it from the middle,
 * and add 2^n u0 v0 to the last term and subtract it from the middle.
 * this gives:
 *
 * uv = (2^2n + 2^n) (u1 v1)  +
 *          (2^n)    (u1 v0 - u1 v1 + u0 v1 - u0 v0)  +
 *        (2^n + 1)  (u0 v0)
 *
 * factoring the middle a bit gives us:
 *
 * uv = (2^2n + 2^n) (u1 v1)  +[u1v1 = high]
 *  (2^n)    (u1 - u0) (v0 - v1)  +[(u1-u0)... = mid]
 *        (2^n + 1)  (u0 v0)[u0v0 = low]
 *
 * the terms (u1 v1), (u1 - u0) (v0 - v1), and (u0 v0) can all be done
 * in just half the precision of the original.  (note that either or both
 * of (u1 - u0) or (v0 - v1) may be negative.)
 *
 * this algorithm is from Knuth vol. 2 (2nd ed), section 4.3.3, p. 278.
 *
 * since C does not give us a `long * long = quad' operator, we split
 * our input quads into two longs, then split the two longs into two
 * shorts.  We can then calculate `short * short = long' in native
 * arithmetic.
 *
 * our product should, strictly speaking, be a `long quad', with 128
 * bits, but we are going to discard the upper 64.  In other words,
 * we are not interested in uv, but rather in (uv mod 2^2n).  this
 * makes some of the terms above vanish, and we get:
 *
 * (2^n)(high) + (2^n)(mid) + (2^n + 1)(low)
 *
 * or
 *
 * (2^n)(high + mid + low) + low
 *
 * furthermore, `high' and `mid' can be computed mod 2^n, as any factor
 * of 2^n in either one will also vanish.  only `low' need be computed
 * mod 2^2n, and only because of the final term above.
 */

static quad_t		__lmulq(u_long				u,
				u_long				v);

quad_t			__muldi3(quad_t				a,
				 quad_t				b)
{
  union uu		u;
  union uu		v;
  union uu		low;
  union uu		prod;
  u_long		high;
  u_long		mid;
  u_long		udiff;
  u_long		vdiff;
  int			negall;
  int			negmid;

#define u1		u.ul[QUAD_H]
#define u0		u.ul[QUAD_L]
#define v1		v.ul[QUAD_H]
#define v0		v.ul[QUAD_L]

  /*
   * get u and v such that u, v >= 0.  when this is finished,
   * u1, u0, v1, and v0 will be directly accessible through the
   * longword fields.
   */

  if (a >= 0)
    u.q = a, negall = 0;
  else
    u.q = -a, negall = 1;

  if (b >= 0)
    v.q = b;
  else
    v.q = -b, negall ^= 1;

  if (u1 == 0 && v1 == 0)
    {
      /*
       * an (I hope) important optimization occurs when u1 and v1
       * are both 0. this should be common since most numbers
       * are small. here the product is just u0*v0.
       */

      prod.q = __lmulq(u0, v0);
    }
  else
    {
      /*
       * compute the three intermediate products, remembering
       * whether the middle term is negative. we can discard
       * any upper bits in high and mid, so we can use native
       * u_long * u_long => u_long arithmetic.
       */

      low.q = __lmulq(u0, v0);

      if (u1 >= u0)
	negmid = 0, udiff = u1 - u0;
      else
	negmid = 1, udiff = u0 - u1;

      if (v0 >= v1)
	vdiff = v0 - v1;
      else
	vdiff = v1 - v0, negmid ^= 1;

      mid = udiff * vdiff;

      high = u1 * v1;

      /*
       * assemble the final product.
       */

      prod.ul[QUAD_H] = high + (negmid ? -mid : mid) + low.ul[QUAD_L] +
	low.ul[QUAD_H];
      prod.ul[QUAD_L] = low.ul[QUAD_L];
    }

  return (negall ? -prod.q : prod.q);

#undef u1
#undef u0
#undef v1
#undef v0

}

/*
 * multiply two 2N-bit longs to produce a 4N-bit quad, where N is half
 * the number of bits in a long (whatever that is---the code below
 * does not care as long as quad.h does its part of the bargain---but
 * typically N==16).
 *
 * we use the same algorithm from Knuth, but this time the modulo refinement
 * does not apply. on the other hand, since N is half the size of a long,
 * we can get away with native multiplication---none of our input terms
 * exceeds (ULONG_MAX >> 1).
 *
 * note that, for u_long l, the quad-precision result
 *
 * l << N
 *
 * splits into high and low longs as HHALF(l) and LHUP(l) respectively.
 */

static quad_t		__lmulq(u_long				u,
				u_long				v)
{
  u_long		u1;
  u_long		u0;
  u_long		v1;
  u_long		v0;
  u_long		udiff;
  u_long		vdiff;
  u_long		high;
  u_long		mid;
  u_long		low;
  u_long		prodh;
  u_long		prodl;
  u_long		was;
  union uu		prod;
  int			neg;

  u1 = QUAD_HHALF(u);
  u0 = QUAD_LHALF(u);
  v1 = QUAD_HHALF(v);
  v0 = QUAD_LHALF(v);

  low = u0 * v0;

  /*
   * this is the same small-number optimization as before.
   */

  if (u1 == 0 && v1 == 0)
    return (low);

  if (u1 >= u0)
    udiff = u1 - u0, neg = 0;
  else
    udiff = u0 - u1, neg = 1;

  if (v0 >= v1)
    vdiff = v0 - v1;
  else
    vdiff = v1 - v0, neg ^= 1;

  mid = udiff * vdiff;

  high = u1 * v1;

  /*
   * prod = (high << 2N) + (high << N);
   */

  prodh = high + QUAD_HHALF(high);
  prodl = QUAD_LHUP(high);

  /*
   * if (neg) prod -= mid << N; else prod += mid << N;
   */

  if (neg)
    {
      was = prodl;
      prodl -= QUAD_LHUP(mid);
      prodh -= QUAD_HHALF(mid) + (prodl > was);
    }
  else
    {
      was = prodl;
      prodl += QUAD_LHUP(mid);
      prodh += QUAD_HHALF(mid) + (prodl < was);
    }

  /*
   * prod += low << N
   */

  was = prodl;
  prodl += QUAD_LHUP(low);
  prodh += QUAD_HHALF(low) + (prodl < was);

  /*
   * ... + low;
   */

  if ((prodl += low) < low)
    prodh++;

  /*
   * return 4N-bit product
   */

  prod.ul[QUAD_H] = prodh;
  prod.ul[QUAD_L] = prodl;

  return (prod.q);
}

/*
 * return -a (or, equivalently, 0 - a), in quad.  see subdi3.c.
 */

quad_t			__negdi2(quad_t				a)
{
  union uu		aa;
  union uu		res;

  aa.q = a;

  res.ul[QUAD_L] = -aa.ul[QUAD_L];
  res.ul[QUAD_H] = -aa.ul[QUAD_H] - (res.ul[QUAD_L] > 0);

  return (res.q);
}

/*
 * return ~a.  for some reason gcc calls this `one's complement' rather
 * than `not'.
 */

quad_t			__one_cmpldi2(quad_t			a)
{
  union uu		aa;

  aa.q = a;

  aa.ul[0] = ~aa.ul[0];
  aa.ul[1] = ~aa.ul[1];

  return (aa.q);
}

#define QUAD_B		((long)1 << QUAD_HALF_BITS)	/* digit base */

/*
 * combine two `digits' to make a single two-digit number.
 */

#define QUAD_COMBINE(a, b)						\
  (((u_long)(a) << QUAD_HALF_BITS) | (b))

/*
 * select a type for digits in base B: use unsigned short if they fit
 */

#if (ULONG_MAX == 0xffffffff) && (USHORT_MAX >= 0xffff)
typedef unsigned short		digit;
#else
typedef u_long			digit;
#endif

/*
 * __qdivrem(u, v, rem) returns u/v and, optionally, sets *rem to u%v.
 *
 * we do this in base 2-sup-HALF_BITS, so that all intermediate products
 * fit within u_long. as a consequence, the maximum length dividend and
 * divisor are 4 `digits' in this base (they are shorter if they have
 * leading zeros).
 */

static void		shl(digit				*p,
			    int					len,
			    int					sh);

u_quad_t		__qdivrem(u_quad_t			uq,
				  u_quad_t			vq,
				  u_quad_t*			arq)
{
  union uu		tmp;
  digit*		u;
  digit*		v;
  digit*		q;
  digit			v1;
  digit			v2;
  u_long		qhat;
  u_long		rhat;
  u_long		t;
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
	  u_long	rbj;		/* r*B+u[j] (not root boy jim) */
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
	  u_long	nn = QUAD_COMBINE(uj0, uj1);

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
	    u[i] = (digit)(((u_long)u[i] >> d) |
			   QUAD_LHALF((u_long)u[i - 1] <<
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
    p[i] = (digit)(QUAD_LHALF((u_long)p[i] << sh) |
		   ((u_long)p[i + 1] >> (QUAD_HALF_BITS - sh)));

  p[i] = (digit)(QUAD_LHALF((u_long)p[i] << sh));
}

/*
 * subtract two quad values. this is trivial since a one-bit carry
 * from a single u_long difference x-y occurs if and only if (x-y) > x.
 */

quad_t			__subdi3(quad_t				a,
				 quad_t				b)
{
  union uu		aa;
  union uu		bb;
  union uu		diff;

  aa.q = a;
  bb.q = b;

  diff.ul[QUAD_L] = aa.ul[QUAD_L] - bb.ul[QUAD_L];
  diff.ul[QUAD_H] = aa.ul[QUAD_H] - bb.ul[QUAD_H] -
    (diff.ul[QUAD_L] > aa.ul[QUAD_L]);

  return (diff.q);
}

/*
 * return 0, 1, or 2 as a <, =, > b respectively.
 * neither a nor b are considered signed.
 */

int			__ucmpdi2(quad_t			a,
				  quad_t			b)
{
  union uu		aa;
  union uu		bb;

  aa.uq = a;
  bb.uq = b;

  return (aa.ul[QUAD_H] < bb.ul[QUAD_H] ? 0 :
	  aa.ul[QUAD_H] > bb.ul[QUAD_H] ? 2 :
	  aa.ul[QUAD_L] < bb.ul[QUAD_L] ? 0 :
	  aa.ul[QUAD_L] > bb.ul[QUAD_L] ? 2 : 1);
}

/*
 * divide two unsigned quads.
 */

u_quad_t		__udivdi3(u_quad_t			a,
				  u_quad_t			b)
{
  return (__qdivrem(a, b, (u_quad_t *)0));
}

/*
 * return remainder after dividing two unsigned quads.
 */

u_quad_t		__umoddi3(u_quad_t			a,
				  u_quad_t			b)
{
  u_quad_t		r;

  (void)__qdivrem(a, b, &r);

  return (r);
}

/*
 * return a ^ b, in quad.
 */

quad_t			__xordi3(quad_t				a,
				 quad_t				b)
{
  union uu		aa;
  union uu		bb;

  aa.q = a;
  bb.q = b;

  aa.ul[0] ^= bb.ul[0];
  aa.ul[1] ^= bb.ul[1];

  return (aa.q);
}
