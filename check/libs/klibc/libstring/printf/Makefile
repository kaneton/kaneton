##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/check/libs/klibc/libstring/printf/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [wed jan 18 19:15:36 2006]
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

PRINTF_LO		:=		printf.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(PRINTF_LO)

$(PRINTF_LO):		libs
	$(call remove,$(PRINTF_LO),)

	$(call archive,$(PRINTF_LO),$(MODULES),)

libs:
	$(call makefile,$(SUBDIRS),)

run:

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call remove,$(PRINTF_LO),)

	$(call purge,)

proto:
	$(call prototypes,"common/common.h",)

dep:
