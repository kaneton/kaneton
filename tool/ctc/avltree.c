/* NOTE: This is a modified version of avltree, originally written by
 * Gregory Tseytin. The code was modified to have an integer key.
 * There does not seem to be a web site for the project: I obtained
 * the code from the FreeBSD ports area:
 * 	http://www.gtlib.gatech.edu/pub/FreeBSD/distfiles/
 *
 * Warren
 */

/*
**  avltree - AVL index routines by Gregory Tseytin.
**
**
**    Copyright (c) 2000 Gregory Tseytin <tseyting@acm.org>
**      All rights reserved.
**  
**    Redistribution and use in source and binary forms, with or without
**    modification, are permitted provided that the following conditions
**    are met:
**    1. Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer as
**       the first lines of this file unmodified.
**    2. Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**  
**    THIS SOFTWARE IS PROVIDED BY Gregory Tseytin ``AS IS'' AND ANY EXPRESS OR
**    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
**    OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
**    IN NO EVENT SHALL Gregory Tseytin BE LIABLE FOR ANY DIRECT, INDIRECT,
**    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
**    NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
**    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
**    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
**    THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**            
**
*/

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "avltree.h"


/*
 * 'inner' avl stuff
 */
/* way3.h */

typedef char way3;		/* -1, 0, 1 */

#define way3stop  ((way3)0)
#define way3left ((way3)-1)
#define way3right ((way3)1)

#define way3sum(x,y) ((x)+(y))
/* assume x!=y */

#define way3opp(x) (-(x))


/* node.h */

typedef struct _node {
  struct _node *ptr[2];		/* left, right */
  way3 balance, trace;
  rectype data;
}   node;

#define stepway(n,x) (((n)->ptr)[way3ix(x)])
#define stepopp(n,x) (((n)->ptr)[way3ix(way3opp(x))])


/* tree.h */

#define SRF_FINDEQUAL 1
#define SRF_FINDLESS  2
#define SRF_FINDGREAT 4
#define SRF_SETMARK   8
#define SRF_FROMMARK 16

#define avltree_init(x) (*(x)=NULL)



static int node_overhead = sizeof(node) - sizeof(uint32_t);

/******************************************************************************
								WAY3
 ******************************************************************************/
static way3
    makeway3(int n)
{
  return n > 0 ? way3right : n < 0 ? way3left : way3stop;
}

static way3
    way3opp2(way3 x, way3 y)
{
  return x == y ? way3opp(x) : way3stop;
}



/*****************************************************************************/

static void freenode(node * n)
{
  free(n);
}

static int compkey(rectype *r1, rectype *r2)
{
        if (r1->key < r2->key)
                return(-1);
        if (r1->key > r2->key)
                return(1);
        if (r1->recptr < r2->recptr)
                return(-1);
        if (r1->recptr > r2->recptr)
                return(1);
        return(0);
}

static void copydata(rectype * r1, rectype * r2)
{
  r1->recptr = r2->recptr;
  r1->key = r2->key;
}


static node *
    allocnode()
{
  int size = sizeof(uint32_t);
  node *n = (node *) malloc(size + node_overhead);
  if (n == NULL) {
    fprintf(stderr, "avltrees: out of memory\n");
    exit(1);
  }
  return n;
}



/******************************************************************************
								NODE
 ******************************************************************************/
static node *
    swapptr(node ** ptrptr, node * new)
{
  node *old = *ptrptr;
  *ptrptr = new;
  return old;
}

static int way3ix(way3 x)
{				/* assume x!=0 */
  return x == way3right ? 1 : 0;
}

/******************************************************************************
								TREE
 ******************************************************************************/

typedef int bool;

static node **t;
static node *r, *s;
static way3 wayhand;

static bool
    restruct(bool op_del)
{
  way3 n = r->balance, c;
  node *p;
  bool g = n == way3stop ? op_del : n == wayhand;
  if (g)
    p = r;
  else {
    p = stepopp(r, wayhand);
    stepopp(r, wayhand) = swapptr(&stepway(p, wayhand), r);
    c = p->balance;
    s->balance = way3opp2(c, wayhand);
    r->balance = way3opp2(c, way3opp(wayhand));
    p->balance = way3stop;
  }
  stepway(s, wayhand) = swapptr(&stepopp(p, wayhand), s);
  *t = p;
#ifdef TESTING
  if (op_del) {
    if (g)
      rstd1++;
    else
      rstd2++;
  } else {
    if (g)
      rsti1++;
    else
      rsti2++;
  }
#endif
  return g;
}

