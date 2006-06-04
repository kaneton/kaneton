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

include			../../../env/.env.mk

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
	$(call remove,$(SCHED_LO),)

	$(call archive,$(SCHED_LO),$(SCHED_O),)

clear:
	$(call remove,$(SCHED_O),)

	$(call remove,$(SCHED_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(SCHED_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(SCHED_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
