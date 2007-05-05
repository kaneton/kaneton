/*
** print.c for epitar in /goinfre/yo/epitar/src
**
** Made by yoann amsellem
** Login   <amsell_y@epita.fr>
**
** Started on  Thu Jul  6 09:53:26 2006 yoann amsellem
** Last update Sat Jul  8 17:36:28 2006 yoann amsellem
*/

#include <stdio.h>
#include "epitar.h"

void	print_filename(struct s_chain_header *header)
{
  if (header == NULL || header->path == NULL)
    return;
  printf("%s\n", header->path);
}
