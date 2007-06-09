/*
** archive.c for epitar in /u/a1/akadir_a/akadir_a-epitar/src
** 
** Made by abass akadiri
** Login   <akadir_a@epita.fr>
** 
** Started on  Sat Jul  8 17:01:07 2006 abass akadiri
** Last update Sat Jul  8 17:24:18 2006 abass akadiri
*/
#include "my_tar.h"

int		write_file(int fd_dest, char *file_src, int verbose)
{
  struct stat	*sb = NULL;
  struct stat	sb1;
  struct s_tar *tarh;
  struct dirent *dp = NULL;
  DIR		*DirectoryPointer = NULL;
  int		fd_src = 0;
  int		length = 0;
  char		*buffer = NULL;
  int		nbr_block = 0;

  buffer = malloc(sizeof (char) * 512);
  tarh = malloc(sizeof(struct s_tar));
  sb = malloc(sizeof (struct stat));
  stat(file_src, sb);
  fstat(fd_dest, &sb1);
  if (sb1.st_ino == sb->st_ino)
    {
      fprintf(stderr,"epitar: %s is the archive; not dumped\n", file_src);
      return (0);
    }
    if (S_ISDIR(sb->st_mode))
    {
      if (verbose == 1)
	printf("%s\n", creatpath(file_src,""));
      memset(tarh->fname, 0, 100);

      strcpy(tarh->fname, file_src);
      fill_name(tarh, creatpath(file_src, ""));
      memset(tarh->fchksum, ' ', 8);
      fill_header_first(tarh, sb);
      fill_header_two(tarh, sb);
      fill_header_three(tarh, sb);
      length +=  write (fd_dest, tarh, 512);
      nbr_block++;
      DirectoryPointer = opendir(file_src);
      for (dp = readdir(DirectoryPointer); dp != NULL; dp = readdir(DirectoryPointer))
	{
	  if ((!strcmp(dp->d_name, ".")) || (!strcmp(dp->d_name, "..")))
	    continue;
	  else
	    {
	      if (verbose == 1)
		nbr_block +=  write_file(fd_dest, creatpath(file_src, dp->d_name), 1);
	      else
		nbr_block +=  write_file(fd_dest, creatpath(file_src, dp->d_name), 0);
	    }
	}
      closedir(DirectoryPointer);
      free(buffer);
      free(tarh);
      free(sb);
      return nbr_block;
    }
    fd_src = open(file_src, O_RDONLY);
    memset(tarh->fname, 0, 100);
    strcpy(tarh->fname, file_src);
  fill_name(tarh, file_src);
  fill_header_first(tarh, sb);
  fill_header_two(tarh, sb);
  fill_header_three(tarh, sb);
  length +=  write (fd_dest, tarh, 512);
  nbr_block++;
  memset(buffer, 0, 512);
  while (read(fd_src, buffer, 512) > 0)
    {
      nbr_block++;
      length += write(fd_dest, buffer, 512);
      memset(buffer, 0, 512);
    }
  if (verbose == 1)
    printf("%s\n", file_src);
  free(buffer);
  free(tarh);
  free(sb);
  close(fd_src);
  return nbr_block;
}

void		fill_header_first(struct s_tar *header, struct stat *st)
{
  my_convert_oct(st->st_mode, 7, header->fmode);
  header->fmode[1] = '0';
  header->fmode[2] = '0';
  header->fmode[3] = '0';
  my_convert_oct(st->st_uid, 7, header->fuid);
  my_convert_oct(st->st_gid, 7, header->fgid);
  my_convert_oct(st->st_mtime, 11, header->fmtime);
  return ;
}
void		fill_header_two(struct s_tar *header, struct stat *st)
{
  char		typ = 0;

  if ((typ = def_type(st->st_mode)) == DIRTYPE )
    st->st_size = 0;
  header->ftypeflag = typ;
  my_convert_oct(st->st_size, 11, header->fsize);
  memset(header->flinkname, 0x0, 100);
  return ;
}
void		fill_header_three(struct s_tar *header, struct stat *st)
{

  char		*p = NULL;
  struct group	*grp;
  int		chksum = 0;
  int		i = 0;

  memset(header->fchksum, ' ', 8);
  strcpy(header->fmagic, MAGIC);
  strcpy(header->fversion, VERSION);
  strcpy(header->funame,(getpwuid(st->st_uid))->pw_name);
  grp = getgrgid(st->st_gid);
  if (grp != 0)
    strcpy(header->fgname, grp->gr_name);
  strcpy(header->fdevminor, "0000000");
  strcpy(header->fdevmajor, "0000000");
  p = (char *)header;
  for (i = 0; i < 512 ;i++)
    chksum += (0xFF & *p++);
  my_convert_oct(chksum, 6, header->fchksum);
  header->fchksum[6] = '\0';
  return ;
}
