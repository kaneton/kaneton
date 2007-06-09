/*
** header.c for epitar in /goinfre/yo/epitar/src
**
** Made by yoann amsellem
** Login   <amsell_y@epita.fr>
**
** Started on  Wed Jul  5 01:49:04 2006 yoann amsellem
** Last update Sat Jul  8 17:35:24 2006 yoann amsellem
*/

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include "epitar.h"

struct s_chain_header	*rewind_headers(struct s_chain_header *ch_header)
{
  while (ch_header != NULL && ch_header->previous != NULL)
    ch_header = ch_header->previous;
  return ch_header;
}

void			free_chain(struct s_chain_header *ch_header)
{
  if (NULL == (ch_header = rewind_headers(ch_header)))
    return;
  while (ch_header)
  {
    if (ch_header->next != NULL)
      ch_header = ch_header->next;
    if (ch_header->previous != NULL)
    {
      free(ch_header->previous->path);
      free(ch_header->previous->header);
      free(ch_header->previous);
    }
  }
  if (ch_header != NULL)
  {
    free(ch_header->path);
    free(ch_header->header);
    free(ch_header);
  }
}

int			is_null_header(struct s_header *header)
{
  int			cpt = 0;

  if (header == NULL || header->typeflag[0] != 0 || header->version[0] != 0 ||
      header->version[1] != 0)
    return 0;
  for (cpt = 0; cpt < 155; ++cpt)
    if (header->prefix[cpt] != 0)
      return 0;
  for (cpt = 0; cpt < NAME_MAX_SZ; ++cpt)
    if (header->name[cpt] != 0 || header->linkname[cpt] != 0)
      return 0;
  for (cpt = 0; cpt < 32; ++cpt)
    if (header->uname[cpt] != 0 || header->gname[cpt] != 0)
      return 0;
  for (cpt = 0; cpt < 12; ++cpt)
    if (header->size[cpt] != 0 || header->mtime[cpt] != 0)
      return 0;
  for (cpt = 0; cpt < 8; ++cpt)
    if (header->mode[cpt] != 0 || header->uid[cpt] != 0 ||
	header->gid[cpt] != 0 || header->chksum[cpt] != 0 ||
	header->devmajor[cpt] != 0 || header->devminor[cpt] != 0)
      return 0;
  for (cpt = 0; cpt < 6; ++cpt)
    if (header->magic[cpt] != 0)
      return 0;
  return 1;
}

struct s_header		*read_header(int fd)
{
  struct s_header	*header = NULL;
  ssize_t		sz_red = 0;

  if (fd < 0 || NULL == (header = malloc(sizeof (struct s_header))))
    return NULL;
  if (0 == (sz_red = read(fd, header, sizeof (struct s_header))) ||
      sz_red != sizeof (struct s_header))
  {
    free(header);
    return NULL;
  }
  lseek(fd, (512 - sz_red), SEEK_CUR);
  return header;
}

static void		load_path(struct s_chain_header *chain)
{
  int			cpt = 0;
  int			pref_len = 0;
  int			name_len = 0;

  if (chain == NULL || chain->header == NULL)
    return;
  while (pref_len < 155 && chain->header->prefix[pref_len] != '\0')
    ++pref_len;
  while (name_len < NAME_MAX_SZ && chain->header->name[name_len] != '\0')
    ++name_len;
  if (NULL == (chain->path = malloc((name_len + pref_len + 4) * sizeof (char))))
    return;
  for (cpt = 0; cpt < pref_len; ++cpt)
    chain->path[cpt] = chain->header->prefix[cpt];
  if (pref_len > 0 && name_len != NAME_MAX_SZ &&
      chain->path[pref_len - 1] != '/')
    chain->path[pref_len++] = '/';
  for (cpt = 0; cpt < name_len; ++cpt)
    chain->path[pref_len + cpt] = chain->header->name[cpt];
  if (chain->path[pref_len + cpt - 1] != '\0')
    chain->path[pref_len + cpt] = '\0';
}

struct s_chain_header	*add_header(struct s_chain_header *chain, int fd)
{
  struct s_chain_header	*new = NULL;

  if (NULL == (new = malloc(sizeof (struct s_chain_header))))
    return NULL;
  new->previous = chain;
  new->next = NULL;
  if (chain != NULL)
  {
    while (chain->next != NULL)
      chain = chain->next;
    chain->next = new;
  }
  new->header = read_header(fd);
  load_path(new);
  return new;
}
