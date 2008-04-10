#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/lec_l/kaneton/kaneton/libc/Makefile
#
# created       matthieu bucchianeri   [sat sep  1 12:40:51 2007]
# updated       laurent lec   [thu apr 10 13:40:30 2008]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/libc

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

LIBC_C			:=		alloc.c				\
					ctype.c				\
					memcpy.c			\
					memcmp.c			\
					memset.c			\
					printf.c			\
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

# allo.c & strdup.c

LIBC_O			:=		$(LIBC_C:.c=.o)

LIBC_INCLUDE		:=		$(_LIBC_INCLUDE_DIR_)/libc.h	\

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(_LIBC_LO_)

$(_LIBC_LO_):		$(LIBC_O)
	$(call env_remove,$(_LIBC_LO_),)

	$(call env_archive,$(_LIBC_LO_),$(LIBC_O),)

clear:
	$(call env_remove,$(LIBC_O),)

	$(call env_remove,$(_LIBC_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(LIBC_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(LIBC_C),)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
