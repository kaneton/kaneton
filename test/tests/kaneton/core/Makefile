#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/test/tests/kaneton/core/Makefile
#
# created       julien quintard   [wed apr 15 04:29:58 2009]
# updated       julien quintard   [thu jan 27 22:20:11 2011]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		test/tests/kaneton/core

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers dependencies

#
# ---------- variables --------------------------------------------------------
#

UNITS			:=		as				\
					id				\
					map				\
					region				\
					segment				\
					scheduler			\
					set				\
					task				\
					thread				\
					timer				

SUBDIRS			:=		$(_TEST_ENGINE_DIR_)		\
					$(UNITS)

MODULES			:=		$(_TEST_ENGINE_LO_)		\
					$(foreach s,$(UNITS),${s}/${s}.lo)

COMPONENT_LO		:=		core.lo

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies
	$(call env_remove,$(COMPONENT_LO),)

	$(call env_archive,$(COMPONENT_LO),$(MODULES),)

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_remove,$(COMPONENT_LO),)

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
