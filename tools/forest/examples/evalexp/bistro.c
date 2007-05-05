/*
** bistro.c for bistro in /u/a1/quinta_j/rendu/c/bistro
**
** Made by julien quintard
** Login   <quinta_j@epita.fr>
**
** Started on  Wed Sep 15 16:28:26 2004 julien quintard
** Last update Fri Sep 17 18:17:04 2004 julien quintard
*/

#include "bistro.h"

static void		bistro_read_base(char **base, int argc,
					 char **argv)
{
  int			i;

  for (i = 0; i < argc; i++)
    if (bistro_strcmp(argv[i], "-b") == 0)
      {
	if ((i + 1) < argc)
	  {
	    *base = argv[i + 1];
	    return;
	  }
	else
	  exit(BISTRO_EOTH);
      }
  *base = BISTRO_DEFAULT_BASE;
}

static void		bistro_read_ops(char **ops, int argc,
					char **argv)
{
  int			i;

  for (i = 0; i < argc; i++)
    if (bistro_strcmp(argv[i], "-o") == 0)
      {
	if ((i + 1) < argc)
	  {
	    *ops = argv[i + 1];
	    return;
	  }
	else
	  exit(BISTRO_EOTH);
      }
  *ops = BISTRO_DEFAULT_OPS;
}

static void		bistro_init(struct s_bistro *bistro,
				    int argc, char **argv)
{
  char			*ops;

  bistro_read_ops(&ops, argc, argv);
  bistro->argc = argc;
  bistro->argv = argv;
  bistro->mode = BISTRO_MODE_POLINV;
  bistro->token.type = BISTRO_TOKEN_UNUSED;
  bistro->token.token = 0;
  bistro->ops.add = ops[0];
  bistro->ops.sub = ops[1];
  bistro->ops.mul = ops[2];
  bistro->ops.div = ops[3];
  bistro->ops.mod = ops[4];
  bistro->ops.oparenth = ops[5];
  bistro->ops.cparenth = ops[6];
  bistro_read_base(&bistro->base, argc, argv);
  bistro->basesz = bistro_strlen(bistro->base);
}

int			main(int argc, char **argv)
{
  struct s_bistro	bistro;

  bistro_init(&bistro, argc, argv);
  bistro_parser(&bistro);
  return BISTRO_EZERO;
}
