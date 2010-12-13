#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/kaneton/core/timer/Makefile
#
# created       julien quintard   [sun jun 10 17:11:24 2007]
# updated       julien quintard   [fri dec 10 15:27:32 2010]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/core/timer

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

TIMER_C			:=		timer.c				

TIMER_O			:=		$(TIMER_C:.c=.o)

TIMER_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/timer.h	

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(_TIMER_LO_)

$(_TIMER_LO_):		$(TIMER_O)
	$(call env_remove,$(_TIMER_LO_),)

	$(call env_archive,$(_TIMER_LO_),$(TIMER_O),)

clear:
	$(call env_remove,$(TIMER_O),)

	$(call env_remove,$(_TIMER_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(TIMER_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(TIMER_C),)

dependencies:

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
