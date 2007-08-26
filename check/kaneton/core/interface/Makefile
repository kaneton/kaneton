#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/kaneton/check/kaneton/core/interface/Makefile
#
# created       matthieu bucchianeri   [sun aug 26 16:38:42 2007]
# updated       matthieu bucchianeri   [sun aug 26 16:38:54 2007]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all build run libs clear proto dep

#
# ---------- variables --------------------------------------------------------
#

LIST			:=		list

SUBDIRS			:=		$(shell $(call env_pull,$(LIST),))

MODULES			:=		$(foreach s,$(SUBDIRS),$s/$s.lo)

INTERFACE_LO		:=		interface.lo

#
# ---------- rules ------------------------------------------------------------
#

main:			build run

build:			$(INTERFACE_LO)

$(INTERFACE_LO):		libs
	$(call env_remove,$(INTERFACE_LO),)

	$(call env_archive,$(INTERFACE_LO),$(MODULES),)

libs:
	for d in $(SUBDIRS) ; do $(call env_launch,$${d}/Makefile,,); done

run:

clear:
	for d in $(SUBDIRS) ; do $(call env_launch,$${d}/Makefile,clear,); done

	$(call env_remove,$(INTERFACE_LO),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(shell pwd)"/common/common.h",)

headers:
	$(call env_launch,Makefile,$(SUBDIRS),dep,)
