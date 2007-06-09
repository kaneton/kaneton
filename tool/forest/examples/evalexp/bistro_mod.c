/*
** bistro_mod.c for bistro in /u/a1/quinta_j/rendu/c/bistro
**
** Made by julien quintard
** Login   <quinta_j@epita.fr>
**
** Started on  Wed Sep 15 16:29:05 2004 julien quintard
** Last update Wed Sep 15 21:38:02 2004 julien quintard
*/

#include "bistro.h"

t_bistro_number		bistro_mod(t_bistro_number left,
				   t_bistro_number right)
{
  if (right == 0)
    bistro_error(BISTRO_EARITH, "[error] modulo by zero\n");
  return left % right;
}
