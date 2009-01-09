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

void debug_shell_call(const char *arg);

static struct debug_shell_cmd_t        debug_shell_cmd[] = 
{
  {"call ", debug_shell_call}
};

void debug_shell(char   (*i_read)(void))
{
  char  buff[512];
  char  *p = buff;
  int   i = 0;

  while ((*p = i_read()))
  {
    if (*p == '\n')
    {
      for (i = 0; i < array_size(debug_shell_cmd); i++)
        if (!strncmp(buff,
                     debug_shell_cmd[i].cmd,
                     strlen(debug_shell_cmd[i].cmd)))
        {
            debug_shell_cmd[i].fn(buff + strlen(debug_shell_cmd[i].cmd));
            return;
        }
    }
    else
      p++;
  }
}

void debug_shell_call(const char *arg)
{
  debug_callbacks_call(arg);
}

