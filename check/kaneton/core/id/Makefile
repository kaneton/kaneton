##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/check/kaneton/core/id/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [thu jul 20 17:50:20 2006]
##

#
# ---------- component --------------------------------------------------------
#

component		:=		test/kaneton/core/id

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all build run libs clear prototypes dependencies

#
# ---------- variables --------------------------------------------------------
#

LIST			:=		list

SUBDIRS			:=		$(shell $(call env_pull,$(LIST),))

MODULES			:=		$(foreach s,$(SUBDIRS),$s/$s.lo)

ID_LO			:=		id.lo

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			build run

build:			$(ID_LO)

$(ID_LO):		libs
	$(call env_remove,$(ID_LO),)

	$(call env_archive,$(ID_LO),$(MODULES),)

libs:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,,)				; \
	done

run:

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_remove,$(ID_LO),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,"common/common.h",)

headers:
	$(call env_launch,Makefile,$(SUBDIRS),dep,)

endif