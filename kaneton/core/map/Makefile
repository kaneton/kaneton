#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/kaneton/core/map/Makefile
#
# created       julien quintard   [sun jun 10 16:58:56 2007]
# updated       julien quintard   [sun jun 10 16:59:28 2007]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

MAP_C			:=		map.c

MAP_O			:=		$(MAP_C:.c=.o)

MAP_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/map.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

main:			$(_MAP_LO_)

$(_MAP_LO_):		$(MAP_O)
	$(call env_remove,$(_MAP_LO_),)

	$(call env_archive,$(_MAP_LO_),$(MAP_O),)

clear:
	$(call env_remove,$(MAP_O),)

	$(call env_remove,$(_MAP_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(MAP_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(MAP_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
