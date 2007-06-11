	##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/2008-K1/check/kaneton/check/kaneton/core/as/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [sat mar  3 21:30:50 2007]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all build run libs clear proto dep

#
# ---------- variables --------------------------------------------------------
#

LIST			:=		list

SUBDIRS			:=		$(call env_contents,$(LIST),)

MODULES			:=		$(foreach s,$(SUBDIRS),$s/$s.lo)

AS_LO			:=		as.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			$(AS_LO)

$(AS_LO):		libs
	$(call env_remove,$(AS_LO),)

	$(call env_archive,$(AS_LO),$(MODULES),)

libs:
	$(call env_launch,Makefile,$(SUBDIRS),,)

run:

clear:
	$(call env_launch,Makefile,$(SUBDIRS),clear,)

	$(call env_remove,$(AS_LO),)

	$(call env_purge,)

proto:
	$(call env_prototypes,"common/common.h",)

headers:
	$(call env_launch,Makefile,$(SUBDIRS),dep,)
