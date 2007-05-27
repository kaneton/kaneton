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

SET_ARRAY_LO		:=		set_array.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(SET_ARRAY_LO)

$(SET_ARRAY_LO):	libs
	$(call remove,$(SET_ARRAY_LO),)

	$(call archive,$(SET_ARRAY_LO),$(MODULES),)

libs:
	$(call launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(SET_ARRAY_LO),)

	$(call purge,)

proto:

dep:
	$(call launch,Makefile,$(SUBDIRS),dep,)
