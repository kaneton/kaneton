/*
** bistro_misc.c for bistro in /u/a1/quinta_j/rendu/c/bistro
**
** Made by julien quintard
** Login   <quinta_j@epita.fr>
**
** Started on  Wed Sep 15 16:28:54 2004 julien quintard
** Last update Wed Sep 15 21:40:33 2004 julien quintard
*/

#include "bistro.h"

t_bistro_number		bistro_switch_sign(t_bistro_number number)
{
  return -number;
}

void			bistro_error(int err, char *str)
{
  write(STDERR_FILENO, str, bistro_strlen(str));
  exit(err);
}

int			bistro_inbase(struct s_bistro *bistro, char c,
				      unsigned int *index)
{
  unsigned int		i;

  for (i = 0; i < bistro->basesz; i++)
    if (bistro->base[i] == c)
      {
	*index = i;
	return 0;
      }
  return -1;
}

char			bistro_indbase(struct s_bistro *bistro,
				       unsigned int index)
{
  if (index > bistro->basesz)
    bistro_error(BISTRO_EOTH, "[error] bad number in base\n");
  return bistro->base[index];
}

unsigned int		bistro_pow(unsigned int v, unsigned int e)
{
  unsigned int		value;
  unsigned int		i;

  for (value = 1, i = 0; i < e; i++)
    value *= v;
  return value;
}
