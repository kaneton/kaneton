##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/check/kaneton/core/arch/ia32-virtual/io/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [sun jul 30 18:06:18 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../../env/.env.mk

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

IO_LO			:=		io.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(IO_LO)

$(IO_LO):		libs
	$(call remove,$(IO_LO),)

	$(call archive,$(IO_LO),$(MODULES),)

libs:
	$(call launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(IO_LO),)

	$(call purge,)

proto:
	$(call prototypes,"common/common.h",)

dep:
	$(call launch,Makefile,$(SUBDIRS),dep,)
