##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/check/kaneton/core/arch/ia32-virtual/as/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [thu jul 20 19:42:37 2006]
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

AS_LO			:=		as.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(AS_LO)

$(AS_LO):		libs
	$(call remove,$(AS_LO),)

	$(call archive,$(AS_LO),$(MODULES),)

libs:
	$(call launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(AS_LO),)

	$(call purge,)

proto:
	$(call prototypes,"arch_common/common.h",)

dep:
	$(call launch,Makefile,$(SUBDIRS),dep,)
