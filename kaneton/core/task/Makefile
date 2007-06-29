#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/kaneton/core/task/Makefile
#
# created       julien quintard   [sun jun 10 17:09:05 2007]
# updated       julien quintard   [fri jun 29 08:37:28 2007]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/core/task

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers dependencies

#
# ---------- variables --------------------------------------------------------
#

TASK_C			:=		task.c				

TASK_O			:=		$(TASK_C:.c=.o)

TASK_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/task.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(_TASK_LO_)

$(_TASK_LO_):		$(TASK_O)
	$(call env_remove,$(_TASK_LO_),)

	$(call env_archive,$(_TASK_LO_),$(TASK_O),)

clear:
	$(call env_remove,$(TASK_O),)

	$(call env_remove,$(_TASK_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(TASK_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(TASK_C),)

dependencies:
	$(call env_launch,$(_LIBC_DIR_)/Makefile,,)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
