##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/check/core/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [wed jan 18 19:10:28 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../env/.env.mk

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
	$(call makefile,$(SUBDIRS),)

run:

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call remove,$(CORE_LO),)

	$(call purge,)

proto:
	$(call makefile,$(SUBDIRS),proto)

dep:
