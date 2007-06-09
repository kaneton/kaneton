/*
** my_tar.h for epitar in /u/a1/ah-tec_m/ah-tec_m-tar/src
**
** Made by mathieu ah-tec
** Login   <ah-tec_m@epita.fr>
**
** Started on  Sat Jul  8 17:36:14 2006 mathieu ah-tec
** Last update Sat Jul  8 17:36:16 2006 mathieu ah-tec
*/

#ifndef MY_TAR_H_
# define MY_TAR_H_
# include <sys/stat.h>

void		copy_filename(char *buf, char *file, char *stack[255], int i);
void		copy_filemode(char *buf, struct stat *st);
void		copy_uid(char *buf, struct stat *st);
void		copy_gid(char *buf, struct stat *st);
void		copy_size(char *buf, struct stat *st);
void		copy_time(char *buf, struct stat *st);
void	copy_typeflag(char *buf, struct stat *st);
void	copy_linkname(char *buf, struct stat *st);
void	copy_magic(char *buf);
void	copy_version(char *buf);
void	copy_uname(char *buf, struct stat *st);
void	copy_gname(char *buf, struct stat *st);
void	copy_checksum(char *buf);
int		compress_tar(int argc, char *argv[], char *str);
int	extract_tar(char *str);


#endif
