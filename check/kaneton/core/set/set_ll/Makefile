##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/check/kaneton/core/set/set_ll/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [thu jul 20 18:01:27 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all build run libs clear proto dep

#
# ---------- variables --------------------------------------------------------
#

LIST			:=		list

SUBDIRS			:=		$(call contents,$(LIST),)

MODULES			:=		$(foreach s,$(SUBDIRS),$s/$s.lo)

SET_LL_LO		:=		set_ll.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(SET_LL_LO)

$(SET_LL_LO):	libs
	$(call remove,$(SET_LL_LO),)

	$(call archive,$(SET_LL_LO),$(MODULES),)

libs:
	$(call launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(SET_LL_LO),)

	$(call purge,)

proto:

dep:
	$(call launch,Makefile,$(SUBDIRS),dep,)
