#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton.STABLE/kaneton/library/Makefile
#
# created       matthieu bucchianeri   [sat sep  1 12:40:51 2007]
# updated       julien quintard   [sun dec  5 15:21:00 2010]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/library

#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

LIBRARY_C		:=		alloc.c				\
					ctype.c				\
					memcpy.c			\
					memcmp.c			\
					memset.c			\
					format.c			\
					strchr.c			\
					strcmp.c			\
					strcpy.c			\
					strdup.c			\
					strlen.c			\
					strncmp.c			\
					strncpy.c			\
					strrchr.c			\
					strtol.c			\
					strtoul.c			\
					sum2.c

LIBRARY_O		:=		$(LIBRARY_C:.c=.o)

LIBRARY_INCLUDE		:=		$(_LIBRARY_INCLUDE_DIR_)/library.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(_LIBRARY_LO_)

$(_LIBRARY_LO_):	$(LIBRARY_O)
	$(call env_remove,$(_LIBRARY_LO_),)

	$(call env_archive,$(_LIBRARY_LO_),$(LIBRARY_O),)

clear:
	$(call env_remove,$(LIBRARY_O),)

	$(call env_remove,$(_LIBRARY_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(LIBRARY_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(LIBRARY_C),)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
