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

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

SCHED_LO		:=		sched.lo

SCHED_C			:=		sched.c		\
					$(_SCHED_FILE_)

SCHED_O			:=		$(SCHED_C:.c=.o)

SCHED_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/sched.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(SCHED_LO):		$(SCHED_O)
	$(call env_remove,$(SCHED_LO),)

	$(call env_archive,$(SCHED_LO),$(SCHED_O),)

clear:
	$(call env_remove,$(SCHED_O),)

	$(call env_remove,$(SCHED_LO),)

	$(call env_purge,)

proto:
	$(call env_prototypes,$(SCHED_INCLUDE),)

dep:
	$(call env_remove,$(_MAKEFILE_MK_),)

	$(call env_dependencies,$(SCHED_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
