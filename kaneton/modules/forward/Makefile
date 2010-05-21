#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/kaneton/modules/forward/Makefile
#
# created       julien quintard   [wed apr 15 07:00:50 2009]
# updated       julien quintard   [mon may 17 11:48:03 2010]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/core/forward

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

FORWARD_LO		:=		forward.lo

FORWARD_C		:=		forward.c

FORWARD_O		:=		$(FORWARD_C:.c=.o)

FORWARD_INCLUDE		:=		$(directory)/include/forward.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(FORWARD_LO)

$(FORWARD_LO):		$(FORWARD_O)
	$(call env_remove,$(FORWARD_LO),)

	$(call env_archive,$(FORWARD_LO),$(FORWARD_O),)

clear:
	$(call env_remove,$(FORWARD_O),)

	$(call env_remove,$(FORWARD_LO),)

	$(call env_purge,)

prototypes:

	$(call env_prototypes,$(FORWARD_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(FORWARD_C),)

dependencies:

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
