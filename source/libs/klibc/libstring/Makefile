##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/libs/klibc/libstring
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:56:59 2005   mycure
## last update   Sun Jul  3 14:48:13 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../.kaneton.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

LIBSTRING		:=		libstring.a

LIBSTRING_C		:=		ctype.c				\
					bcmp.c				\
					bcopy.c				\
					bzero.c				\
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
					strtoul.c			

LIBSTRING_O		:=		$(LIBSTRING_C:.c=.o)

LIBSTRING_INCLUDE	:=		$(_LIBS_DIR_)/klibc/include/libstring/libstring.h

#
# ---------- rules ------------------------------------------------------------
#

$(LIBSTRING):		$(LIBSTRING_O)
	$(call remove,$(LIBSTRING),)

	$(call archive,$(LIBSTRING),$(LIBSTRING_O),)

	$(call static-linker,$(LIBSTRING),)

clear:
	$(call remove,$(LIBSTRING_O),)

	$(call remove,$(LIBSTRING),)

	$(call purge,)

proto:
	$(call prototypes,$(LIBSTRING_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(LIBSTRING_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

include			./$(_MAKEFILE_MK_)
