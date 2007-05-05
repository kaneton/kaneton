/*
** chksum.c for epitar in /goinfre/yo/epitar/src
**
** Made by yoann amsellem
** Login   <amsell_y@epita.fr>
**
** Started on  Wed Jul  5 11:03:08 2006 yoann amsellem
** Last update Sat Jul  8 17:34:06 2006 yoann amsellem
*/

#include <unistd.h>
#include <stdlib.h>
#include "epitar.h"

void		bad_cheksum(void)
{
  write(STDERR_FILENO, "bad checksum\n", 13);
  exit(2);
}

int		calc_chksum(struct s_header *header)
{
  int		tot = 0;
  int		cpt = 0;
  char		*count = NULL;

  for (cpt = 0, count = (char *)header; cpt < HEADER_SZ; ++cpt, ++count)
  {
    if (cpt == CHKSUM_POSI)
    {
      count += CHKSUM_LEN;
      cpt += CHKSUM_LEN;
    }
    tot += *count;
  }
  tot += ' ' * 8;
  return tot;
}

void		check_chksum(struct s_header *header, int real)
{
  char		sum[CHKSUM_LEN + 1];
  int		cpt = 0;

  if (header == NULL)
    return;
  for (cpt = 0; cpt < CHKSUM_LEN; ++cpt)
    sum[cpt] = header->chksum[cpt];
  sum[cpt] = '\0';
  if (my_getnbr_base(sum, "01234567") != real)
    bad_cheksum();
}
