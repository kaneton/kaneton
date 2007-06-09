##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/libs/klibc/libstring/Makefile
##
## created       julien quintard   [fri feb 11 02:56:59 2005]
## updated       matthieu bucchianeri   [tue jan 24 11:58:28 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../env/.env.mk

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

LIBSTRING_INCLUDE	:=		$(_KLIBC_INCLUDE_DIR_)/libstring/libstring.h

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
