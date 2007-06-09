/*
** bistro_parser.c for bistro in /u/a1/quinta_j/rendu/c/bistro
**
** Made by julien quintard
** Login   <quinta_j@epita.fr>
**
** Started on  Wed Sep 15 16:29:19 2004 julien quintard
** Last update Thu Dec  9 21:49:17 2004 julien quintard
*/

#include "bistro.h"

static void		bistro_next_token(struct s_bistro *bistro);
static t_bistro_number	bistro_read_number(struct s_bistro *bistro);
static t_bistro_number	bistro_parser_v(struct s_bistro *bistro,
					t_bistro_number leftn);
static t_bistro_number	bistro_parser_u(struct s_bistro *bistro,
 					t_bistro_number leftn);
static t_bistro_number	bistro_parser_p(struct s_bistro *bistro);
static t_bistro_number	bistro_parser_t(struct s_bistro *bistro);
static t_bistro_number	bistro_parser_e(struct s_bistro *bistro);
static t_bistro_number	bistro_parser_s(struct s_bistro *bistro);

static void		bistro_next_token(struct s_bistro *bistro)
{
  char			c;

  if (bistro->token.type == BISTRO_TOKEN_VALID)
    return;
  if (read(STDIN_FILENO, &c, 1) != 1)
    {
      bistro->token.type = BISTRO_TOKEN_UNUSED;
      bistro->token.token = 0;
      return;
    }
  bistro->token.type = BISTRO_TOKEN_VALID;
  bistro->token.token = c;
}

static t_bistro_number	bistro_read_number(struct s_bistro *bistro)
{
  t_bistro_number	number;
  unsigned int		value;
  unsigned int		size;

  BISTRO_INIT_NUMBER(number);
  for (size = 0;
       bistro_inbase(bistro, bistro->token.token, &value) != -1;
       size++)
    {
      number *= bistro->basesz;
      number += value;
      BISTRO_TOKEN_OK(bistro);
      bistro_next_token(bistro);
    }
  if (size == 0)
    bistro_error(BISTRO_ESCAN, "[error] expected: number\n");
  return number;
}

static t_bistro_number	bistro_parser_v(struct s_bistro *bistro,
					t_bistro_number leftn)
{
  t_bistro_number	number;

  BISTRO_INIT_NUMBER(number);
  BISTRO_TOKEN_EAT(bistro);
  if (BISTRO_TKCMP(bistro, bistro->ops.mul))
    {
      BISTRO_MUL(bistro, leftn, number);
    }
  else if (BISTRO_TKCMP(bistro, bistro->ops.div))
    {
      BISTRO_DIV(bistro, leftn, number);
    }
  else if (BISTRO_TKCMP(bistro, bistro->ops.mod))
    {
      BISTRO_MOD(bistro, leftn, number);
    }
  else
    number = leftn;
  return number;
}

static t_bistro_number	bistro_parser_u(struct s_bistro *bistro,
					t_bistro_number leftn)
{
  t_bistro_number	number;

  BISTRO_INIT_NUMBER(number);
  BISTRO_TOKEN_EAT(bistro);
  if (BISTRO_TKCMP(bistro, bistro->ops.add))
    {
      BISTRO_ADD(bistro, leftn, number);
    }
  else if (BISTRO_TKCMP(bistro, bistro->ops.sub))
    {
      BISTRO_SUB(bistro, leftn, number);
    }
  else
    number = leftn;
  return number;
}

static t_bistro_number	bistro_parser_p(struct s_bistro *bistro)
{
  t_bistro_number	number;

  BISTRO_INIT_NUMBER(number);
  BISTRO_TOKEN_EAT(bistro);
  if (BISTRO_TKCMP(bistro, bistro->ops.oparenth))
    {
      BISTRO_OPARENTH(bistro, number);
    }
  else if (BISTRO_TKCMP(bistro, bistro->ops.add))
    {
      BISTRO_PLUS(bistro, number);
    }
  else if (BISTRO_TKCMP(bistro, bistro->ops.sub))
    {
      BISTRO_MINUS(bistro, number);
    }
  else
    number = bistro_read_number(bistro);
  return number;
}

static t_bistro_number	bistro_parser_t(struct s_bistro *bistro)
{
  t_bistro_number	number;

  BISTRO_INIT_NUMBER(number);
  BISTRO_TOKEN_EAT(bistro);
  number = bistro_parser_p(bistro);
  number = bistro_parser_v(bistro, number);
  return number;
}

static t_bistro_number	bistro_parser_e(struct s_bistro *bistro)
{
  t_bistro_number	number;

  BISTRO_INIT_NUMBER(number);
  BISTRO_TOKEN_EAT(bistro);
  number = bistro_parser_t(bistro);
  number = bistro_parser_u(bistro, number);
  return number;
}

static t_bistro_number	bistro_parser_s(struct s_bistro *bistro)
{
  t_bistro_number	number;

  BISTRO_INIT_NUMBER(number);
  BISTRO_TOKEN_EAT(bistro);
  if (bistro->token.type == BISTRO_TOKEN_UNUSED)
    exit(BISTRO_EZERO);
  number = bistro_parser_e(bistro);
  bistro_next_token(bistro);
  if (bistro->token.type == BISTRO_TOKEN_VALID)
    bistro_error(BISTRO_EPARSE, "[error] unexpected character\n");
  return number;
}

void			bistro_parser(struct s_bistro *bistro)
{
  t_bistro_number	number;

  BISTRO_INIT_NUMBER(number);
  number = bistro_parser_s(bistro);
  bistro_dump_number(bistro, number);
}
