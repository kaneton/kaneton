#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "my_tar.h"

#define INVALIDE_OPT 64

typedef struct		s_opt
{
  char			opt;
  int			num_opt;
}			t_opt;

t_opt			gl_opt[] =
  {
    {'c', 1},
    {'f', 2},
    {'x', 3},
    {'v', 4},
    {'t', 5},
    {0,0}
  };

int is_inside(char c,  t_opt tab[])
{
  int i = 0;

  for (i = 0; tab[i].opt != 0; i++)
    {
      if (c == tab[i].opt)
	return 0;
    }
  return 1;
}
int		exec_opt_line(char *line, int dest, char *source)
{
  int		i = 0;
  int		block = 0;
  int		length = 0;
  DIR *DirectoryPointer = NULL;
  struct dirent *dp = NULL;
  struct stat *sb = NULL;
  struct s_tar *tarh = NULL;

  if ((dest < 0) && (!source))
    {
      printf("epitar: missing argument\nTry `./epitar --help`\n");
      exit (1);
    }
  if (line[i] == '-')
    i++;
  if (line[i] == 'c')
    {
      sb = malloc(sizeof(struct stat));
      stat(source, sb);
      if (S_ISDIR(sb->st_mode))
	{
	  tarh = malloc(sizeof(struct s_tar));
	  printf("je suis un rep Merci\n");
	  if (line[i + 1] == 'v')
	    printf("%s\n", creatpath(source, ""));
	  memset(tarh->fname, 0, 100);
	  strcpy(tarh->fname, source);
	  fill_name(tarh, creatpath(source, ""));
	  fill_header_first(tarh, sb);
	  fill_header_two(tarh, sb);
	  fill_header_three(tarh, sb);
	  length +=  write (dest, tarh, 512);
	  block++;
	  DirectoryPointer = opendir(source);
	  for (dp = readdir(DirectoryPointer); dp != NULL; dp = readdir(DirectoryPointer))
	    {
	      if ((!strcmp(dp->d_name, ".")) || (!strcmp(dp->d_name, "..")))
		continue;
	      else
		{
		  if (line[i + 1] == 'v')
		  block +=  write_file(dest, creatpath(source, dp->d_name), 1);
		  else
		   block += write_file(dest, creatpath(source, dp->d_name), 0);
		}
	    }
	  closedir(DirectoryPointer);
	}
      else
	{
	  if (line[i + 1] == 'v')
	    block += write_file(dest, source, 1);
	  else
	    block += write_file(dest, source, 0);
	}
      free(sb);
      free(tarh);
    }
  if (line[i] == 'x')
    {
      if (line[i +  1] == 'v')
	xtract_file_aux(dest, 1);
      else
	xtract_file_aux(dest, 0);
    }
  return block;
}

int parse_opt(char *argv)
{
  if (!strcmp(argv, "--help"))
   fprintf(stderr, "epitar: Usage: ./epitar [cvjf] <file.tar> <files...>\nReport bug to akadir_a@epita.fr\n");
  return 0;
}

int verif_opt(char *argv)
{
  int i = 0;

  if (argv[i] == '-' && argv[i+1] == '-')
    {
      parse_opt(argv);
      exit(0);
    }
  else if (argv[i] == '-')
    i++;
    for (; argv[i]; i++)
      {
	if (is_inside(argv[i], gl_opt))
	  {
	    invalid_option(argv[i]);
	    exit(INVALIDE_OPT);
	  }
      }
  return 0;
}
