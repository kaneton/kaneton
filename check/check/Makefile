##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/check/check/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       julien quintard   [thu jul 13 11:42:03 2006]
##

#
# ---------- component --------------------------------------------------------
#

component		:=		test/check

#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

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

CHECK_LO		:=		check.lo

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			build run

build:			$(CHECK_LO)

$(CHECK_LO):		libs
	$(call env_remove,$(CHECK_LO),)

	$(call env_archive,$(CHECK_LO),$(MODULES),)

libs:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,,)				; \
	done

run:

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_remove,$(CHECK_LO),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,"common/common.h",)

headers:

endif
