##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/check/arch/ia32-virtual/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       julien quintard   [thu jul 13 11:41:37 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../env/.env.mk

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

IA32VIRTUAL_LO		:=		ia32-virtual.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(IA32VIRTUAL_LO)

$(IA32VIRTUAL_LO):		libs
	$(call remove,$(IA32VIRTUAL_LO),)

	$(call archive,$(IA32VIRTUAL_LO),$(MODULES),)

libs:
	$(call launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(IA32VIRTUAL_LO),)

	$(call purge,)

proto:
	$(call launch,Makefile,$(SUBDIRS),proto,)

dep:
