include ../Makefile.rules
CFLAGS		= $(IPATH)
LDFLAGS		= $(LPATH)
NAME		= ../epitar
#LIB		= -lmyString_NetBSD
SRC		= main.c \
		  header.c \
		  chksum.c \
		  print.c \
		  my_getnbr_base.c \
		  file_list.c \
		  load_header.c \
		  tar.c \
		  untar.c
OBJ		= $(SRC:.c=.o)
INCL		= epitar.h
IPATH		= -I. -I..
LPATH		= -L. -L..
STRIP		= /usr/bin/strip
RM		= /bin/rm -f
MAKE		= make

all: 
	$(MAKE) $(NAME)
$(NAME): $(SRC) $(INCL) $(OBJ)
	$(CC) $(OBJ) $(FLAGS) $(LDFLAGS) -g -o $(NAME)

re: distclean all


clean:
	-$(RM) $(OBJ)
	-$(RM) *~ \#* *core

distclean: clean
	-$(RM) $(NAME)

.c.o:
	$(CC) $(FLAGS) $(CFLAGS) -g -c $<