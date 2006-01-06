##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/libs/libia32/interrupt/Makefile
##
## created       matthieu bucchianeri   [fri jan  6 11:45:56 2006]
## updated       matthieu bucchianeri   [fri jan  6 11:50:10 2006]
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

INTERRUPT_LO		:=		interrupt.lo

INTERRUPT_C		:=		interrupt.c			\
					idt.c

INTERRUPT_O		:=		$(INTERRUPT_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#

$(INTERRUPT_LO):		$(INTERRUPT_O)
	$(call remove,$(INTERRUPT_LO),)

	$(call archive,$(INTERRUPT_LO),$(INTERRUPT_O),)

clear:
	$(call remove,$(INTERRUPT_O),)

	$(call remove,$(INTERRUPT_LO),)

	$(call purge,)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(INTERRUPT_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
