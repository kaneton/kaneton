#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton.STABLE/kaneton/core/segment/Makefile
#
# created       julien quintard   [sun jun 10 17:02:46 2007]
# updated       julien quintard   [sun nov 28 19:44:24 2010]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/core/segment

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

SEGMENT_C		:=		segment-fit.c			

SEGMENT_O		:=		$(SEGMENT_C:.c=.o)

SEGMENT_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/segment.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(_SEGMENT_LO_)

$(_SEGMENT_LO_):	$(SEGMENT_O)
	$(call env_remove,$(_SEGMENT_LO_),)

	$(call env_archive,$(_SEGMENT_LO_),$(SEGMENT_O),)

clear:
	$(call env_remove,$(SEGMENT_O),)

	$(call env_remove,$(_SEGMENT_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(SEGMENT_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(SEGMENT_C),)

dependencies:

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
