/*
** bistro_std.c for bistro in /u/a1/quinta_j/rendu/c/bistro
**
** Made by julien quintard
** Login   <quinta_j@epita.fr>
**
** Started on  Wed Sep 15 16:29:25 2004 julien quintard
** Last update Wed Sep 15 20:25:08 2004 julien quintard
*/

#include "bistro.h"

void			bistro_dump_number(struct s_bistro *bistro,
					   t_bistro_number number)
{
  t_bistro_number	intsz;
  int			size;
  int			pw;
  char			c;
  int			i;

  if (number == 0)
    {
      write(STDOUT_FILENO, &bistro->base[0], 1);
      return;
    }
  if (number < 0)
    {
      write(STDOUT_FILENO, &bistro->ops.sub, 1);
      number = -number;
    }
  for (size = 0, intsz = number; intsz > 0; size++)
    intsz /= bistro->basesz;
  for (intsz = number, i = size - 1; i >= 0; i--)
    {
      pw = bistro_pow(bistro->basesz, i);
      c = bistro_indbase(bistro, intsz / pw);
      intsz %= pw;
      write(STDOUT_FILENO, &c, 1);
    }
}

int			bistro_strlen(char *str)
{
  int			i;

  for (i = 0; str[i]; i++)
    ;
  return i;
}

int			bistro_strcmp(char *s1, char *s2)
{
  int			i;

  if (bistro_strlen(s1) != bistro_strlen(s2))
    return -1;
  for (i = 0; s1[i]; i++)
    if (s1[i] != s2[i])
      return s1[i] - s2[i];
  return 0;
}
