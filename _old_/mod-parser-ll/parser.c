// dire dans la doc que si une valeur est compose de plusieurs mots ou
// que les espaces/tabulations/retours a la ligne sont importants il faut
// utiliser des guillemets

// limiter les caracteres qui font un label comme ca on peut coller le :
// au nom et le specifier dans la doc

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

char*		file = NULL;
unsigned int	filesz = 0;

char*		token = NULL;

typedef struct
{
  char*		name;
  char*		value;
}		t_conf_param;

typedef struct
{
  char*		name;
  char*		value;
}		t_conf_arg;

typedef struct
{
  char*		name;

  t_conf_param*	params;
  unsigned int	nparams;

  t_conf_arg*	args;
  unsigned int	nargs;
}		t_conf_mod;

typedef struct
{
  t_conf_mod*	mods;
  unsigned int	nmods;
}		t_conf;

t_conf		conf;

int		read_file(char*	filename)
{
  struct stat	st;
  int		fd;

  if (stat(filename, &st) == -1)
    return (-1);

  if ((file = malloc(st.st_size)) == NULL)
    return (-1);

  if ((fd = open(filename, O_RDONLY)) == -1)
    return (-1);

  read(fd, file, st.st_size);

  filesz = st.st_size;
}

char*		get_token(unsigned int*	i)
{
  char*		t;
  unsigned int	s;

  if (*i >= filesz)
    return (NULL);

  while (((file[*i] == ' ') || (file[*i] == '\t') || (file[*i] == '\n')) &&
	  (*i < filesz))
    (*i)++;

  s = *i;

  if (file[*i] == '"') // gerer le ' et les escapes
    {
      (*i)++;

      while ((file[*i] != '"') && (*i < filesz))
	(*i)++;

      (*i)++;

      token = malloc(*i - s + 1);
      memset(token, 0x0, *i - s + 1);

      strncpy(token, file + s, *i - s);

      return (token);
    }

  while (((file[*i] != ' ') && (file[*i] != '\t') && (file[*i] != '\n')) &&
	 (*i < filesz))
    (*i)++;

  if (*i == s)
    return (NULL);

  token = malloc(*i - s + 1);

  strncpy(token, file + s, *i - s);

  return (token);
}

void		new_arg(char*		name,
			char*		value)
{
  unsigned int	i = conf.nmods - 1;
  unsigned int	j = conf.mods[i].nargs++;

  conf.mods[i].args = realloc(conf.mods[i].args,
			      conf.mods[i].nargs * sizeof(t_conf_arg));

  memset(&conf.mods[i].args[j], 0x0, sizeof(t_conf_arg));

  conf.mods[i].args[j].name = name;
  conf.mods[i].args[j].value = value;
}

void		args(unsigned int*	i)
{
  char*		separator = NULL;
  char*		value = NULL;
  char*		name = NULL;

  if (strcmp(token, "{") != 0)
    return;

  free(token);
  token = NULL;

  name = get_token(i);

  do
    {
      separator = get_token(i);

      free(token);
      token = NULL;

      value = get_token(i);

      new_arg(name, value);

      name = get_token(i);
    } while (strcmp(token, "}") != 0);

  free(token);
  token = NULL;

  get_token(i);
}

void		new_param(char*		name,
			  char*		value)
{
  unsigned int	i = conf.nmods - 1;
  unsigned int	j = conf.mods[i].nparams++;

  conf.mods[i].params = realloc(conf.mods[i].params,
				conf.mods[i].nparams * sizeof(t_conf_param));

  memset(&conf.mods[i].params[j], 0x0, sizeof(t_conf_param));

  conf.mods[i].params[j].name = name;
  conf.mods[i].params[j].value = value;
}

void		params(unsigned int*	i)
{
  char*		separator = NULL;
  char*		value = NULL;
  char*		name = NULL;

  if (strcmp(token, "[") != 0)
    return;

  free(token);
  token = NULL;

  name = get_token(i);

  do
    {
      separator = get_token(i);

      free(token);
      token = NULL;

      value = get_token(i);

      new_param(name, value);

      name = get_token(i);
    } while (strcmp(token, "]") != 0);

  free(token);
  token = NULL;

  get_token(i);
}

void		new_mod(char*		name)
{
  unsigned int	i = conf.nmods++;

  conf.mods = realloc(conf.mods, conf.nmods * sizeof(t_conf_mod));

  memset(&conf.mods[i], 0x0, sizeof(t_conf_mod));

  conf.mods[i].name = name;
  conf.mods[i].params = NULL;
  conf.mods[i].args = NULL;
}

void		mods(unsigned int*	i)
{
  new_mod(token);

  get_token(i);

  params(i);
  args(i);
}

void		garbage(unsigned int*	i)
{
  while (get_token(i) != NULL)
    printf("--- %s\n", token);
}

void		parse(unsigned int	i)
{
  if (get_token(&i) == NULL)
    return ;

  while (token != NULL)
    mods(&i);
}

void		dump(void)
{
  unsigned int	m;
  unsigned int	i;

  for (m = 0; m < conf.nmods; m++)
    {
      printf("%s\n", conf.mods[m].name);

      printf("  params:\n");

      for (i = 0; i < conf.mods[m].nparams; i++)
	printf("    %s: %s\n",
	       conf.mods[m].params[i].name,
	       conf.mods[m].params[i].value);

      printf("  args:\n");

      for (i = 0; i < conf.mods[m].nargs; i++)
	printf("    %s: %s\n",
	       conf.mods[m].args[i].name,
	       conf.mods[m].args[i].value);

      printf("\n");
    }
}

int		main(int	argc,
		     char**	argv)
{
  read_file("kaneton.conf");

  memset(&conf, 0x0, sizeof(t_conf));

  parse(0);

  dump();

  return (0);
}
