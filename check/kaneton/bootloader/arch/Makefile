##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/check/kaneton/bootloader/arch/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [wed jul 19 20:15:32 2006]
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

ARCH_LO			:=		arch.lo

#
# ---------- rules ------------------------------------------------------------
#

main:			build run

build:			$(ARCH_LO)

$(ARCH_LO):		libs
	$(call env_remove,$(ARCH_LO),)

	$(call env_archive,$(ARCH_LO),$(MODULES),)

libs:
	for d in $(SUBDIRS) ; do $(call env_launch,$${d}/Makefile,,); done

run:

clear:
	for d in $(SUBDIRS) ; do $(call env_launch,$${d}/Makefile,clear,); done

	$(call env_remove,$(ARCH_LO),)

	$(call env_purge,)

prototypes:
	$(call env_launch,Makefile,$(SUBDIRS),proto,)

headers:
	$(call env_launch,Makefile,$(SUBDIRS),dep,)
