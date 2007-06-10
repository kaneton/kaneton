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

include			../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear prototypes headers

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
	$(call env_remove,$(LIBSTRING_LO),)

	$(call env_archive,$(LIBSTRING_LO),$(LIBSTRING_O),)

clear:
	$(call env_remove,$(LIBSTRING_O),)

	$(call env_remove,$(LIBSTRING_LO),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(LIBSTRING_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(LIBSTRING_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
