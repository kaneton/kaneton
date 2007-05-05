##
## Makefile for bistro in /u/a1/quinta_j/rendu/c/bistro
## 
## Made by julien quintard
## Login   <quinta_j@epita.fr>
## 
## Started on  Tue Sep 14 20:47:46 2004 julien quintard
## Last update Wed Sep 15 21:08:04 2004 julien quintard
##

BISTRO			=		evalexp
BISTRO_SRCS		=		bistro.c			\
					bistro_parser.c			\
					bistro_misc.c			\
					bistro_std.c			\
					bistro_add.c			\
					bistro_sub.c			\
					bistro_mul.c			\
					bistro_div.c			\
					bistro_mod.c			
BISTRO_OBJS		=		$(BISTRO_SRCS:.c=.o)
CC_NetBSD		=		/usr/bin/gcc
CFLAGS_NetBSD		=		-Wall -W -Werror -O -ansi	\
					-pedantic -fomit-frame-pointer
CC_solaris		=		/usr/local/bin/gcc
CFLAGS_solaris		=		-Wall -Werror -ansi -pedantic	\
					-W -O -fomit-frame-pointer -m64
CC_osf1			=		/bin/cc
CFLAGS_osf1		=		-w2 -std1 -warnprotos
CC			=		$(CC_$(OSTYPE))
CFLAGS			=		$(CFLAGS_$(OSTYPE))
RM			=		rm -f

all:		bistro

bistro:		$(BISTRO_OBJS)
	$(CC) $(CFLAGS) -o $(BISTRO) $(BISTRO_OBJS)

clean:
	$(RM) $(BISTRO_OBJS) *~

distclean:	clean
	$(RM) $(BISTRO)
