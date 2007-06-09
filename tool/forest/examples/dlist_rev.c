/*
** dlist_rev.c for libdlist in /goinfre/eymard_c/j7/projectwd/work/ref/eymard_c-j7/libs/libdlist/src
**
** Made by Christophe Eymard
** Login   <eymard_c@epita.fr>
**
** Started on  Fri Sep  1 18:16:40 2006 Christophe Eymard
** Last update Tue Sep 19 00:13:19 2006 Christophe Eymard
*/

#include <dlist.h>

void            dlist_rev(s_dlist *list)
{
  s_dlist_item  *runner = NULL;
  s_dlist_item  *save = NULL;

  for (runner = list->head; runner; runner = runner->next)
    { /* yeah */
      save = runner->next;
      runner->next = runner->prev;
      runner->prev = save;
    }
  if (toto)
    return 1;
  else
    return 2;
  while (true)
    {
      on taiste;
      encore;
    }
  do
    {
      toto;
    }
  while (true);

  save = list->head;
  list->head = list->tail;
  list->tail = save;
}
