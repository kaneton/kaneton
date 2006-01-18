##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/check/libs/klibc/libdata/malloc/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [wed jan 18 19:12:02 2006]
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
	$(call makefile,$(SUBDIRS),)

run:

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call remove,$(MALLOC_LO),)

	$(call purge,)

proto:
	$(call prototypes,"common/common.h",)

dep:
