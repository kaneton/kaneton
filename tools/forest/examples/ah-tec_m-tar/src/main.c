/*
** main.c for epitar in /u/a1/ah-tec_m/ah-tec_m-tar/src
**
** Made by mathieu ah-tec
** Login   <ah-tec_m@epita.fr>
**
** Started on  Sat Jul  8 17:36:02 2006 mathieu ah-tec
** Last update Sat Jul  8 17:36:04 2006 mathieu ah-tec
*/

#include <stdio.h>
#include "my_tar.h"

int		verbose = 0;

static int	print_usage()
{
  printf("Usage : ./epitar -x[v] <tarball>\n");
  printf("        ./epitar -c[v] <file> <path>+\n");
  return 1;
}

int	check_usage(int argc, char *argv[])
{
  if (argc < 3)
    return print_usage();
  if (argv[1][0] != '-')
    return print_usage();
  if ((argc == 3) && (argv[1][1] == 'x') && (argv[1][2] == '\0'))
    return extract_tar(argv[2]);
  if ((argc == 3) && (argv[1][1] == 'x') && (argv[1][2] == 'v') && (argv[1][3] == '\0'))
    {
      verbose = 1;
      return extract_tar(argv[2]);
    }
  if ((argc > 3) && (argv[1][1] == 'c') && (argv[1][2] == '\0'))
    return compress_tar(argc - 3, argv + 3, argv[2]);
  if ((argc > 3) && (argv[1][1] == 'c') && (argv[1][2] == 'v') && (argv[1][3] == '\0'))
    {
      verbose = 1;
      return compress_tar(argc - 3, argv + 3, argv[2]);
    }
  return print_usage();
}

int	main(int argc, char *argv[])
{
  return check_usage(argc, argv);
}
