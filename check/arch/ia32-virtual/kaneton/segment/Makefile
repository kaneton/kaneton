##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/check/arch/ia32-virtual/kaneton/segment/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       julien quintard   [thu jul 13 11:40:05 2006]
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

SEGMENT_LO		:=		segment.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(SEGMENT_LO)

$(SEGMENT_LO):		libs
	$(call remove,$(SEGMENT_LO),)

	$(call archive,$(SEGMENT_LO),$(MODULES),)

libs:
	$(call launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(SEGMENT_LO),)

	$(call purge,)

proto:
	$(call prototypes,"common/common.h",)

dep:
