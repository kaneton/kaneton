#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/kaneton/modules/Makefile
#
# created       matthieu bucchianeri   [sat sep  1 12:40:51 2007]
# updated       julien quintard   [fri may  1 19:00:52 2009]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/modules

#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		test

MODULES_LO		:=		$(foreach m,$(SUBDIRS),$(m)/$(m).lo)

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(_MODULES_LO_)

$(_MODULES_LO_):	$(MODULES_LO)
	$(call env_remove,$(_MODULES_LO_),)

	$(call env_archive,$(_MODULES_LO_),$(MODULES_LO),)

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_remove,$(_MODULES_LO_),)

	$(call env_purge,)

prototypes:
	for d in $(_MODULES_) ; do					\
	  $(call env_launch,$${d}/Makefile,prototypes,)			; \
	done

headers:
	for d in $(_MODULES_) ; do					\
	  $(call env_launch,$${d}/Makefile,headers,)			; \
	done

dependencies:
	for d in $(_MODULES_) ; do					\
	  $(call env_launch,$${d}/Makefile,,)				; \
	done

endif
