##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/check/arch/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [wed jan 18 19:58:39 2006]
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

ARCH_LO			:=		arch.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(ARCH_LO)

$(ARCH_LO):		libs
	$(call remove,$(ARCH_LO),)

	$(call archive,$(ARCH_LO),$(MODULES),)

libs:
	$(call makefile,$(SUBDIRS),)

run:

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call remove,$(ARCH_LO),)

	$(call purge,)

proto:
	$(call makefile,$(SUBDIRS),)

dep:
