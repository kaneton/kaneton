#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/services/chiche/libkaneton/Makefile
#
# created       matthieu bucchianeri   [sun aug  5 23:11:44 2007]
# updated       julien quintard   [wed apr 22 12:14:32 2009]
#

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

LIBKANETON_A		:=		libkaneton.a

LIBKANETON_C		:=		messaging.c			\
					libkaneton.c			\
					thread_args.c			\
					thread_state_unlock.c

LIBKANETON_O		:=		$(LIBKANETON_C:.c=.o)

LIBKANETON_INCLUDE	:=		$(_LIBKANETON_INCLUDE_DIR_)/libkaneton.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(LIBKANETON_A)

$(LIBKANETON_A):		$(LIBKANETON_O)
	$(call env_static-library,$(LIBKANETON_A),$(LIBKANETON_O),)

clear:
	$(call env_remove,$(LIBKANETON_O),)

	$(call env_remove,$(LIBKANETON_A),)

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
