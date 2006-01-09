##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/check/libs/libia32/pmode/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [mon jan  9 11:08:34 2006]
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

PMODE_LO		:=		pmode.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(PMODE_LO)

$(PMODE_LO):		libs
	$(call remove,$(PMODE_LO),)

	$(call archive,$(PMODE_LO),$(MODULES),)

libs:
	$(call makefile,$(SUBDIRS),)

run:

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call remove,$(PMODE_LO),)

	$(call purge,)

proto:

dep:
