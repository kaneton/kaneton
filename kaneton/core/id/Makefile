#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/kaneton/kaneton/core/id/Makefile
#
# created       julien quintard   [thu jun 28 20:32:29 2007]
# updated       matthieu bucchianeri   [sat sep  1 12:31:01 2007]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/core/id

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

ID_C			:=		id.c

ID_O			:=		$(ID_C:.c=.o)

ID_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/id.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(_ID_LO_)

$(_ID_LO_):		$(ID_O)
	$(call env_remove,$(_ID_LO_),)

	$(call env_archive,$(_ID_LO_),$(ID_O),)

clear:
	$(call env_remove,$(ID_O),)

	$(call env_remove,$(_ID_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(ID_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(ID_C),)

dependencies:

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
