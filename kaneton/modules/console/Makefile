#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/kaneton/modules/console/Makefile
#
# created       julien quintard   [wed apr 15 07:00:50 2009]
# updated       julien quintard   [fri dec 10 14:19:12 2010]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/core/console

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

CONSOLE_LO		:=		console.lo

CONSOLE_C		:=		console.c

CONSOLE_O		:=		$(CONSOLE_C:.c=.o)

CONSOLE_INCLUDE		:=		$(directory)/include/console.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(CONSOLE_LO)

$(CONSOLE_LO):		$(CONSOLE_O)
	$(call env_remove,$(CONSOLE_LO),)

	$(call env_archive,$(CONSOLE_LO),$(CONSOLE_O),)

clear:
	$(call env_remove,$(CONSOLE_O),)

	$(call env_remove,$(CONSOLE_LO),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(CONSOLE_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(CONSOLE_C),)

dependencies:

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
