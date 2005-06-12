##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/libs/klibc/libc
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:56:59 2005   mycure
## last update   Sun Jun 12 21:20:02 2005   mycure
##

include			../../../.kaneton.mk

LIBNAME			=		libc.a
LIBINCLUDE		=		../include/libc/libc.h

LIBC_O			=		_ctype.o			\
					bcmp.o				\
					bcopy.o				\
					bzero.o				\
					dump.o				\
					index.o				\
					memcmp.o			\
					memcpy.o			\
					memset.o			\
					printf.o			\
					rindex.o			\
					strcat.o			\
					strchr.o			\
					strcmp.o			\
					strcpy.o			\
					strlen.o			\
					strncat.o			\
					strncmp.o			\
					strncpy.o			\
					strtol.o			\
					strtoul.o			\
					sum2.o				

all:			$(LIBC_O)
	$(call ar-o-a,$(LIBNAME),$(LIBC_O),)
	$(call ranlib,$(LIBNAME),)

distclean:
	$(call rm,$(LIBC_O),)

	@$(_PURGE_)

	$(call rm,$(LIBNAME),)

proto:
	$(call proto,$(LIBINCLUDE),)
