#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/kaneton/core/clock/Makefile
#
# created       julien quintard   [sun jun 10 17:11:24 2007]
# updated       julien quintard   [fri dec 10 15:28:04 2010]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/core/time

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

CLOCK_C			:=		clock.c				

CLOCK_O			:=		$(CLOCK_C:.c=.o)

CLOCK_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/clock.h	

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(_CLOCK_LO_)

$(_CLOCK_LO_):		$(CLOCK_O)
	$(call env_remove,$(_CLOCK_LO_),)

	$(call env_archive,$(_CLOCK_LO_),$(CLOCK_O),)

clear:
	$(call env_remove,$(CLOCK_O),)

	$(call env_remove,$(_CLOCK_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(CLOCK_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(CLOCK_C),)

dependencies:

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
