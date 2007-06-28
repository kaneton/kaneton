#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/kaneton/core/time/Makefile
#
# created       julien quintard   [sun jun 10 17:11:24 2007]
# updated       julien quintard   [thu jun 28 20:38:10 2007]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		time

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

TIME_C			:=		timer.c

TIME_O			:=		$(TIME_C:.c=.o)

TIME_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/timer.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(_TIME_LO_)

$(_TIME_LO_):		$(TIME_O) $(_LIBC_A_)
	$(call env_remove,$(_TIME_LO_),)

	$(call env_archive,$(_TIME_LO_),$(TIME_O),)

clear:
	$(call env_remove,$(TIME_O),)

	$(call env_remove,$(_TIME_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(TIME_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(TIME_C),)

dependencies:
	$(call env_launch,$(_LIBC_DIR_)/Makefile,,)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
