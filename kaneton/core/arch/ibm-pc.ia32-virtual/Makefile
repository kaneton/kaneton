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

include			../../../../env/.env.mk

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

libs:
	$(call launch,Makefile,$(_LIBIA32_DIR_),,)

	$(call launch,Makefile,.,$(MACHDEP_LO),)

$(MACHDEP_LO):		$(MACHDEP_O)
	$(call remove,$(MACHDEP_LO),)

	$(call archive,$(MACHDEP_LO),$(MACHDEP_O) $(_LIBIA32_A_),)

clear:
	$(call remove,$(MACHDEP_O),)

	$(call remove,$(MACHDEP_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(MACHDEP_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(MACHDEP_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
