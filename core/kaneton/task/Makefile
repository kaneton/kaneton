##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/core/kaneton/task/Makefile
##
## created       julien quintard   [sat dec 10 13:50:17 2005]
## updated       julien quintard   [sat dec 10 13:50:26 2005]
##

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

TASK_C			:=		task.c				

TASK_O			:=		$(TASK_C:.c=.o)

TASK_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/kaneton/task.h

CFLAGS			+=		-D___kernel

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

proto:
	$(call prototypes,$(TASK_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(TASK_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
