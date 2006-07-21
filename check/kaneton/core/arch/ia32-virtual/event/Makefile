##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/check/kaneton/core/arch/ia32-virtual/event/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [wed jul 19 20:23:19 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../../env/.env.mk

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
	$(call launch,Makefile,$(SUBDIRS),dep,)
