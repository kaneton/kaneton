/*
** main.c for epitar in /goinfre/yo/epitar/src
**
** Made by yoann amsellem
** Login   <amsell_y@epita.fr>
**
** Started on  Thu Jul  6 11:14:32 2006 yoann amsellem
** Last update Sat Jul  8 17:35:59 2006 yoann amsellem
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include "epitar.h"

void			launch_tar(int fd, char *file, int verbose)
{
  struct s_chain_header	*header = NULL;
  struct s_chain_header	*base = NULL;

  if (NULL == (header = scan_dir(file, 1)))
    return;
  if (fd < 0)
    unreadable();
  base = header;
  while(header != NULL && verbose == 1)
  {
    printf("%s\n", header->path);
    header = header->next;
  }
  fill_headers(base);
  write_tar(fd, base);
}

int			go_back(char *argv[])
{
  if (strcmp(argv[2], "x") && strcmp(argv[2], "-x") &&
      strcmp(argv[2], "c") && strcmp(argv[2], "-c") &&
      strcmp(argv[2], "xv") && strcmp(argv[2], "-xv") &&
      strcmp(argv[2], "cv") && strcmp(argv[2], "-cv") &&
      strcmp(argv[2], "v") && strcmp(argv[2], "-v"))
    return 1;
  return 0;
}

static void		check_args(int argc, char *argv[], int cpt, int dif)
{
  int			fd = 0;

  while (((++cpt) - dif) < argc)
  {
    if ((fd = open(argv[cpt - dif], O_RDONLY, 0666)) < 0)
      unreadable();
  }
}

static void		check_self(int argc, char *argv[], int cpt, int dif)
{
  int			begin = 0;

  begin = (++cpt) - dif;
  while (((++cpt) - dif) < argc)
  {
    if (argv[begin] == NULL || argv[cpt - dif] == NULL)
      return;
    if (!strcmp(argv[begin], argv[cpt - dif]))
    {
      write(STDERR_FILENO, "Tarball is self-included, aborting.\n", 36);
      exit(1);
    }
  }
}

int			main(int argc, char *argv[])
{
  int			cpt = 0;
  int			verbose = 0;
  int			compress = -1;
  int			dif = 0;
  int			compress_fd = 0;

  if (argc < 3)
    unreadable();
  for (cpt = 1; cpt < 3; ++cpt)
  {
    if (0 == strcmp(argv[cpt], "v") || 0 == strcmp(argv[cpt], "-v"))
      verbose = 1;
    if ((0 == strcmp(argv[cpt], "cv") || 0 == strcmp(argv[cpt], "-cv") ||
	 0 == strcmp(argv[cpt], "vc") || 0 == strcmp(argv[cpt], "-vc")) &&
	(verbose = 1) == 1 && (compress = 1) == 1)
      break;
    if ((0 == strcmp(argv[cpt], "xv") || 0 == strcmp(argv[cpt], "-xv") ||
	 0 == strcmp(argv[cpt], "vx") || 0 == strcmp(argv[cpt], "-vx")) &&
	(verbose = 1) == 1 && (compress = 0) == 0)
      break;
    if (0 == strcmp(argv[cpt], "c") || 0 == strcmp(argv[cpt], "-c"))
      compress = 1;
    if (0 == strcmp(argv[cpt], "x") || 0 == strcmp(argv[cpt], "-x"))
      compress = 0;
  }
  if (compress == -1)
    unreadable();
  if (cpt == 3 && go_back(argv) == 1)
    dif = 2;
  check_args(argc, argv, (cpt += ((compress == 1) ? 1 : 0)), dif);
  if (compress == 0)
  {
    while (((++cpt) - dif) < argc)
      untar(argv[cpt - dif], verbose);
  }
  else
  {
    compress_fd = open(argv[cpt - dif], O_CREAT | O_TRUNC | O_RDWR, 0666);
    check_self(argc, argv, cpt, dif);
    while (((++cpt) - dif) < argc)
      launch_tar(compress_fd, argv[cpt - dif], verbose);
    close(compress_fd);
  }
  return 0;
}
