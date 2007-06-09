#ifndef     MY_TAR_H
# define    MY_TAR_H

#include    <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#define TL_BLOCK 0x200
#define INVALIDE_OPT 64

struct      s_tar
{
  char      fname[100];
  char      fmode[8];
  char      fuid[8];
  char      fgid[8];
  char      fsize[12];
  char      fmtime[12];
  char      fchksum[8];
  char      ftypeflag;
  char      flinkname[100];
  char      fmagic[6];
  char      fversion[2];
  char      funame[32];
  char      fgname[32];
  char      fdevmajor[8];
  char      fdevminor[8];
  char      fprefix[155];
  char      fgarbage[12];
};




# define MAX_PATH_LEN 0xFF

#define TL_BLOCK 0x200
#define SIZE_TAP 0x2800

#define MAGIC "ustar"
#define VERSION "00"

#define REGTYPE '0'
#define LNKTYPE '1'
#define SYMTYPE '2'
#define CHRTYPE '3'
#define BLKTYPE '4'
#define DIRTYPE '5'
#define FIFOTYPE '6'
#define CONTTYPE '7'

int   write_file(int fd_dest, char *file_src, int verbose);
void    invalid_option(char c);
int   only_tar(int i);
void    fill_header_first(struct s_tar *header, struct stat *st);
void    fill_header_two(struct s_tar *header, struct stat *st);
void    fill_header_three(struct s_tar *header, struct stat *st);
void    fill_name(struct s_tar *tarh, char *str);
char    *creatpath(char *path, char *str) ;
int   my_strlen(char *str);
void    xtract_file_aux(int tarball, int verbose);
void    xtract_files(int fd_tarball, struct s_tar *header, int verbose);
int   exec_opt_line(char *line, int dest, char *source);
int   parse_opt(char *argv);
int   verif_opt(char *argv);
char    is_inside_c(char *str);
void    my_convert_oct(int nb, int len, char *buffer);
char            def_type(int st_mode);
#endif /*MY_TAR_H*/
