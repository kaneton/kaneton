##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/libs/libia32/interrupt/Makefile
##
## created       matthieu bucchianeri   [fri jan  6 11:45:56 2006]
## updated       matthieu bucchianeri   [wed oct 11 22:59:19 2006]
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

INTERRUPT_LO		:=		interrupt.lo

INTERRUPT_C		:=		pic.c			\
					interrupt.c		\
					ipi.c


INTERRUPT_O		:=		$(INTERRUPT_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#

$(INTERRUPT_LO):		$(INTERRUPT_O)
	$(call env_remove,$(INTERRUPT_LO),)

	$(call env_archive,$(INTERRUPT_LO),$(INTERRUPT_O),)

clear:
	$(call env_remove,$(INTERRUPT_O),)

	$(call env_remove,$(INTERRUPT_LO),)

	$(call env_purge,)

dep:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(INTERRUPT_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
