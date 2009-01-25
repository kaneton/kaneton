##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/check/kaneton/core/set/set_array/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [thu jul 20 18:01:25 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../environment/env.mk

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

SET_ARRAY_LO		:=		set_array.lo

#
# ---------- rules ------------------------------------------------------------
#

main:			build run

build:			libs $(SET_ARRAY_LO)

$(SET_ARRAY_LO):	$(MODULES)
	$(call env_remove,$(SET_ARRAY_LO),)

	$(call env_archive,$(SET_ARRAY_LO),$(MODULES),)

libs:
	for d in $(SUBDIRS) ; do $(call env_launch,$${d}/Makefile,,); done

run:

clear:
	for d in $(SUBDIRS) ; do $(call env_launch,$${d}/Makefile,clear,); done

	$(call env_remove,$(SET_ARRAY_LO),)

	$(call env_purge,)

prototypes:

headers:
	$(call env_launch,Makefile,$(SUBDIRS),dep,)
