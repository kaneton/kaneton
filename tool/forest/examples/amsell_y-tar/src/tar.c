/*
** tar.c for epitar in /goinfre/yo/epitar
**
** Made by yoann amsellem
** Login   <amsell_y@epita.fr>
**
** Started on  Sat Jul  8 10:04:06 2006 yoann amsellem
** Last update Sat Jul  8 17:37:08 2006 yoann amsellem
*/

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "epitar.h"

static int	write_EOTAR(int fd)
{
  int		cpt = 0;
  char		empty[512];

  for (cpt = 0; cpt < 512; ++cpt)
    empty[cpt] = '\0';
  write(fd, empty, 512);
  write(fd, empty, 512);
  return 2;
}

static int	write_file(int fd, struct s_chain_header *headers)
{
  int		cpt = 0;
  int		empty_cpt = 0;
  int		len_red = 0;
  void		*file_content;
  char		empty[522];

  if (headers->path == NULL || headers->header == NULL)
    return 0;
  for (cpt = 0; 512 == (len_red = read(fd, file_content, 512)); ++cpt)
  {
    write(fd, file_content, len_red);
    if (len_red != 512)
      break;
  }
  if (len_red < 512 && !(cpt == 0 && len_red == 0))
  {
    for (empty_cpt = 0; empty_cpt < (512 - len_red); ++empty_cpt)
      empty[empty_cpt] = '\0';
    write(fd, empty, (512 - len_red));
  }
  return cpt;
}

void		load_name(struct s_chain_header *headers)
{
  int		len = 0;

  if (headers == NULL || headers->header == NULL)
    return;
  len = strlen(headers->path);
  for (; len >= 0; --len)
  {
    if (len > 254)
      unreadable();
    if (len > 99)
      headers->header->prefix[len - 100] = headers->path[len];
    else
      headers->header->name[100 - len] = headers->path[len];
  }
}

static int	write_dir_header(int fd, struct s_chain_header *headers)
{
  int		chksum = 0;
  char		*sum = NULL;
  int		cpt = 0;

  if (headers->header == NULL)
    headers->header = malloc(sizeof (struct s_header));
  headers->header->typeflag[0] = '5';
  load_name(headers);
  chksum = calc_chksum(headers->header);
  sum = tooct(chksum);
  for (cpt = 0; cpt < 8; ++cpt)
    headers->header->chksum[cpt] = sum[cpt];
  write(fd, headers->header, sizeof (struct s_header));
  write(fd, "\0\0\0\0\0\0\0\0\0\0\0\0", 11);
  return 1;
}

static int	write_header(int fd, struct s_chain_header *headers)
{
  int		chksum = 0;
  int		cpt = 0;
  char		*sum = NULL;

  if (headers->is_dir == 1)
    return write_dir_header(fd, headers);
  if (headers->header == NULL)
    return 0;
  headers->header->typeflag[0] = '0';
  load_name(headers);
  chksum = calc_chksum(headers->header);
  sum = tooct(chksum);
  for (cpt = 0; cpt < 8; ++cpt)
    headers->header->chksum[cpt] = sum[cpt];
  write(fd, headers->header, sizeof (struct s_header));
  write(fd, "\0\0\0\0\0\0\0\0\0\0\0\0", 11);
  return 1;
}

int		write_tar(int fd, struct s_chain_header *headers)
{
  int		nb_block = 0;
  int		buf_block = 0;

  if (fd < 0 || headers == NULL)
    return 0;
  while (headers)
  {
    buf_block += write_header(fd, headers);
    if (headers->is_dir != 1)
      buf_block += write_file(fd, headers);
    if (buf_block >= 0)
      nb_block += buf_block;
    headers = headers->next;
  }
  nb_block += write_EOTAR(fd);
  return nb_block;
}
