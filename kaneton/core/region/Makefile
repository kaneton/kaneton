#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton.STABLE/kaneton/core/region/Makefile
#
# created       julien quintard   [sun jun 10 17:00:15 2007]
# updated       julien quintard   [sun nov 28 19:44:38 2010]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/core/region

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

REGION_C		:=		region-fit.c			

REGION_O		:=		$(REGION_C:.c=.o)

REGION_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/region.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(_REGION_LO_)

$(_REGION_LO_):		$(REGION_O)
	$(call env_remove,$(_REGION_LO_),)

	$(call env_archive,$(_REGION_LO_),$(REGION_O),)

clear:
	$(call env_remove,$(REGION_O),)

	$(call env_remove,$(_REGION_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(REGION_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(REGION_C),)

dependencies:

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
