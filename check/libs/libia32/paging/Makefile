##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/check/libs/libia32/paging/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       julien quintard   [thu jul 13 11:34:42 2006]
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

PAGING_LO		:=		paging.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(PAGING_LO)

$(PAGING_LO):		libs
	$(call remove,$(PAGING_LO),)

	$(call archive,$(PAGING_LO),$(MODULES),)

libs:
	$(call launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(PAGING_LO),)

	$(call purge,)

proto:
	$(call prototypes,"common/common.h",)

dep:
