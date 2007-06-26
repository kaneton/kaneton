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
# updated       julien quintard   [tue jun 26 15:52:24 2007]
#

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

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

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

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
