/*
** main.c for epitar in /u/a1/akadir_a/akadir_a-epitar/src
**
** Made by abass akadiri
** Login   <akadir_a@epita.fr>
**
** Started on  Sat Jul  8 16:52:05 2006 abass akadiri
** Last update Sat Jul  8 17:33:19 2006 abass akadiri
*/
#include "my_tar.h"
int	 main(int argc, char **argv)
{
  int	i = 0;
  int	block = 0;
  int	fd_dest = 0;
  char	buffer[512];

  if (argc < 2)
    return only_tar(argc);
  verif_opt(argv[1]);
  if(is_inside_c(argv[1]) == 'c')
    {
      if ((fd_dest = open(argv[2], O_RDWR|O_CREAT|O_TRUNC, 0666)) < 0)
	perror("error");
      for (i = 3; argv[i]; i++)
	{
	  block +=  exec_opt_line(argv[1], fd_dest, argv[i]);
	}
      memset(buffer, 0, 512);
      while (block % 20)
	{
	  block++;
	  write(fd_dest, buffer, 512);
	}
      close(fd_dest);
    }
  else
    {
      for (i = 2; argv[i]; i++)
	fd_dest = open(argv[i], O_RDONLY , 0666);
      exec_opt_line(argv[1], fd_dest, NULL);
      close(fd_dest);
    }
  return 0;
}
