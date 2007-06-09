/*
** creatpath.c for epitar in /u/a1/akadir_a/akadir_a-epitar/src
**
** Made by abass akadiri
** Login   <akadir_a@epita.fr>
**
** Started on  Thu Jul  6 00:57:51 2006 abass akadiri
** Last update Sat Jul  8 17:09:03 2006 abass akadiri
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int		my_strlen(char *str)
{
  int		i = 0;

  if (str == NULL)
    return 0;
  else
    for (i = 0; str[i]; i++)
      ;
  return i;
}
 char		*creatpath(char *path, char *str)
{
  int		a = 0;
  int		b = 0;
  int		i = 0;
  int		j = 0;
  char		*ret = NULL;

  a = my_strlen (path);
  b = my_strlen (str);
  if (b == 0)
    {
      ret = malloc(sizeof(char) * (b + 2));
      strcpy(ret, path);
      if (ret[a - 1] == '/')
	ret[a] = '\0';
      else
	ret[a] = '/';
    }
  else
    {
      ret = malloc(sizeof(char) * a + b + 2);
      for (i = 0;  path[i]  ; i++)
	ret[i] = path[i];
      if (path[i - 1] != '/')
	ret[i++] = '/';
      for (j = 0; str[j] ; i++, j++)
	ret[i] = str[j];
      ret[i] = '\0';
    }
  return ret;
}
