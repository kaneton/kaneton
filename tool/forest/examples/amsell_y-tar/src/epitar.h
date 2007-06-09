/*
** epitar.h for epitar in /goinfre/yo/epitar/src
**
** Made by yoann amsellem
** Login   <amsell_y@epita.fr>
**
** Started on  Tue Jul  4 13:28:51 2006 yoann amsellem
** Last update Sat Jul  8 17:34:29 2006 yoann amsellem
*/

#ifndef EPITAR_H_
# define EPITAR_H_ 1
# define NAME_MAX_SZ 100
# define HEADER_SZ 500
# define HEADER_EXCEED 12
# define CHKSUM_POSI 148
# define CHKSUM_LEN 8

union			u_conv
{
  int			integer;
  char			cara;
};

struct			s_header
{
  char			name[NAME_MAX_SZ];
  char			mode[8];
  char			uid[8];
  char			gid[8];
  char			size[12];
  char			mtime[12];
  char			chksum[CHKSUM_LEN];
  char			typeflag[1];
  char			linkname[NAME_MAX_SZ];
  char			magic[6];
  char			version[2];
  char			uname[32];
  char			gname[32];
  char			devmajor[8];
  char			devminor[8];
  char			prefix[155];
};

struct			s_chain_header
{
  struct s_header	*header;
  char			*path;
  int			is_dir;
  struct s_chain_header	*next;
  struct s_chain_header	*previous;
};

void			untar(const char *filename, int verbose);
void			unreadable(void);
void			bad_cheksum(void);
int			calc_chksum(struct s_header *header);
void			check_chksum(struct s_header *header, int real);
struct s_chain_header	*rewind_headers(struct s_chain_header *ch_header);
void			free_chain(struct s_chain_header *ch_header);
int			is_null_header(struct s_header *header);
struct s_header		*read_header(int fd);
struct s_chain_header	*add_header(struct s_chain_header *chain, int fd);
void			print_filename(struct s_chain_header *header);
int			my_getnbr_base(const char *str, const char *base);
struct s_chain_header	*scan_dir(char *path, int is_root);
char			*tooct(int value);
int			write_tar(int fd, struct s_chain_header *headers);
int			fill_headers(struct s_chain_header *headers);
int			get_mode(char *mod);

#endif /* !EPITAR_H_ */
