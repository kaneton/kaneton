	##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/check/kaneton/core/map/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [thu jul 20 18:12:27 2006]
##

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

MAP_LO			:=		map.lo

#
# ---------- rules ------------------------------------------------------------
#

main:			build run

build:			libs $(MAP_LO)

$(MAP_LO):		$(MODULES)
	$(call env_remove,$(MAP_LO),)

	$(call env_archive,$(MAP_LO),$(MODULES),)

libs:
	for d in $(SUBDIRS) ; do $(call env_launch,$${d}/Makefile,,); done

run:

clear:
	for d in $(SUBDIRS) ; do $(call env_launch,$${d}/Makefile,clear,); done

	$(call env_remove,$(MAP_LO),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(shell pwd)"/common/common.h",)

headers:
	$(call env_launch,Makefile,$(SUBDIRS),dep,)
