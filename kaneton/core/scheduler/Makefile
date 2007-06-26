##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/core/sched/Makefile
##
## created       matthieu bucchianeri   [sat jun  3 22:33:32 2006]
## updated       matthieu bucchianeri   [sat jun  3 22:33:33 2006]
##

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

SCHEDULER_C		:=		scheduler.c			\
					$(_SCHEDULER_FILE_)

SCHEDULER_O		:=		$(SCHEDULER_C:.c=.o)

SCHEDULER_INCLUDE	:=		$(_CORE_INCLUDE_DIR_)/scheduler.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

main:			dependencies $(_SCHEDULER_LO_)

$(_SCHEDULER_LO_):		$(SCHEDULER_O) $(_LIBC_A_)
	$(call env_remove,$(_SCHEDULER_LO_),)

	$(call env_archive,$(_SCHEDULER_LO_),$(SCHEDULER_O),)

clear:
	$(call env_remove,$(SCHEDULER_O),)

	$(call env_remove,$(_SCHEDULER_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(SCHEDULER_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(SCHEDULER_C),)

dependencies:
	$(call env_launch,$(_LIBC_DIR_)/Makefile,,)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
