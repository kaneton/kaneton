/*
** extract.c for epitar in /u/a1/ah-tec_m/ah-tec_m-tar/src
**
** Made by mathieu ah-tec
** Login   <ah-tec_m@epita.fr>
**
** Started on  Sat Jul  8 17:35:40 2006 mathieu ah-tec
** Last update Sat Jul  8 17:35:44 2006 mathieu ah-tec
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

extern int	verbose;

static int	err = 0;

static int	get_right(char *header)
{
  int		i;
  int		res = 0;

  for (i = 0; i < 7; ++i)
    res = 8 * res + (header[100 + i] - '0');
  return res;
}

static int	is_regfile(char *header)
{
  return (header[156] == '0') || (header[156] == '\0');
}

static int	is_empty(char *header)
{
  int		i = 0;

  while ((i < 512) && (header[i] == '\0'))
    ++i;
  return i == 512;
}

static void	check_header(char *header)
{
  int		i;
  unsigned long	s = 0;
  unsigned long r = 0;

  for (i = 0; i < 512; ++i)
    s += (((i < 148) || (i >= 156)) ? (unsigned char) header[i] : ' ');
  for (i = 0; i < 6; ++i)
    r = 8 * r + (header[148 + i] - '0');
  if (r != s)
    {
      printf("Bad checksum archive\n");
      exit(2);
    }
}

void		extract_data(char *header, char *name, FILE *f)
{
  unsigned int	i = 0;
  int		j;
  char		*buf;
  FILE		*s;
  char		dir[156];
  char		*p = NULL;

  if (header[345])
    {
      memcpy(dir, header + 345, 155);
      dir[155] = '\0';
      p = getcwd(NULL, 0);
      chdir(dir);
      if (verbose)
	printf("%s", dir);
    }
  if (verbose)
    printf("%s\n", name);
  for (j = 0; j < 11; ++j)
    i = i * 8 + (header[124 + j] - '0');
  buf = malloc(i * sizeof (char));
  fread(buf, sizeof (char), i, f);
  s = fopen(name, "w");
  if (s == NULL)
    {
      printf("Cannot write to file %s\n", name);
      exit(2);
    }
  fwrite(buf, sizeof (char), i, s);
  fclose(s);
  chmod(name, get_right(header));
  free(buf);
  if (p)
    {
      chdir(p);
      free(p);
    }
  if (i % 512)
    fseek(f, 512 - (i % 512), SEEK_CUR);
}

int		extract_block(FILE *f, char *header)
{
  char	name[101];

  fread(name, sizeof (char), 100, f);
  name[100] = '\0';
  memcpy(header, name, 100);
  fread(header + 100, sizeof (char), 412, f);
  if (is_empty(header))
    {
      fread(header, sizeof (char), 512, f);
      if (is_empty(header))
	return 0;
      else
	{
	  printf("File is corrupted\n");
	  exit(2);
	}
    }
  check_header(header);
  if (is_regfile(header))
    extract_data(header, name, f);
  else
    {
      if (verbose)
	printf("%s\n", header);
      mkdir(name, get_right(header));
    }
  return 1;
}

int	extract_tar(char *str)
{
  FILE *f;
  char	header[512];

  f = fopen(str, "r");
  if (f == NULL)
    {
      printf("File not found\n");
      exit(1);
    }
  while (extract_block(f, header))
    ;
  fclose(f);
  return err;
}
