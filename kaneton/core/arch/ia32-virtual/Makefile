##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/core/arch/ia32-virtual/Makefile
##
## created       julien quintard   [fri feb 11 02:57:24 2005]
## updated       matthieu bucchianeri   [sat jun  3 22:57:53 2006]
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
					sched.c

MACHDEP_O		:=		$(MACHDEP_C:.c=.o)

MACHDEP_INCLUDE		:=		$(_MACHDEP_INCLUDE_DIR_)/core/core.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(MACHDEP_LO):		libs $(MACHDEP_O)
	$(call remove,$(MACHDEP_LO),)

	$(call archive,$(MACHDEP_LO),$(MACHDEP_O) $(_LIBIA32_A_),)

libs:
	$(call makefile,$(_LIBIA32_DIR_),)

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
