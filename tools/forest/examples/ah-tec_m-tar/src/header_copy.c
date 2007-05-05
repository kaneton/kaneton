/*
** header_copy.c for epitar in /u/a1/ah-tec_m/ah-tec_m-tar/src
**
** Made by mathieu ah-tec
** Login   <ah-tec_m@epita.fr>
**
** Started on  Sat Jul  8 17:35:53 2006 mathieu ah-tec
** Last update Sat Jul  8 17:35:56 2006 mathieu ah-tec
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

void		copy_filename(char *buf, char *file, char *stack[255], int i)
{
  int		l;

  for (l = 0; l < i; ++l)
    {
      strcat(buf, stack[l]);
      strcat(buf, "/");
    }
  strcat(buf, file);
}

void		copy_filemode(char *buf, struct stat *st)
{
  unsigned int	a;
  int		i;

  a = st->st_mode & 07777;
  for (i = 106; i >= 100; --i)
    {
      buf[i] = '0' + (a % 8);
      a /= 8;
    }
}

void		copy_uid(char *buf, struct stat *st)
{
  unsigned int	a;
  int		i;

  a = st->st_uid;
  for (i = 114; i >= 108; --i)
    {
      buf[i] = '0' + (a % 8);
      a /= 8;
    }
}

void		copy_gid(char *buf, struct stat *st)
{
  unsigned int	a;
  int		i;

  a = st->st_gid;
  for (i = 122; i >= 116; --i)
    {
      buf[i] = '0' + (a % 8);
      a /= 8;
    }
}

void		copy_size(char *buf, struct stat *st)
{
  unsigned int	a;
  int		i;

  if (S_ISDIR(st->st_mode))
    a = 0;
  else
    a = st->st_size;
  for (i = 134; i >= 124; --i)
    {
      buf[i] = '0' + (a % 8);
      a /= 8;
    }
}

void		copy_time(char *buf, struct stat *st)
{
  unsigned int	a;
  int		i;

  a = st->st_mtime;
  for (i = 146; i >= 136; --i)
    {
      buf[i] = '0' + (a % 8);
      a /= 8;
    }
}

void	copy_typeflag(char *buf, struct stat *st)
{
  strcpy(buf + 148, "        ");
  if (S_ISREG(st->st_mode))
    buf[156] = '0';
  else
    buf[156] = '5';
}

void	copy_magic(char *buf)
{
  strcpy(buf + 257, "ustar");
}

void	copy_version(char *buf)
{
  strcpy(buf + 262, "  ");
}

void	copy_uname(char *buf, struct stat *st)
{
  struct passwd	*p;

  p = getpwuid(st->st_uid);
  strcpy(buf + 265, p->pw_name);
}

void	copy_gname(char *buf, struct stat *st)
{
  struct group	*p;

  p = getgrgid(st->st_gid);
  strcpy(buf + 297, p->gr_name);
}

/*copy_devmaj(buf, st);
copy_devmin(buf, st);
copy_prefix(buf, "");
*/

void	copy_checksum(char *buf)
{
  unsigned int	s = 0;
  int		i;

  for (i = 0; i < 512; ++i)
    s += (unsigned char) buf[i];
  sprintf(buf + 148, "%.6o", s);
}
