#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/kaneton/check/kaneton/core/io/Makefile
#
# created       matthieu bucchianeri   [sun aug 26 16:38:42 2007]
# updated       matthieu bucchianeri   [sun aug 26 16:39:53 2007]
#

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

IO_LO			:=		io.lo

#
# ---------- rules ------------------------------------------------------------
#

main:			build run

build:			$(IO_LO)

$(IO_LO):		libs
	$(call env_remove,$(IO_LO),)

	$(call env_archive,$(IO_LO),$(MODULES),)

libs:
	for d in $(SUBDIRS) ; do $(call env_launch,$${d}/Makefile,,); done

run:

clear:
	for d in $(SUBDIRS) ; do $(call env_launch,$${d}/Makefile,clear,); done

	$(call env_remove,$(IO_LO),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(shell pwd)"/common/common.h",)

headers:
	$(call env_launch,Makefile,$(SUBDIRS),dep,)
