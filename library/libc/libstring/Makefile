#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/library/libc/libstring/Makefile
#
# created       julien quintard   [thu jun 28 20:48:10 2007]
# updated       julien quintard   [thu jun 28 20:48:31 2007]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		libstring

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

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
ifeq ($(behaviour),default)

main:			$(LIBSTRING_LO)

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

	$(call env_headers,$(LIBSTRING_C),)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
