##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/check/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       julien quintard   [mon nov 28 19:24:49 2005]
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

KANETON_LO		:=		kaneton.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(KANETON_LO)

$(KANETON_LO):		libs
	$(call remove,$(KANETON_LO),)

	$(call archive,$(KANETON_LO),$(MODULES),)

libs:
	$(call makefile,$(SUBDIRS),)

run:

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call remove,$(KANETON_LO),)

	$(call purge,)

proto:

dep:
