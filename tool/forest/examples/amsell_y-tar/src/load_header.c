/*
** load_header.c for epitar in /goinfre/yo/epitar
**
** Made by yoann amsellem
** Login   <amsell_y@epita.fr>
**
** Started on  Sat Jul  8 11:08:54 2006 yoann amsellem
** Last update Sat Jul  8 17:35:42 2006 yoann amsellem
*/

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "epitar.h"

char		*tooct(int value)
{
  int		cpt = 8;
  char		*res = NULL;
  union u_conv	conv;
  char		test = ' ';

  res = malloc(8 * sizeof (char));
  do
  {
    conv.integer = 48 + (value % 8);
    test = conv.cara;
    res[--cpt] = test;
    value /= 8;
  } while (cpt > 0 && value > 0);
  while (cpt > 0)
    res[--cpt] = '0';
  return res;
}

int	fill_headers(struct s_chain_header *headers)
{
  int	sum = 0;
  char	mag[6] = "USTAR\0";
  char	*chksum = NULL;
  int	cpt = 0;

  if (headers == NULL)
    return 0;
  while (headers)
  {
    if (headers->header == NULL)
    {
      headers = headers->next;
      continue;
    }
    for (cpt = 0; cpt < 6; ++cpt)
      headers->header->magic[cpt] = mag[cpt];
    sum = calc_chksum(headers->header);
    chksum = tooct(sum);
    for (cpt = 0; cpt < 8; ++cpt)
      headers->header->chksum[cpt] = chksum[cpt];
    free(chksum);
    headers = headers->next;
  }
  return 1;
}
