#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...TABLE/test/tests/kaneton/region/Makefile
#
# created       julien quintard   [wed apr 15 04:29:58 2009]
# updated       julien quintard   [mon nov  8 12:13:06 2010]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers dependencies

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		flush				\
					get				\
					ia32				\
					reserve				\
					resize				\
					split				\
					write				

MODULES			:=		$(foreach s,$(SUBDIRS),${s}/${s}.lo)

REGION_LO		:=		region.lo

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(REGION_LO)

$(REGION_LO):		$(MODULES)
	$(call env_remove,$(REGION_LO),)

	$(call env_archive,$(REGION_LO),$(MODULES),)

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_remove,$(REGION_LO),)

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
