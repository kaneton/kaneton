NAME 		= epitar
SRCS 		= write_file.c\
		  error.c\
		  creatpath.c\
		  my_tar.c\
		  extract.c\
		  main.c\
		  archive.c

OBJS 		= $(SRCS:.c=.o)
INCLS 		=my_tar.h

IPATH 		= -I. -I..
LPATH 		= -L. -L..
CC_i386 	= cc
CC_i686 	= cc
CC_NetBSD 	= cc
CC_alpha 	= cc
CC_sun4 	= /opt/SUNWSpro/bin/CC

MAKE 		= make
STRIP 		= /usr/bin/strip
RM		= /bin/rm -f
CFLAGS 		= $(IPATH)
LDFLAGS 	= $(LPATH)

all	:$(HOSTTYPE)

i386	:intel

i686	:intel

NetBSD	:intel

intel	:
	  $(MAKE) TARGET_FLAGS="-Wall -Werror -W -ansi -g -pedantic -O" $(NAME)

alpha	:
	$(MAKE) TARGET_FLAGS="-w2 -std1 -warnprotos" $(NAME)
sun4	:
	$(MAKE) TARGET_FLAGS="-xarch=v9 -Xc -errwarn=%all" $(NAME)

$(NAME)	: $(SRCS) $(INCLS) $(OBJS)
	$(CC_$(HOSTTYPE)) $(OBJS) $(TARGET_FLAGS) $(LDFLAGS) -o ../$(NAME)

re	: distclean all

clean 	:
	-$(RM) $(OBJS)
	-$(RM) *~ \#* *core

distclean: clean
	-$(RM) ../$(NAME)

.c.o	:
	$(CC_$(HOSTTYPE)) $(TARGET_FLAGS) $(CFLAGS) -c $<