##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/check/core/kaneton/id/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       julien quintard   [thu jul 13 11:31:11 2006]
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
	$(call launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(ID_LO),)

	$(call purge,)

proto:
	$(call prototypes,"common/common.h",)

dep:
