#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...ABLE/test/tests/kaneton/machine/Makefile
#
# created       julien quintard   [wed apr 15 04:29:58 2009]
# updated       julien quintard   [wed dec  1 05:56:08 2010]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		test/tests/kaneton/machine

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

UNITS			:=						\
				architecture/$(_ARCHITECTURE_)/

#				glue/$(_PLATFORM_).$(_ARCHITECTURE_)/
#				platform/$(_PLATFORM_)/

SUBDIRS			:=		$(_TEST_ENGINE_DIR_)		\
					$(UNITS)

MODULES			:=						\
			$(_TEST_ENGINE_LO_)				\
			architecture/$(_ARCHITECTURE_)/architecture.lo

#			glue/$(_PLATFORM_).$(_ARCHITECTURE_)/glue.lo
#			platform/$(_PLATFORM_)/platform.lo

COMPONENT_LO		:=		machine.lo

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
