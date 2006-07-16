##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/check/libs/libia32/interrupt/Makefile
##
## created       renaud voltz   [mon mar 13 12:30:14 2006]
## updated       julien quintard   [thu jul 13 11:34:50 2006]
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

INTERRUPT_LO		:=		interrupt.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(INTERRUPT_LO)

$(INTERRUPT_LO):		libs
	$(call remove,$(INTERRUPT_LO),)

	$(call archive,$(INTERRUPT_LO),$(MODULES),)

libs:
	$(call launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(INTERRUPT_LO),)

	$(call purge,)

proto:
	$(call prototypes,"common/common.h",)

dep:
