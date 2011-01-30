#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/test/tests/kaneton/Makefile
#
# created       julien quintard   [wed apr 15 04:29:58 2009]
# updated       julien quintard   [thu jan 27 22:19:29 2011]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		test/tests/kaneton

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

COMPONENTS		:=		core				\
					machine				

SUBDIRS			:=		$(_TEST_ENGINE_DIR_)		\
					$(COMPONENTS)

MODULES			:=		$(_TEST_ENGINE_LO_)		\
					$(foreach s,$(COMPONENTS),${s}/${s}.lo)

INVENTORY_C		:=		inventory.c
INVENTORY_O		:=		$(INVENTORY_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies
	$(call env_remove,$(_TEST_STORE_BUNDLE_KANETON_LO_),)

	$(call env_remove,$(INVENTORY_C) $(INVENTORY_O),)
	$(call env_launch,$(_TEST_INVENTORY_SCRIPT_),$(_TEST_TESTS_KANETON_DIR_),)
	$(call env_compile-c,$(INVENTORY_O),$(INVENTORY_C),)

	$(call env_archive,$(_TEST_STORE_BUNDLE_KANETON_LO_),$(MODULES) $(INVENTORY_O),)

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_remove,$(INVENTORY_O) $(INVENTORY_C),)

	$(call env_purge,)

prototypes:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,prototypes,)			; \
	done

headers:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,headers,)			; \
	done

dependencies:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,,)				; \
	done

endif
