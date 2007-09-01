#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/kaneton/kaneton/core/interface/Makefile
#
# created       julien quintard   [thu jun 28 20:32:46 2007]
# updated       matthieu bucchianeri   [sat sep  1 12:30:53 2007]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/core/interface

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

INTERFACE_C		:=		interface.c

INTERFACE_O		:=		$(INTERFACE_C:.c=.o)

INTERFACE_INCLUDE	:=		$(_CORE_INCLUDE_DIR_)/interface.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(_INTERFACE_LO_)

$(_INTERFACE_LO_):	$(INTERFACE_O)
	$(call env_remove,$(_INTERFACE_LO_),)

	$(call env_archive,$(_INTERFACE_LO_),$(INTERFACE_O),)

clear:
	$(call env_remove,$(INTERFACE_O),)

	$(call env_remove,$(_INTERFACE_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(INTERFACE_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(INTERFACE_C),)

dependencies:

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
