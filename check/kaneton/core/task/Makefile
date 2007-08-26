##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/check/kaneton/core/task/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [thu jul 20 18:13:44 2006]
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

TASK_LO			:=		task.lo

#
# ---------- rules ------------------------------------------------------------
#

main:			build run

build:			$(TASK_LO)

$(TASK_LO):		libs
	$(call env_remove,$(TASK_LO),)

	$(call env_archive,$(TASK_LO),$(MODULES),)

libs:
	for d in $(SUBDIRS) ; do $(call env_launch,$${d}/Makefile,,); done

run:

clear:
	for d in $(SUBDIRS) ; do $(call env_launch,$${d}/Makefile,clear,); done

	$(call env_remove,$(TASK_LO),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(shell pwd)"/common/common.h",)

headers:
	$(call env_launch,Makefile,$(SUBDIRS),dep,)
