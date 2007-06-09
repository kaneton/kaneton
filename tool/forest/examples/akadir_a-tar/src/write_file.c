/*
** write_file.c for epitar in /goinfre/akadir_a/akadir_a-epitar/src/test
**
** Made by abass akadiri
** Login   <akadir_a@epita.fr>
**
** Started on  Tue Jul  4 17:17:49 2006 abass akadiri
** Last update Sat Jul  8 17:07:29 2006 abass akadiri
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include "my_tar.h"
#define TL_BLOCK 0x200

 int             gl_types[] =
   {
     S_IFIFO, FIFOTYPE, S_IFCHR, CHRTYPE, S_IFDIR, DIRTYPE,
     S_IFBLK, BLKTYPE, S_IFREG, REGTYPE,
     S_IFLNK, LNKTYPE, S_IFSOCK, CONTTYPE, 0, 0
   };

char            def_type(int st_mode)
{
  int           i;

  i = 0;
  while (gl_types[i + 1] != 0)
    {
      if ((st_mode & S_IFMT) == gl_types[i])
        return (gl_types[i + 1]);
      i += 2;
    }
  return ('?');
}

void		my_convert_oct(int nb, int len, char *buffer)
{
  memset(buffer, '0', len);
    len -= 1;
  if (nb == 0)
    buffer[len] = '0';
  while ((len >= 0) && (nb != 0))
    {
      buffer[len] = '0' + (nb & ((1 << 3) - 1));
      nb >>= 3;
      len -= 1;
    }
}

void		fill_name(struct s_tar *tarh, char *str)
{
  int i = 0;

  for (i = 0; str[i]; i++)
    tarh->fname[i] = str[i];
  return ;
}
