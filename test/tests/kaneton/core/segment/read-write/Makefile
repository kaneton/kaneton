#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...ests/kaneton/segment/read-write/Makefile
#
# created       julien quintard   [wed apr 15 04:29:58 2009]
# updated       julien quintard   [sat nov  6 17:30:44 2010]
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
					02				

MODULES			:=		$(foreach s,$(SUBDIRS),${s}/${s}.lo)

READWRITE_LO		:=		read-write.lo

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(READWRITE_LO)

$(READWRITE_LO):	$(MODULES)
	$(call env_remove,$(READWRITE_LO),)

	$(call env_archive,$(READWRITE_LO),$(MODULES),)

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_remove,$(READWRITE_LO),)

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
