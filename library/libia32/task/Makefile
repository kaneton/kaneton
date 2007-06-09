
#
# ---------- dependencies -----------------------------------------------------
#

include			../../../env/.env.mk

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
	$(call remove,$(TASK_LO),)

	$(call archive,$(TASK_LO),$(TASK_O),)

clear:
	$(call remove,$(TASK_O),)

	$(call remove,$(TASK_LO),)

	$(call purge,)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(TASK_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
