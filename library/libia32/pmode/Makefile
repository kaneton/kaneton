##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/libs/libia32/pmode/Makefile
##
## created       matthieu bucchianeri   [fri jan  6 11:45:56 2006]
## updated       matthieu bucchianeri   [fri jan  6 11:47:13 2006]
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

PMODE_LO		:=		pmode.lo

PMODE_C			:=		pmode.c				\
					gdt.c				\
					ldt.c				\
					idt.c

PMODE_O			:=		$(PMODE_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#

$(PMODE_LO):		$(PMODE_O)
	$(call remove,$(PMODE_LO),)

	$(call archive,$(PMODE_LO),$(PMODE_O),)

clear:
	$(call remove,$(PMODE_O),)

	$(call remove,$(PMODE_LO),)

	$(call purge,)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(PMODE_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
