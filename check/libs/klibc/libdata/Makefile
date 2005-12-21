##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/check/libs/klibc/libdata/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [tue dec 20 15:09:30 2005]
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

LIBDATA_LO		:=		libdata.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(LIBDATA_LO)

$(LIBDATA_LO):		libs
	$(call remove,$(LIBDATA_LO),)

	$(call archive,$(LIBDATA_LO),$(MODULES),)

libs:
	$(call makefile,$(SUBDIRS),)

run:

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call remove,$(LIBDATA_LO),)

	$(call purge,)

proto:

dep:
