##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/check/libs/libia32/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [mon jan  9 11:07:21 2006]
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

LIBIA32_LO		:=		libia32.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(LIBIA32_LO)

$(LIBIA32_LO):		libs
	$(call remove,$(LIBIA32_LO),)

	$(call archive,$(LIBIA32_LO),$(MODULES),)

libs:
	$(call makefile,$(SUBDIRS),)

run:

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call remove,$(LIBIA32_LO),)

	$(call purge,)

proto:

dep:
