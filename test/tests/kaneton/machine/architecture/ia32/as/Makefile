#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /data/mycure/repo...on/machine/architecture/ia32/as/Makefile
#
# created       julien quintard   [wed apr 15 04:29:58 2009]
# updated       julien quintard   [tue nov 30 16:40:54 2010]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		test/tests/kaneton/machine/architecture/as

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers dependencies

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		mapping				\
					switch				

MODULES			:=		$(foreach s,$(SUBDIRS),${s}/${s}.lo)

AS_LO			:=		as.lo

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(AS_LO)

$(AS_LO):		$(MODULES)
	$(call env_remove,$(AS_LO),)

	$(call env_archive,$(AS_LO),$(MODULES),)

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_remove,$(AS_LO),)

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
