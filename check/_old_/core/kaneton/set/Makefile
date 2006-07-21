##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/check/core/kaneton/set/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       julien quintard   [thu jul 13 11:30:30 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../env/.env.mk

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

SET_LO			:=		set.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(SET_LO)

$(SET_LO):		libs
	$(call remove,$(SET_LO),)

	$(call archive,$(SET_LO),$(MODULES),)

libs:
	$(call launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(SET_LO),)

	$(call purge,)

proto:
	$(call prototypes,"common/common.h",)

dep:
