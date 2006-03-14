##
## licence       Kaneton licence
##
## project       kaneton
##
## file          /home/rhino/kaneton/check/libs/libia32/interrupt/Makefile
##
## created       renaud voltz   [mon mar 13 12:30:14 2006]
## updated       renaud voltz   [mon mar 13 12:30:14 2006]
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
	$(call makefile,$(SUBDIRS),)

run:

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call remove,$(INTERRUPT_LO),)

	$(call purge,)

proto:
	$(call prototypes,"common/common.h",)

dep:
