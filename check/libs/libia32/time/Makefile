##
## licence       Kaneton licence
##
## project       kaneton
##
## file          /home/rhino/kaneton/check/libs/libia32/time/Makefile
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
	$(call makefile,$(SUBDIRS),)

run:

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call remove,$(TIME_LO),)

	$(call purge,)

proto:
	$(call prototypes,"common/common.h",)

dep:
