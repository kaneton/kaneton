##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/check/kaneton/core/capability/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [sun jul 30 17:59:03 2006]
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

CAPABILITY_LO		:=		capability.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(CAPABILITY_LO)

$(CAPABILITY_LO):		libs
	$(call env_remove,$(CAPABILITY_LO),)

	$(call env_archive,$(CAPABILITY_LO),$(MODULES),)

libs:
	$(call env_launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call env_launch,Makefile,$(SUBDIRS),clear,)

	$(call env_remove,$(CAPABILITY_LO),)

	$(call env_purge,)

proto:
	$(call env_prototypes,"common/common.h",)

headers:
	$(call env_launch,Makefile,$(SUBDIRS),dep,)
