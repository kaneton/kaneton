##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton licence
##
## file          /home/enguerrand/kaneton/kaneton/core/arch/sgi-o2.mips64/Makefile
##
## created       enguerrand raymond   [wed oct 18 14:25:13 2006]
## updated       enguerrand raymond   [sun jan  7 23:11:11 2007]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

MACHDEP_LO		:=		machdep.lo

MACHDEP_C		:=		as.c				\
					region.c			\
					segment.c			\
					event.c				\
					timer.c				\
					thread.c			\
					task.c				\
					sched.c				\
					message.c			\
					cpu.c				\
					io.c

MACHDEP_O		:=		$(MACHDEP_C:.c=.o)

MACHDEP_INCLUDE		:=		$(_MACHDEP_INCLUDE_DIR_)/core/core.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(MACHDEP_LO):		libs $(MACHDEP_O)
	$(call env_env_remove,$(MACHDEP_LO),)

	$(call env_env_archive,$(MACHDEP_LO),$(MACHDEP_O) $(_LIBMIPSR10000_A_),)

libs:
	$(call env_env_launch,Makefile,$(_LIBMIPSR10000_DIR_),,)

clear:
	$(call env_env_remove,$(MACHDEP_O),)

	$(call env_env_remove,$(MACHDEP_LO),)

	$(call env_env_purge,)

proto:
	$(call env_env_prototypes,$(MACHDEP_INCLUDE),)

dep:
	$(call env_env_remove,$(_DEPENDENCY_MK_),)

	$(call env_env_dependencies,$(MACHDEP_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
