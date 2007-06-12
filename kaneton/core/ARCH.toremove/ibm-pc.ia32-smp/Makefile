#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/kaneton/core/arch/machdep/Makefile
#
# created       julien quintard   [sun dec 10 18:38:17 2006]
# updated       julien quintard   [sun dec 10 18:38:53 2006]
#

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
	$(call env_remove,$(MACHDEP_LO),)

	$(call env_archive,$(MACHDEP_LO),$(MACHDEP_O) $(_LIBIA32_A_),)

libs:
	$(call env_launch,Makefile,$(_LIBIA32_DIR_),,)

clear:
	$(call env_remove,$(MACHDEP_O),)

	$(call env_remove,$(MACHDEP_LO),)

	$(call env_purge,)

proto:
	$(call env_prototypes,$(MACHDEP_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(MACHDEP_C),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
