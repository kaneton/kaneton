#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...kaneton/machine/platform/ibm-pc/Makefile
#
# created       julien quintard   [wed apr 15 04:29:58 2009]
# updated       julien quintard   [thu nov 18 17:29:19 2010]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers dependencies

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		

MODULES			:=		$(foreach s,$(SUBDIRS),${s}/${s}.lo)

PLATFORM_LO		:=		platform.lo

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(PLATFORM_LO)

$(PLATFORM_LO):		$(MODULES)
	$(call env_remove,$(PLATFORM_LO),)

	$(call env_archive,$(PLATFORM_LO),$(MODULES),)

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_remove,$(PLATFORM_LO),)

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
