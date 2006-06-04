/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/libs/klibc/libdata/malloc/04/04.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:12:15 2005]
 * updated       matthieu bucchianeri   [sun jun  4 19:55:04 2006]
 */

#include <klibc.h>
#include "../common/common.h"

/*
 * reuse and fragmentation
 */

void		check_malloc_04(void)
{
  void		*p1;
  void		*p2;
  void		*p3;
  void		*p4;
  void		*p5;
  void		*p6;
  void		*pn;

  TEST_ENTER(1);

  p1 = malloc(10);

  free(p1);

  p2 = malloc(10);

  free(p2);

  if (p1 != p2)
    printf("not reused\n");

  p1 = malloc(100);

  free(p1);

  pn = p1 + 100;

  p2 = malloc(20);
  p3 = malloc(50);
  p4 = malloc(70);
  p5 = malloc(90);
  p6 = malloc(100);

  free(p2);
  free(p3);
  free(p4);
  free(p5);
  free(p6);

  if (p2 < p1 && p2 > pn &&
      p3 < p1 && p3 > pn &&
      p4 < p1 && p4 > pn &&
      p5 < p1 && p5 > pn &&
      p6 < p1 && p6 > pn)
    printf("no reuse\n");

  p1 = malloc(100);
  p2 = realloc(p1, 50);

  if (p1 != p2)
    printf("realloc do not reuse memory\n");

  p3 = malloc(10);

  if (p3 < p2 || p3 > p2 + 100)
    printf("realloc do not split\n");

  free(p2);
  free(p3);

  TEST_LEAVE;
}
