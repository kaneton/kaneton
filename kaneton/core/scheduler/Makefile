#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton.STABLE/kaneton/core/scheduler/Makefile
#
# created       julien quintard   [thu jun 28 20:35:48 2007]
# updated       julien quintard   [sun nov 28 19:44:50 2010]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/core/scheduler

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

SCHEDULER_C		:=		scheduler-mfq.c

SCHEDULER_O		:=		$(SCHEDULER_C:.c=.o)

SCHEDULER_INCLUDE	:=		$(_CORE_INCLUDE_DIR_)/scheduler.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(_SCHEDULER_LO_)

$(_SCHEDULER_LO_):		$(SCHEDULER_O)
	$(call env_remove,$(_SCHEDULER_LO_),)

	$(call env_archive,$(_SCHEDULER_LO_),$(SCHEDULER_O),)

clear:
	$(call env_remove,$(SCHEDULER_O),)

	$(call env_remove,$(_SCHEDULER_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(SCHEDULER_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(SCHEDULER_C),)

dependencies:

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
