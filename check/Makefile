##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/check/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [wed jul 19 20:16:22 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../environment/env.mk

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

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(_CHECK_LO_)

$(_CHECK_LO_):		libs
	$(call env_remove,$(_CHECK_LO_),)

	$(call env_archive,$(_CHECK_LO_),$(MODULES),)

libs:
	$(call env_launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call env_launch,Makefile,$(SUBDIRS),clear,)

	$(call env_remove,$(_CHECK_LO_),)

	$(call env_purge,)

proto:
	$(call env_launch,Makefile,$(SUBDIRS),proto,)

headers:
	$(call env_launch,Makefile,$(SUBDIRS),dep,)
