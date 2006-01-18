##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/check/libs/klibc/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [wed jan 18 19:11:37 2006]
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

KLIBC_LO		:=		klibc.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(KLIBC_LO)

$(KLIBC_LO):		libs
	$(call remove,$(KLIBC_LO),)

	$(call archive,$(KLIBC_LO),$(MODULES),)

libs:
	$(call makefile,$(SUBDIRS),)

run:

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call remove,$(KLIBC_LO),)

	$(call purge,)

proto:
	$(call makefile,$(SUBDIRS),proto)

dep:
