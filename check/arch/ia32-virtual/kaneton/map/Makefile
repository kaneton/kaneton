##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/map/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [sun apr  2 19:00:28 2006]
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

MAP_LO			:=		map.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(MAP_LO)

$(MAP_LO):		libs
	$(call remove,$(MAP_LO),)

	$(call archive,$(MAP_LO),$(MODULES),)

libs:
	$(call makefile,$(SUBDIRS),)

run:

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call remove,$(MAP_LO),)

	$(call purge,)

proto:
	$(call prototypes,"common/common.h",)


dep:
