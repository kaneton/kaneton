##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/check/libs/klibc/libdata/malloc/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       julien quintard   [thu jul 13 11:32:19 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../env/.env.mk

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

MALLOC_LO		:=		malloc.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(MALLOC_LO)

$(MALLOC_LO):		libs
	$(call remove,$(MALLOC_LO),)

	$(call archive,$(MALLOC_LO),$(MODULES),)

libs:
	$(call launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(MALLOC_LO),)

	$(call purge,)

proto:
	$(call prototypes,"common/common.h",)

dep:
