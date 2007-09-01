#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/kaneton/kaneton/core/capability/Makefile
#
# created       julien quintard   [thu jun 28 20:30:19 2007]
# updated       matthieu bucchianeri   [sat sep  1 12:31:47 2007]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/core/capability

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

CAPABILITY_C		:=		capability.c

CAPABILITY_O		:=		$(CAPABILITY_C:.c=.o)

CAPABILITY_INCLUDE	:=		$(_CORE_INCLUDE_DIR_)/capability.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(_CAPABILITY_LO_)

$(_CAPABILITY_LO_):	$(CAPABILITY_O)
	$(call env_remove,$(_CAPABILITY_LO_),)

	$(call env_archive,$(_CAPABILITY_LO_),$(CAPABILITY_O),)

clear:
	$(call env_remove,$(CAPABILITY_O),)

	$(call env_remove,$(_CAPABILITY_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(CAPABILITY_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(CAPABILITY_C),)

dependencies:

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
