/*
** my_getnbr_base.c for epitar in /goinfre/yo/epitar/src
**
** Made by yoann amsellem
** Login   <amsell_y@epita.fr>
**
** Started on  Thu Jul  6 14:26:49 2006 yoann amsellem
** Last update Sat Jul  8 17:36:20 2006 yoann amsellem
*/

#include "epitar.h"

unsigned	get_place_in_base(const char *base, char cara)
{
  unsigned	cpt = 0;

  for (cpt = 0; base[cpt] && base[cpt] != cara; ++cpt)
    ;
  return cpt;
}

unsigned	strlen(const char *str)
{
  unsigned	cpt = 0;

  for (; str[cpt]; ++cpt)
    ;
  return cpt;
}

unsigned	power(int nb, unsigned pow)
{
  int		res = 1;

  if (nb == 0)
    return 0;
  if (pow == 0)
    return 1;
  for (; pow > 0; --pow)
    res = res * nb;
  return res;
}

int	is_plus_or_min(char cara)
{
  if (cara == '+' || cara == '-')
    return 1;
  return 0;
}

int		my_getnbr_base(const char *str, const char *base)
{
  unsigned	cpt = 0;
  unsigned	i = 0;
  unsigned	res = 0;
  unsigned	base_size = 0;
  unsigned	str_len = 0;
  unsigned	deb = 0;
  int		neg_factor = 1;

  for (i = 0; is_plus_or_min(str[i]); ++i)
  {
    if (str[i] == '-')
      neg_factor *= (-1);
  }
  deb = i;
  base_size = strlen(base);
  str_len = strlen(str);
  for (cpt = 0, i = str_len; i > deb; ++cpt, --i)
  {
    res += (get_place_in_base(base, str[i - 1])) * (power(base_size, cpt));
  }
  return (neg_factor * res);
}
