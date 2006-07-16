##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/check/libs/klibc/libdata/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       julien quintard   [thu jul 13 11:32:29 2006]
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
	$(call launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(LIBDATA_LO),)

	$(call purge,)

proto:
	$(call launch,Makefile,$(SUBDIRS),proto,)

dep:
