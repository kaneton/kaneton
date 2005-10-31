##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:56:59 2005   mycure
## last update   Fri Oct 29 21:30:57 2004   mycure
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

LIBSTRING_LO		:=		libstring.lo

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
					strdup.c			\
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

$(LIBSTRING_LO):	$(LIBSTRING_O)
	$(call remove,$(LIBSTRING_LO),)

	$(call archive,$(LIBSTRING_LO),$(LIBSTRING_O),)

clear:
	$(call remove,$(LIBSTRING_O),)

	$(call remove,$(LIBSTRING_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(LIBSTRING_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(LIBSTRING_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
