#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...est/tests/kaneton/region/resize/Makefile
#
# created       julien quintard   [wed apr 15 04:29:58 2009]
# updated       julien quintard   [mon nov  8 12:19:13 2010]
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

SUBDIRS			:=		01				\
					02				\
					03				

MODULES			:=		$(foreach s,$(SUBDIRS),${s}/${s}.lo)

RESIZE_LO		:=		resize.lo

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(RESIZE_LO)

$(RESIZE_LO):		$(MODULES)
	$(call env_remove,$(RESIZE_LO),)

	$(call env_archive,$(RESIZE_LO),$(MODULES),)

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_remove,$(RESIZE_LO),)

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
