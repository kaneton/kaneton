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

SUBDIRS			:=		$(shell $(call env_pull,$(LIST),))

MODULES			:=		$(foreach s,$(SUBDIRS),$s/$s.lo)

AS_LO			:=		as.lo

#
# ---------- rules ------------------------------------------------------------
#

main:			build run

build:			libs $(AS_LO)

$(AS_LO):		$(MODULES)
	$(call env_remove,$(AS_LO),)

	$(call env_archive,$(AS_LO),$(MODULES),)

libs:
	for d in $(SUBDIRS) ; do $(call env_launch,$${d}/Makefile,,); done

run:

clear:
	for d in $(SUBDIRS) ; do $(call env_launch,$${d}/Makefile,clear,); done

	$(call env_remove,$(AS_LO),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(shell pwd)"/common/common.h",)

headers:
	$(call env_launch,Makefile,$(SUBDIRS),dep,)
