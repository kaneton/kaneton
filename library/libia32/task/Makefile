
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

TASK_C			:=		context.c			\
					tss.c

TASK_O			:=		$(TASK_C:.c=.o)

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

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(TASK_C),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
