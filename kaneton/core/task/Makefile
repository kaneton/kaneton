##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/kaneton/core/task/Makefile
##
## created       julien quintard   [sat dec 10 13:50:17 2005]
## updated       julien quintard   [wed jun 28 11:18:24 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

TASK_LO			:=		task.lo

TASK_C			:=		task.c				

TASK_O			:=		$(TASK_C:.c=.o)

TASK_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/task.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(TASK_LO):		$(TASK_O)
	$(call env_remove,$(TASK_LO),)

	$(call env_archive,$(TASK_LO),$(TASK_O),)

clear:
	$(call env_remove,$(TASK_O),)

	$(call env_remove,$(TASK_LO),)

	$(call env_purge,)

proto:
	$(call env_prototypes,$(TASK_INCLUDE),)

dep:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(TASK_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
