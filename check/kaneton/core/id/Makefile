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

ID_LO			:=		id.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(ID_LO)

$(ID_LO):		libs
	$(call env_remove,$(ID_LO),)

	$(call env_archive,$(ID_LO),$(MODULES),)

libs:
	$(call env_launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call env_launch,Makefile,$(SUBDIRS),clear,)

	$(call env_remove,$(ID_LO),)

	$(call env_purge,)

proto:
	$(call env_prototypes,"common/common.h",)

headers:
	$(call env_launch,Makefile,$(SUBDIRS),dep,)
