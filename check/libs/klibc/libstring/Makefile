##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/check/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       julien quintard   [mon nov 28 19:24:49 2005]
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

LIBSTRING_LO		:=		libstring.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(LIBSTRING_LO)

$(LIBSTRING_LO):		libs
	$(call remove,$(LIBSTRING_LO),)

	$(call archive,$(LIBSTRING_LO),$(MODULES),)

libs:
	$(call makefile,$(SUBDIRS),)

run:

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call remove,$(LIBSTRING_LO),)

	$(call purge,)

proto:

dep:
