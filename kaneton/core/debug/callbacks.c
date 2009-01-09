/*
** callbacks.c
**
** Jean Guyader <jean.guyader@gmail.com>
**
*/

#include <libc/libc.h>
#include <kaneton.h>
#include <core/debug.h>

struct debug_callbacks_t
{
  const char    *name;
  void          (*fn)(void);
};

static struct debug_callbacks_t *debug_callbacks = 0;

void    debug_callbacks_register(const char *name, void (*fn)(void))
{
  int   s = 0;

  for (s = 0; debug_callbacks && debug_callbacks[s].name; s++)
    ;

  debug_callbacks = realloc(debug_callbacks, (s + 2) / sizeof (struct debug_callbacks_t));
  debug_callbacks[s].name = name;
  debug_callbacks[s].fn = fn;
  debug_callbacks[s + 1].name = 0;
}

void    debug_callbacks_call(const char *name)
{
  int   i = 0;

  for (i = 0; debug_callbacks && debug_callbacks[i].name; i++)
    if (!strcmp(debug_callbacks[i].name, name))
    {
      debug_callbacks[i].fn();
      return;
    }
}
