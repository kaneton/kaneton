##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/check/kaneton/core/set/set_stack/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [thu jul 20 18:01:22 2006]
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

SUBDIRS			:=		$(call env_contents,$(LIST),)

MODULES			:=		$(foreach s,$(SUBDIRS),$s/$s.lo)

SET_PIPE_LO		:=		set_stack.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(SET_PIPE_LO)

$(SET_PIPE_LO):	libs
	$(call env_remove,$(SET_PIPE_LO),)

	$(call env_archive,$(SET_PIPE_LO),$(MODULES),)

libs:
	$(call env_launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call env_launch,Makefile,$(SUBDIRS),clear,)

	$(call env_remove,$(SET_PIPE_LO),)

	$(call env_purge,)

proto:

headers:
	$(call env_launch,Makefile,$(SUBDIRS),dep,)
