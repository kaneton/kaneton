##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/check/check/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       julien quintard   [thu jul 13 11:42:03 2006]
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

CHECK_LO			:=		check.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(CHECK_LO)

$(CHECK_LO):		libs
	$(call remove,$(CHECK_LO),)

	$(call archive,$(CHECK_LO),$(MODULES),)

libs:
	$(call launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(CHECK_LO),)

	$(call purge,)

proto:
	$(call prototypes,"common/common.h",)

dep:
