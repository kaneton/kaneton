#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/kaneton/core/event/Makefile
#
# created       julien quintard   [thu jun 28 20:32:08 2007]
# updated       julien quintard   [thu jun 28 20:44:45 2007]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		event

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

EVENT_C			:=		event.c

EVENT_O			:=		$(EVENT_C:.c=.o)

EVENT_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/event.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(_EVENT_LO_)

$(_EVENT_LO_):		$(EVENT_O) $(_LIBC_A_)
	$(call env_remove,$(_EVENT_LO_),)

	$(call env_archive,$(_EVENT_LO_),$(EVENT_O),)

clear:
	$(call env_remove,$(EVENT_O),)

	$(call env_remove,$(_EVENT_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(EVENT_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(EVENT_C),)

dependencies:
	$(call env_launch,$(_LIBC_DIR_)/Makefile,,)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
