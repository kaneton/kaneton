/*
** file_list.c for epitar in /goinfre/yo/epitar/src
**
** Made by yoann amsellem
** Login   <amsell_y@epita.fr>
**
** Started on  Wed Jul  5 23:44:15 2006 yoann amsellem
** Last update Sat Jul  8 17:35:02 2006 yoann amsellem
*/

#include "epitar.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/dirent.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

static int	is_skiped(char *file_name)
{
  if (!strcmp(file_name, "..") ||
      !strcmp(file_name, "."))
    return 1;
  return 0;
}

static void	close_and_free(DIR *dir,
                               struct dirent *struct1,
                               struct stat  *struct2)
{
  if (dir != NULL)
    closedir(dir);
  if (struct1 != NULL)
    free(struct1);
  if (struct2 != NULL)
    free(struct2);
}

static struct s_chain_header	*get_new_chain(char *path, int is_dir)
{
  struct s_chain_header		*new = NULL;

  if (NULL == (new = malloc(sizeof (struct s_chain_header))))
    return NULL;
  new->is_dir = is_dir;
  new->path = path;
  return new;
}

char			*sep_path(const char *path,
				  struct dirent *dir_to_add,
				  int is_sep_before)
{
  int			lg_path = 0;
  int			cpt = 0;
  char			*new_path = NULL;
  char			separator = '/';

  if (path == NULL || dir_to_add == NULL)
    return NULL;
  lg_path = strlen(path);
  new_path = malloc((lg_path + dir_to_add->d_namlen + 2) * sizeof (char));
  for (cpt = 0; path[cpt]; ++cpt)
    new_path[cpt] = path[cpt];
  if (is_sep_before)
    new_path[lg_path] = separator;
  for (cpt = is_sep_before; cpt < dir_to_add->d_namlen + is_sep_before; ++cpt)
    new_path[lg_path + cpt] = dir_to_add->d_name[cpt - is_sep_before];
  if (!is_sep_before)
    {
      new_path[lg_path + cpt] = separator;
      new_path[lg_path + cpt + 1] = '\0';
    }
  else
    new_path[lg_path + cpt] = '\0';
  return new_path;
}

struct s_chain_header	*scan_dir(char *path, int is_root)
{
  DIR			*cur_dir = NULL;
  struct dirent		*cur_dirent = NULL;
  struct stat		*cur_stat = NULL;
  struct s_chain_header	*head = NULL;
  struct s_chain_header	*prev = NULL;
  struct s_chain_header	*dir = NULL;

  if ((cur_stat = malloc(sizeof (struct stat))) == NULL || path == NULL ||
      (cur_dir = opendir(path)) == NULL)
    return NULL;
  if (is_root == 1)
  {
    if (stat(path, cur_stat) == 0 &&
        S_ISDIR(cur_stat->st_mode))
      head = get_new_chain(path, 1);
    else
      head = get_new_chain(path, 0);
    head->header = NULL;
    head->previous = NULL;
    prev = head;
  }
  while ((cur_dirent = readdir(cur_dir)) != NULL)
  {
    if (is_skiped(cur_dirent->d_name))
      continue;
    if (stat(sep_path(path, cur_dirent, 1), cur_stat) == 0 &&
	S_ISDIR(cur_stat->st_mode))
    {
      dir = get_new_chain(sep_path(path, cur_dirent, 1), 1);
      dir->previous = prev;
      dir->header = NULL;
      if (prev != NULL)
	prev->next = dir;
      prev = dir;
      if(NULL != (head = scan_dir(sep_path(path, cur_dirent, 1), 0)))
      {
	dir->next = head;
	head->previous = dir;
	while (head->next != NULL)
	  head = head->next;
	prev = head;
      }
      continue;
    }
    else
      head = get_new_chain(sep_path(path, cur_dirent, 1), 0);
    if (prev != NULL)
      prev->next = head;
    head->previous = prev;
    head->header = NULL;
    prev = head;
  }
  if (head != NULL)
    head->next = NULL;
  head = rewind_headers(prev);
  close_and_free(cur_dir, cur_dirent, cur_stat);
  return head;
}
