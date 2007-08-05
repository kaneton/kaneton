#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/kaneton/library/libkaneton/Makefile
#
# created       matthieu bucchianeri   [sun aug  5 23:11:44 2007]
# updated       matthieu bucchianeri   [sun aug  5 23:22:55 2007]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		libkaneton

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

LIBKANETON_C		:=		messaging.c			\
					libkaneton.c

LIBKANETON_O		:=		$(LIBKANETON_C:.c=.o)

LIBKANETON_INCLUDE	:=		$(_LIBKANETON_INCLUDE_DIR_)/libkaneton.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(_LIBKANETON_A_)

$(_LIBKANETON_A_):		$(LIBKANETON_O)
	$(call env_static-library,$(_LIBKANETON_A_),$(LIBKANETON_O),)

clear:
	$(call env_remove,$(LIBKANETON_O),)

	$(call env_remove,$(_LIBKANETON_A_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(LIBKANETON_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(LIBKANETON_C),)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
