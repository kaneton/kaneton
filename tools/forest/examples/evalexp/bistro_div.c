/*
** bistro_div.c for bistro in /u/a1/quinta_j/rendu/c/bistro
**
** Made by julien quintard
** Login   <quinta_j@epita.fr>
**
** Started on  Wed Sep 15 16:28:44 2004 julien quintard
** Last update Wed Sep 15 21:37:36 2004 julien quintard
*/

#include "bistro.h"

t_bistro_number		bistro_div(t_bistro_number left,
				   t_bistro_number right)
{
  if (right == 0)
    bistro_error(BISTRO_EARITH, "[error] division by zero\n");
  return left / right;
}
