#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton.STABLE/kaneton/modules/bundle/Makefile
#
# created       julien quintard   [wed apr 15 07:00:50 2009]
# updated       julien quintard   [sat dec  4 23:35:47 2010]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/core/bundle

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

BUNDLE_LO			:=		bundle.lo

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(BUNDLE_LO)

$(BUNDLE_LO):		$(_TEST_STORE_BUNDLE_KANETON_LO_)
	$(call env_remove,$(BUNDLE_LO),)

	$(call env_archive,$(BUNDLE_LO),$(_TEST_STORE_BUNDLE_KANETON_LO_),)

clear:
	$(call env_remove,$(BUNDLE_LO),)

	$(call env_purge,)

prototypes:

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

dependencies:


endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
