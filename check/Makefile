##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/check/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [wed jan 18 19:10:07 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../env/.env.mk

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

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(_CHECK_LO_)

$(_CHECK_LO_):		libs
	$(call remove,$(_CHECK_LO_),)

	$(call archive,$(_CHECK_LO_),$(MODULES),)

libs:
	$(call makefile,$(SUBDIRS),)

run:

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call remove,$(_CHECK_LO_),)

	$(call purge,)

proto:
	$(call makefile,$(SUBDIRS),proto)

dep:
