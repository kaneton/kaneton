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

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

SCHED_C			:=		sched.c				\
					$(_SCHED_FILE_)

SCHED_O			:=		$(SCHED_C:.c=.o)

SCHED_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/sched.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

main:			$(_SCHED_LO_)

$(_SCHED_LO_):		$(SCHED_O)
	$(call env_remove,$(_SCHED_LO_),)

	$(call env_archive,$(_SCHED_LO_),$(SCHED_O),)

clear:
	$(call env_remove,$(SCHED_O),)

	$(call env_remove,$(_SCHED_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(SCHED_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(SCHED_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
