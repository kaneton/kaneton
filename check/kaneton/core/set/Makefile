##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/check/kaneton/core/set/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [thu jul 20 17:56:34 2006]
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

SUBDIRS			:=		$(call env_contents,$(LIST),)

MODULES			:=		$(foreach s,$(SUBDIRS),$s/$s.lo)

SET_LO			:=		set.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(SET_LO)

$(SET_LO):		libs
	$(call env_remove,$(SET_LO),)

	$(call env_archive,$(SET_LO),$(MODULES),)

libs:
	$(call env_launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call env_launch,Makefile,$(SUBDIRS),clear,)

	$(call env_remove,$(SET_LO),)

	$(call env_purge,)

proto:
	$(call env_prototypes,"common/common.h",)

headers:
	$(call env_launch,Makefile,$(SUBDIRS),dep,)
