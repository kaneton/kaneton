#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton.STABLE/test/engine/Makefile
#
# created       julien quintard   [wed apr 15 04:30:04 2009]
# updated       julien quintard   [mon nov 29 16:53:11 2010]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		test/engine

#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

ENGINE_C			:=		engine.c		

ENGINE_O			:=		$(ENGINE_C:.c=.o)

ENGINE_H			:=		$(_TEST_ENGINE_DIR_)/engine.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(_TEST_ENGINE_LO_)

$(_TEST_ENGINE_LO_):	$(ENGINE_O)
	$(call env_remove,$(_TEST_ENGINE_LO_),)

	$(call env_archive,$(_TEST_ENGINE_LO_),$(ENGINE_O),)

clear:
	$(call env_remove,$(ENGINE_O),)

	$(call env_remove,$(_TEST_ENGINE_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(ENGINE_H),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(ENGINE_C),)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
