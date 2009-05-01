#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/test/tests/kaneton/.engine/Makefile
#
# created       julien quintard   [wed apr 15 04:30:04 2009]
# updated       julien quintard   [fri may  1 20:50:46 2009]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

ENGINE_C		:=		engine.c
ENGINE_O		:=		$(ENGINE_C:.c=.o)

ENGINE_H		:=		$(directory)/$(ENGINE_C:.c=.h)

ENGINE_LO		:=		$(ENGINE_O:.o=.lo)

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(ENGINE_LO)

$(ENGINE_LO):		$(ENGINE_O)
	$(call env_remove,$(ENGINE_LO),)

	$(call env_archive,$(ENGINE_LO),$(ENGINE_O),)

clear:
	$(call env_remove,$(ENGINE_O),)

	$(call env_remove,$(ENGINE_LO),)

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
