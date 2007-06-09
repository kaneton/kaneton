##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/check/kaneton/core/event/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [thu jul 20 18:12:03 2006]
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

EVENT_LO		:=		event.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(EVENT_LO)

$(EVENT_LO):		libs
	$(call env_remove,$(EVENT_LO),)

	$(call env_archive,$(EVENT_LO),$(MODULES),)

libs:
	$(call env_launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call env_launch,Makefile,$(SUBDIRS),clear,)

	$(call env_remove,$(EVENT_LO),)

	$(call env_purge,)

proto:
	$(call env_prototypes,"common/common.h",)

dep:
	$(call env_launch,Makefile,$(SUBDIRS),dep,)
