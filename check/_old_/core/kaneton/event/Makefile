##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/check/core/kaneton/event/Makefile
##
## created       renaud voltz   [sun mar 19 00:53:01 2006]
## updated       julien quintard   [thu jul 13 11:29:22 2006]
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

EVENT_LO		:=		event.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(EVENT_LO)

$(EVENT_LO):		libs
	$(call remove,$(EVENT_LO),)

	$(call archive,$(EVENT_LO),$(MODULES),)

libs:
	$(call launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(EVENT_LO),)

	$(call purge,)

proto:
	$(call prototypes,"common/common.h",)

dep:
