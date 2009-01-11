/*
** shell.c 
**
** Made by Jean Guyader
** Login   <johnweb@epita.fr>
**
** Started on  Ven 19 déc 20:24:28 2008 Jean Guyader
** Last update Ven 19 déc 20:24:28 2008 Jean Guyader
*/

#include <libc/libc.h>
#include <kaneton.h>

struct debug_shell_cmd_t
{
  const char    *cmd;
  void          (*fn)();
};

struct debug_shell_config_t
{
	const char	*name;
	int			val;
};

static struct debug_shell_config_t debug_shell_config[] =
{
	{"noecho", 0},
};

static struct debug_shell_cmd_t        debug_shell_cmd[] = 
{
  {"call ", debug_shell_call},
  {"echo ", debug_shell_echo},
  {"set ", debug_shell_set},
  {"unset ", debug_shell_unset},
};

void debug_shell(char   (*i_read)(void))
{
  char  buff[512];
  char  *p = buff;
  int   i = 0;

  while ((*p = i_read()))
  {
	  if (*p == '\r' || *p == '\n')
	  {
		  if (!debug_shell_config_isset("noecho"))
			  printf("\n");

		  *p = 0;
		  for (i = 0; i < array_size(debug_shell_cmd); i++)
			  if (!strncmp(buff,
						   debug_shell_cmd[i].cmd,
						   strlen(debug_shell_cmd[i].cmd)))
			  {
				  debug_shell_cmd[i].fn(buff + strlen(debug_shell_cmd[i].cmd));
			  }
		  p = buff;
		  *p = 0;
	  }
	  else
	  {
		  if (!debug_shell_config_isset("noecho"))
		  {
			  serial_put(*p);
			  serial_put(-1);
		  }
		  p++;
	  }
  }
}

void debug_shell_echo(const char *arg)
{
	printf("%s\n", arg);
}

void debug_shell_call(const char *arg)
{
	debug_callbacks_call(arg);
	cons_msg('+', "%s called\n", arg);
}

void debug_shell_config_set(const char *arg, int val)
{
	int i;

	for (i = 0; i < array_size(debug_shell_config); i++)
		if (!strcmp(arg, debug_shell_config[i].name))
			debug_shell_config[i].val = val;
}

int debug_shell_config_isset(const char *arg)
{
	int i = 0;

	for (i = 0; i < array_size(debug_shell_config); i++)
		if (!strcmp(arg, debug_shell_config[i].name))
			return debug_shell_config[i].val;
}

void debug_shell_set(const char *arg)
{
	debug_shell_config_set(arg, 1);
}

void debug_shell_unset(const char *arg)
{
	debug_shell_config_set(arg, 0);
}
