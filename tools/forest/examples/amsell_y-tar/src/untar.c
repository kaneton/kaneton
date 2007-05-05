/*
** untar.c for epitar in /goinfre/yo/epitar/src
**
** Made by yoann amsellem
** Login   <amsell_y@epita.fr>
**
** Started on  Wed Jul  5 23:46:44 2006 yoann amsellem
** Last update Sat Jul  8 17:37:28 2006 yoann amsellem
*/

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/dirent.h>
#include <sys/time.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <utime.h>
#include "epitar.h"

void			unreadable(void)
{
  write(STDERR_FILENO, "can't open tarball: No such file or directory.\n", 47);
  exit(1);
}

int			get_mode(char *mod)
{
  int			cpt = 0;
  char			res[9];

  if (mod == NULL)
    return 0;
  for (cpt = 0; cpt < 8; ++cpt)
    res[cpt] = mod[cpt];
  res[cpt] = '\0';
  return my_getnbr_base(res, "01234567");
}

static int		get_size(char *sz)
{
  int			cpt = 0;
  char			res[13];

  if (sz == NULL)
    return 0;
  for (cpt = 0; cpt < 12; ++cpt)
    res[cpt] = sz[cpt];
  res[cpt] = '\0';
  return my_getnbr_base(res, "01234567");
}

static int		get_id(char *s)
{
  int			cpt = 0;
  char			res[8];

  if (s == NULL)
    return 0;
  for (cpt = 0; cpt < 8; ++cpt)
    res[cpt] = s[cpt];
  res[cpt] = '\0';
  return my_getnbr_base(res, "01234567");
}

static int		get_time(char *s)
{
  int			cpt = 0;
  char			res[12];

  if (s == NULL)
    return 0;
  for (cpt = 0; cpt < 12; ++cpt)
    res[cpt] = s[cpt];
  res[cpt] = '\0';
  return my_getnbr_base(res, "01234567");
}

void			extract_file(int fd, struct s_chain_header *header)
{
  void			*file_content = NULL;
  int			file_sz = 0;
  int			new_fd = 0;

  if (header == NULL || header->header == NULL)
    return;
  if (header->header->typeflag[0] == '5')
  {
    if (mkdir(header->path, get_mode(header->header->mode)) < 0)
      unreadable();
    else
      return;
  }
  if ((file_sz = get_size(header->header->size)) != 0)
  {
    file_content = malloc(file_sz * sizeof (char));
    if (file_sz != read(fd, file_content, file_sz))
      unreadable();
    lseek(fd, 512 - (lseek(fd, 0, SEEK_CUR) % 512), SEEK_CUR);
  }
  if ((new_fd = open(header->path, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0 ||
      file_sz != write(new_fd, file_content, file_sz))
    unreadable();
  free(file_content);
  close(new_fd);
}

void			untar(const char *filename, int verbose)
{
  int			fd = 0;
  int			nb_null = 0;
  struct utimbuf	timep;
  struct s_chain_header	*headers = NULL;

  if (filename != NULL && strlen(filename) == 1 && filename[0] == '-')
    fd = STDIN_FILENO;
  else if (filename == NULL || (fd = open(filename , O_RDONLY, 0666)) < 0)
    unreadable();
  while (nb_null < 2)
  {
    if (NULL == (headers = add_header(headers, fd)) || headers->header == NULL)
      unreadable();
    if (is_null_header(headers->header))
    {
      ++nb_null;
      continue;
    }
    else
    {
      check_chksum(headers->header, calc_chksum(headers->header));
      extract_file(fd, headers);
      chmod(headers->path, get_mode(headers->header->mode));
      chown(headers->path, get_id(headers->header->uid), get_id(headers->header->gid));
      timep.modtime = get_time(headers->header->mtime);
      utime(headers->path, &timep);
      nb_null = 0;
    }
    if (verbose == 1)
      print_filename(headers);
  }
  close(fd);
}
