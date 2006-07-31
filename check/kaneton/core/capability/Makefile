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

CAPABILITY_LO		:=		capability.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(CAPABILITY_LO)

$(CAPABILITY_LO):		libs
	$(call remove,$(CAPABILITY_LO),)

	$(call archive,$(CAPABILITY_LO),$(MODULES),)

libs:
	$(call launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(CAPABILITY_LO),)

	$(call purge,)

proto:
	$(call prototypes,"common/common.h",)

dep:
	$(call launch,Makefile,$(SUBDIRS),dep,)