static rectype *
    avltree_search(node ** tt, rectype * key, unsigned short searchflags)
{
  node *p, *q, *pp;
  way3 aa, waydir, wayopp;

  if (!(~searchflags & (SRF_FINDGREAT | SRF_FINDLESS)))
    return NULL;
  if (!(searchflags & (SRF_FINDGREAT | SRF_FINDEQUAL | SRF_FINDLESS)))
    return NULL;
  waydir = searchflags & SRF_FINDGREAT ? way3right :
    searchflags & SRF_FINDLESS ? way3left : way3stop;
  wayopp = way3opp(waydir);
  p = q = NULL;
  while ((pp = *tt) != NULL) {
    aa = searchflags & SRF_FROMMARK ? pp->trace :
      makeway3(compkey(key, &(pp->data)));
    if (searchflags & SRF_SETMARK)
      pp->trace = aa;
    if (aa == way3stop) {
      if (searchflags & SRF_FINDEQUAL)
	return &(pp->data);
      if ((q = stepway(pp, waydir)) == NULL)
	break;
      if (searchflags & SRF_SETMARK)
	pp->trace = waydir;
      while (1) {
	if ((pp = stepway(q, wayopp)) == NULL) {
	  if (searchflags & SRF_SETMARK)
	    q->trace = way3stop;
	  return &(q->data);
	}
	if (searchflags & SRF_SETMARK)
	  q->trace = wayopp;
	q = pp;
      }
    }
    /* remember the point where we can change direction to waydir */
    if (aa == wayopp)
      p = pp;
    tt = &stepway(pp, aa);
  }
  if (p == NULL || !(searchflags & (SRF_FINDLESS | SRF_FINDGREAT)))
    return NULL;
  if (searchflags & SRF_SETMARK)
    p->trace = way3stop;
  return &(p->data);
}

static void avltree_first(node ** tt)
{
  node *pp;
  while ((pp = *tt) != NULL) {
    pp->trace = way3left;
    tt = &stepway(pp, way3left);
  }
}

static void avltree_last(node ** tt)
{
  node *pp;
  while ((pp = *tt) != NULL) {
    pp->trace = way3right;
    tt = &stepway(pp, way3right);
  }
}

static rectype *
    avltree_insert(node ** tt, rectype * key)
{
  way3 aa, b;
  node *p, *q, *pp;

  t = tt;
  p = *tt;
  while ((pp = *tt) != NULL) {
    aa = makeway3(compkey(key, &(pp->data)));
    if (aa == way3stop) {
      return NULL;
    }
    if (pp->balance != way3stop)
      t = tt;			/* t-> the last disbalanced node */
    pp->trace = aa;
    tt = &stepway(pp, aa);
  }
  *tt = q = allocnode();
  q->balance = q->trace = way3stop;
  stepway(q, way3left) = stepway(q, way3right) = NULL;
  copydata(&(q->data), key);
  /* balancing */
  s = *t;
  wayhand = s->trace;
  if (wayhand != way3stop) {
    r = stepway(s, wayhand);
    for (p = r; p != NULL; p = stepway(p, b))
      b = p->balance = p->trace;
    b = s->balance;
    if (b != wayhand)
      s->balance = way3sum(wayhand, b);
    else if (restruct(0))
      s->balance = r->balance = way3stop;
  }
  return &(q->data);
}

static rectype *
    avltree_delete(node ** tt, rectype * key, unsigned short searchflags)
{
  way3 aa, aaa, b, bb;
  node *p, *q, *pp, *p1;
  node **t1, **tt1, **qq1, **rr = tt;

  t = t1 = tt1 = qq1 = tt;
  p = *tt;
  q = NULL;
  aaa = way3stop;

  while ((pp = *tt) != NULL) {
    aa = aaa != way3stop ? aaa :
      searchflags & SRF_FROMMARK ? pp->trace :
      makeway3(compkey(key, &(pp->data)));
    b = pp->balance;
    if (aa == way3stop) {
      qq1 = tt;
      q = pp;
      rr = t1;
      aa = b != way3stop ? b : way3left;
      aaa = way3opp(aa);	/* will move opposite to aa */
    }
    t = t1;
    if (b == way3stop || (b != aa && stepopp(pp, aa)->balance == way3stop))
      t1 = tt;
    tt1 = tt;
    tt = &stepway(pp, aa);
    pp->trace = aa;
  }
  if (aaa == way3stop)
    return NULL;
  copydata(key, &(q->data));
  p = *tt1;
  *tt1 = p1 = stepopp(p, p->trace);
  if (p != q) {
    *qq1 = p;
    memcpy(p->ptr, q->ptr, sizeof(p->ptr));
    p->balance = q->balance;
    wayhand = p->trace = q->trace;
    if (t == &stepway(q, wayhand))
      t = &stepway(p, wayhand);
  }
  while ((s = *t) != p1) {
    wayhand = way3opp(s->trace);
    b = s->balance;
    if (b != wayhand) {
      s->balance = way3sum(wayhand, b);
    } else {
      r = stepway(s, wayhand);
      if (restruct(1)) {
	if ((bb = r->balance) != way3stop)
	  s->balance = way3stop;
	r->balance = way3sum(way3opp(wayhand), bb);
      }
    }
    t = &stepopp(s, wayhand);
  }
  while ((p = *rr) != NULL) {
    /* adjusting trace */
    aa = makeway3(compkey(&(q->data), &(p->data)));
    p->trace = aa;
    rr = &stepway(p, aa);
  }
  freenode(q);
  return key;
}

static void avltree_clear(node ** tt)
{
  node *p = *tt, *q = NULL, *x, **xx;

  if (p != NULL) {
    while (1) {
      if ((x = stepway(p, way3left)) != NULL ||
	  (x = stepway(p, way3right)) != NULL) {
	stepway(p, way3left) = q;
	q = p;
	p = x;
	continue;
      }
      freenode(p);
      if (q == NULL)
	break;
      if (*(xx = &stepway(q, way3right)) == p)
	*xx = NULL;
      p = q;
      q = *(xx = &stepway(p, way3left));
      *xx = NULL;
    }
    *tt = NULL;
  }
}


/******************************************************************************
						'PLUS' interface style
 ******************************************************************************/

void avl_create_index(AVL_IX_DESC * pix)
{
  pix->root = NULL;

}

void avl_destroy_index(AVL_IX_DESC * pix)
{
  avltree_clear((node **) & (pix->root));
  pix->root = NULL;
}

int avl_find_key(AVL_IX_REC * pe, AVL_IX_DESC * pix)
{
  rectype *ptr;

  pe->recptr = NULL;
  ptr = avltree_search((node **) & (pix->root), pe,
		       SRF_FINDEQUAL | SRF_SETMARK | SRF_FINDGREAT);
  if (ptr == NULL)
    return AVL_IX_FAIL;

  pe->recptr = ptr->recptr;
  if (compkey(pe, ptr))
    return AVL_IX_FAIL;
  return AVL_IX_OK;
}

int avl_locate_key(AVL_IX_REC * pe, AVL_IX_DESC * pix)
{
  rectype *ptr;
  int ret;

  pe->recptr = NULL;
  ptr = avltree_search((node **) & (pix->root), pe,
		       SRF_FINDEQUAL | SRF_SETMARK | SRF_FINDGREAT);
  if (ptr == NULL)
    return AVL_EOIX;
  ret = compkey(pe, ptr) ? AVL_IX_FAIL : AVL_IX_OK;
  copydata(pe, ptr);
  return ret;
}

int avl_add_key(AVL_IX_REC * pe, AVL_IX_DESC * pix)
{
  if (avltree_insert((node **) & (pix->root), pe) == NULL)
    return AVL_IX_FAIL;
  return AVL_IX_OK;
}

int avl_delete_key(AVL_IX_REC * pe, AVL_IX_DESC * pix)
{
  rectype *ptr;

  ptr = avltree_search((node **) & (pix->root), pe, SRF_FINDEQUAL | SRF_SETMARK);
  if (ptr == NULL)
    return AVL_IX_FAIL;
  avltree_delete((node **) & (pix->root), pe, SRF_FROMMARK);
  return AVL_IX_OK;
}

void avl_first_key(AVL_IX_DESC * pix)
{
  avltree_first((node **) & (pix->root));
}

void avl_last_key(AVL_IX_DESC * pix)
{
  avltree_last((node **) & (pix->root));
}

int avl_next_key(AVL_IX_REC * pe, AVL_IX_DESC * pix)
{
  rectype *ptr;
  if ((ptr = avltree_search((node **) & (pix->root), pe,	/* pe not used */
		       SRF_FROMMARK | SRF_SETMARK | SRF_FINDGREAT)) == NULL)
    return AVL_EOIX;
  copydata(pe, ptr);
  return AVL_IX_OK;
}

int avl_prev_key(AVL_IX_REC * pe, AVL_IX_DESC * pix)
{
  rectype *ptr;
  if ((ptr = avltree_search((node **) & (pix->root), pe,	/* pe not used */
			SRF_FROMMARK | SRF_SETMARK | SRF_FINDLESS)) == NULL)
    return AVL_EOIX;
  copydata(pe, ptr);
  return AVL_IX_OK;
}

int avl_find_exact(AVL_IX_REC * pe, AVL_IX_DESC * pix)
{
  rectype *ptr;
  ptr = avltree_search((node **) & (pix->root), pe,
		       SRF_FINDEQUAL | SRF_FINDGREAT | SRF_SETMARK);
  if (ptr == NULL)
    return AVL_IX_FAIL;
  if (pe->recptr != ptr->recptr)
    return AVL_IX_FAIL;
  return AVL_IX_OK;
}
