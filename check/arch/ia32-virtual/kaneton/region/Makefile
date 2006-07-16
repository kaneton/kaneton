##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/check/arch/ia32-virtual/kaneton/region/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       julien quintard   [thu jul 13 11:41:02 2006]
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

REGION_LO		:=		region.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(REGION_LO)

$(REGION_LO):		libs
	$(call remove,$(REGION_LO),)

	$(call archive,$(REGION_LO),$(MODULES),)

libs:
	$(call launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(REGION_LO),)

	$(call purge,)

proto:
	$(call prototypes,"common/common.h",)


dep:
