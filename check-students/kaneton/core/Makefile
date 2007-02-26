##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/check/kaneton/core/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [wed jul 19 20:17:38 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../env/.env.mk

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

CORE_LO			:=		core.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(CORE_LO)

$(CORE_LO):		libs
	$(call remove,$(CORE_LO),)

	$(call archive,$(CORE_LO),$(MODULES),)

libs:
	$(call launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(CORE_LO),)

	$(call purge,)

proto:
	$(call launch,Makefile,$(SUBDIRS),proto,)

dep:
	$(call launch,Makefile,$(SUBDIRS),dep,)
