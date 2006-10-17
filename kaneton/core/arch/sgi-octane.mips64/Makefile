##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton licence
##
## file          /home/enguerrand/kaneton/kaneton/core/arch/sgi-octane.mips64/Makefile
##
## created       Enguerrand RAYMOND   [sun oct 15 22:01:33 2006]
## updated       Enguerrand RAYMOND   [tue oct 17 23:44:21 2006]
##

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

$(MACHDEP_LO):		libs $(MACHDEP_O)
	$(call remove,$(MACHDEP_LO),)

	$(call archive,$(MACHDEP_LO),$(MACHDEP_O) $(_LIBOCTANE_A_),)

libs:
	$(call launch,Makefile,$(_LIBOCTANE_DIR_),,)

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
