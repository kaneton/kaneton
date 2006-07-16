##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/check/libs/libia32/time/Makefile
##
## created       renaud voltz   [mon mar 13 12:30:14 2006]
## updated       julien quintard   [thu jul 13 11:34:59 2006]
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

TIME_LO		:=		time.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(TIME_LO)

$(TIME_LO):		libs
	$(call remove,$(TIME_LO),)

	$(call archive,$(TIME_LO),$(MODULES),)

libs:
	$(call launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(TIME_LO),)

	$(call purge,)

proto:
	$(call prototypes,"common/common.h",)

dep:
