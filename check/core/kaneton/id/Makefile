##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/check/core/kaneton/id/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [wed jan 18 19:00:24 2006]
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

ID_LO		:=		id.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(ID_LO)

$(ID_LO):		libs
	$(call remove,$(ID_LO),)

	$(call archive,$(ID_LO),$(MODULES),)

libs:
	$(call makefile,$(SUBDIRS),)

run:

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call remove,$(ID_LO),)

	$(call purge,)

proto:
	$(call prototypes,"common/common.h",)

dep:
