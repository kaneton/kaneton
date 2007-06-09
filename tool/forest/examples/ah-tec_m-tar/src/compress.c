/*
** compress.c for epitar in /u/a1/ah-tec_m/ah-tec_m-tar/src
** 
** Made by mathieu ah-tec
** Login   <ah-tec_m@epita.fr>
** 
** Started on  Sat Jul  8 17:35:34 2006 mathieu ah-tec
** Last update Sat Jul  8 17:35:35 2006 mathieu ah-tec
*/

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "my_tar.h"

static int	err = 0;

static long	blk = 0;

extern int	verbose;

static void	get_header(char *buf, char *file, struct stat *st, char *stack[255], int sz)
{
  if (strlen(file) > 100)
    {
      printf("File name is too long\n");
      err = 1;
    }
  else
    {
      copy_filename(buf, file, stack, sz);
      copy_filemode(buf, st);
      copy_uid(buf, st);
      copy_gid(buf, st);
      copy_size(buf, st);
      copy_time(buf, st);
      copy_typeflag(buf, st);
      copy_magic(buf);
      copy_version(buf);
      copy_uname(buf, st);
      copy_gname(buf, st);
      copy_checksum(buf);
    }
}

void		compress_file(char *str, struct stat *st, FILE *res, char *stack[255], int stsz)
{
  char		*buf;
  FILE		*f;
  char		*temp;
  unsigned int	i;
  int		j;
  int		b;


  i = st->st_size;
  b = S_ISDIR(st->st_mode);
  f = fopen(str, "r");
  if ((f == NULL) && (!b))
    {
      printf("Cannot open %s\n", str);
      err = 1;
    }
  else
    {
      buf = calloc(sizeof (char), 512);
      get_header(buf, str, st, stack, stsz);
      blk += 512;
      fwrite(buf, sizeof (char), 512, res);
      if (!b)
	{
	  temp = malloc((i + 1) * sizeof(char));
	  temp[i] = '\0';
	  fread(temp, sizeof(char), i, f);
	  fwrite(temp, sizeof (char), i, res);
	  blk += i;
	  if (i % 512)
	    for (j = 0; (unsigned) j < (512 - (i % 512)); ++j, ++blk)
	      fwrite("", sizeof (char), 1, res);
	  fclose(f);
	  free(buf);
	}
    }
}

void		compress_directory(char *dir, char *stack[255], int *st_sz, int *st_len, FILE *res)
{
  DIR		*d;
  struct dirent	*dirent;
  struct stat	st;
  char		*pwd;
  int		i;

  pwd = getcwd(NULL, 0);
  d = opendir(dir);
  stat(dir, &st);
  chdir(dir);
  stack[(*st_sz)++] = dir;
  *st_len += strlen(dir) + 1;
  if (verbose)
    {
      for (i = 0; i < *st_sz; ++i)
	printf("%s/", stack[i]);
      printf("\n");
    }
  compress_file("", &st, res, stack, *st_sz);
  if (*st_len > 155)
    {
      printf("Path is too long ...\n");
      err = 1;
    }
  else
    while ((dirent = readdir(d)))
      {
	if (!(strcmp(dirent->d_name, ".") && (strcmp(dirent->d_name, ".."))))
	  continue;
	if (stat(dirent->d_name, &st) == -1)
	  {
	    printf("Cannot compress %s\n", dirent->d_name);
	    err = 1;
	  }
	else
	  {
	    if (S_ISDIR(st.st_mode))
	      {
		compress_directory(dirent->d_name, stack, st_sz, st_len, res);
	      }
	    else
	      {
		compress_file(dirent->d_name, &st, res, stack, *st_sz);
		if (verbose)
		  {
		    printf("%s/", *stack);
		    for (i = 1; i < *st_sz; ++i)
		      printf("%s/", stack[i]);
		    printf("%s\n", dirent->d_name);
		  }
		if (strlen(dirent->d_name) > 100)
		  {
		    printf("File Name is too long\n");
		    err = 1;
		  }
	      }
	  }
      }
  chdir(pwd);
  *st_sz -= 1;
  *st_len -= strlen(stack[*st_sz]) + 1;
  closedir(d);
}

void		write_end(FILE *res)
{
  int		i;
  int		j;

  for (i = 0; i < 2; ++i)
    for (j = 0; j < 512; ++j, ++blk)
      fwrite("", sizeof (char), 1, res);
  if (blk % 10240)
    for (i = 0; i < (10240 - (blk % 10240)); ++i)
      fwrite("", sizeof (char), 1, res);
}

int		compress_tar(int argc, char *argv[], char *str)
{
  int		i;
  struct stat	st;
  char		*stack[255];
  int		st_len = 0;
  int		st_sz = 0;
  FILE		*res;

  res = fopen(str, "w");
  for (i = 0; i < argc; ++i)
    {
      if (stat(argv[i], &st) == -1)
	{
	  printf("Cannot compress %s\n", argv[i]);
	  err = 1;
	}
      else
	{
	  if (S_ISDIR(st.st_mode))
	    compress_directory(argv[i], stack, &st_sz, &st_len, res);
	  else
	    compress_file(argv[i], &st, res, stack, st_sz);
	}
    }
  write_end(res);
  fclose(res);
  return err;
}
