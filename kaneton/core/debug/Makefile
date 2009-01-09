#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/kaneton/kaneton/core/debug/Makefile
#
# created       julien quintard   [thu jun 28 20:31:38 2007]
# updated       matthieu bucchianeri   [sat sep  1 12:31:27 2007]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/core/debug

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers dependencies

#
# ---------- variables --------------------------------------------------------
#

DEBUG_C			:=		debug.c				\
					view.c				\
					gdb.c                           \
                                        callbacks.c                     \
                                        shell.c

DEBUG_O			:=		$(DEBUG_C:.c=.o)

DEBUG_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/debug.h	\
					$(_CORE_INCLUDE_DIR_)/view.h
#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(_DEBUG_LO_)

$(_DEBUG_LO_):		$(DEBUG_O)
	$(call env_remove,$(_DEBUG_LO_),)

	$(call env_archive,$(_DEBUG_LO_),$(DEBUG_O),)

clear:
	$(call env_remove,$(DEBUG_O),)

	$(call env_remove,$(_DEBUG_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(DEBUG_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(DEBUG_C),)

dependencies:

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
