/*
** error.c for epitar in /goinfre/akadir-a/akadir_a-epitar/src
** 
** Made by abass akadiri
** Login   <akadir_a@epita.fr>
** 
** Started on  Wed Jul  5 18:20:53 2006 abass akadiri
** Last update Sat Jul  8 17:48:18 2006 abass akadiri
*/
#include <stdio.h>
#include "my_tar.h"

void invalid_option(char c)
{
  fprintf(stderr, "epitar: invalid option -- %c\n", c);
  fprintf(stderr, "Try `./epitar --help' for more information.\n");
  return;
}

int only_tar(int i)
{
  if (i < 2)
    {
      printf("epitar: You must specify one of the `-cvx' options\n");
      printf("Try `./epitar --help' for more information.\n");
    }
  return 1;
}

char		is_inside_c(char *str)
{
  int		i = 0;

  for (i = 0; str[i]; i++)
    {
      if (str[i] == 'c')
	return str[i];
    }
  return 1;
}
