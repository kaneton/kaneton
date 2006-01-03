/*
** test_printf.c for  in /root/work/lseos/lseos-core/libc/printf/regress
** 
** Made by Vianney Rancurel
** Login   <vianney@epita.fr>
** 
** Started on  Sun Oct 24 18:53:17 2004 Vianney Rancurel
** Last update Wed Dec  1 15:17:04 2004 Vianney Rancurel
*/
#include <libc.h>

int     main(int argc, char **argv)
{
  printf("'%-4d'\n", 42);
  printf("'%-4qd'\n", 42LL);
  printf("'%4d'\n", 42);
  printf("'%04d'\n", 42);
  printf("'%.4d'\n", 42);
  printf("'%2.2d'\n", 42);
  printf("'%.3s'\n", "foobar");
  printf("'%.40sz'\n", "foobar");
  printf("'%3.40sz'\n", "foobar");
  printf("'%-3.40sz'\n", "foobar");
  printf("'%40sz'\n", "foobar");
  printf("'%-2.3s'\n", "foobar");
  printf("'%-2.3s'\n", "f");
  printf("'%*0d'\n", 5, 42);
  printf("'%-*0d'\n", 5, 42);
  printf("'%qb'\n", 42LL);
  printf("'%qB'\n", 42LL);
  printf("'%qB'\n", -1LL);
  printf("'%qb'\n", -1LL);
  printf("'%qu'\n", -1LL);
  
  return (0);
}
