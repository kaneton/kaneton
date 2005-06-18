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
## last update   Sat Jun 18 13:16:41 2005   mycure
##

#
# dependencies
#

include			../../../.kaneton.mk

#
# directives
#

.PHONY:		clear proto dep

#
# variables
#

LIBC			:=		libc.a

LIBC_C			:=		_ctype.c			\
					bcmp.c				\
					bcopy.c				\
					bzero.c				\
					dump.c				\
					index.c				\
					memcmp.c			\
					memcpy.c			\
					memset.c			\
					printf.c			\
					rindex.c			\
					strcat.c			\
					strchr.c			\
					strcmp.c			\
					strcpy.c			\
					strlen.c			\
					strncat.c			\
					strncmp.c			\
					strncpy.c			\
					strtol.c			\
					strtoul.c			\
					sum2.c

LIBC_O			:=		$(LIBC_C:.c=.o)

LIBC_INCLUDE		:=		../include/libc/libc.h

#
# rules
#

$(LIBC):		$(LIBC_O)
	$(call archive,$(LIBC),$(LIBC_O),)

	$(call static-linker,$(LIBC),)

clear:
	$(call remove,$(LIBC_O),)

	$(call remove,$(LIBC),)

	$(call purge,)

proto:
	$(call prototypes,$(LIBC_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(LIBC_C),$(_MAKEFILE_MK_),)

#
# dependencies
#

include			./$(_MAKEFILE_MK_)
