/*
** bistro.h for bistro in /u/a1/quinta_j/rendu/c/bistro
**
** Made by julien quintard
** Login   <quinta_j@epita.fr>
**
** Started on  Wed Sep 15 16:28:31 2004 julien quintard
** Last update Wed Sep 15 21:40:19 2004 julien quintard
*/

#ifndef BISTRO_H_
# define BISTRO_H_

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# undef BISTRO_PARSER_DEBUG
# undef BISTRO_ALGO_DEBUG

# define BISTRO_DEFAULT_BASE	"0123456789"
# define BISTRO_DEFAULT_OPS	"+-*/%()"
# define BISTRO_EZERO		0x0
# define BISTRO_ESCAN		0x1
# define BISTRO_EPARSE		0x2
# define BISTRO_EARITH		0x3
# define BISTRO_EOTH		0x4

# define BISTRO_TOKEN_UNUSED	0x0
# define BISTRO_TOKEN_VALID	0x1

# define BISTRO_MODE_PREFIXE	0x1
# define BISTRO_MODE_SUFFIXE	0x2
# define BISTRO_MODE_POLINV	0x3

typedef int		t_bistro_number;
typedef char *		t_bistro_base;

struct			s_bistro_ops
{
  char			oparenth;
  char			cparenth;
  char			add;
  char			sub;
  char			mul;
  char			div;
  char			mod;
  char			sep;
};

struct			s_bistro_token
{
  char			type;
  char			token;
};

struct			s_bistro
{
  int			argc;
  char			**argv;
  int			mode;
  struct s_bistro_token	token;
  struct s_bistro_ops	ops;
  t_bistro_base		base;
  unsigned int		basesz;
};

/*
** bistro.c
*/

int			main(int argc, char **argv);

/*
** bistro_add.c
*/

t_bistro_number		bistro_add(t_bistro_number left,
				   t_bistro_number right);

/*
** bistro_div.c
*/

t_bistro_number		bistro_div(t_bistro_number left,
				   t_bistro_number right);

/*
** bistro_misc.c
*/

t_bistro_number		bistro_switch_sign(t_bistro_number number);
void			bistro_error(int err, char *str);
int			bistro_inbase(struct s_bistro *bistro, char c,
				      unsigned int *index);
char			bistro_indbase(struct s_bistro *bistro,
				       unsigned int index);
unsigned int		bistro_pow(unsigned int v, unsigned int e);

/*
** bistro_mod.c
*/

t_bistro_number		bistro_mod(t_bistro_number left,
				   t_bistro_number right);

/*
** bistro_mul.c
*/

t_bistro_number		bistro_mul(t_bistro_number left,
				   t_bistro_number right);

/*
** bistro_parser.c
*/

void			bistro_parser(struct s_bistro *bistro);

/*
** bistro_std.c
*/

void			bistro_dump_number(struct s_bistro *bistro,
					   t_bistro_number number);
int			bistro_strlen(char *str);
int			bistro_strcmp(char *s1, char *s2);

/*
** bistro_sub.c
*/

t_bistro_number		bistro_sub(t_bistro_number left,
				   t_bistro_number right);

#endif /* !BISTRO_H_ */
