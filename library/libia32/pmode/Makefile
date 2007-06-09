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

include			../../../environment/env.mk

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
	$(call env_remove,$(PMODE_LO),)

	$(call env_archive,$(PMODE_LO),$(PMODE_O),)

clear:
	$(call env_remove,$(PMODE_O),)

	$(call env_remove,$(PMODE_LO),)

	$(call env_purge,)

dep:
	$(call env_remove,$(_MAKEFILE_MK_),)

	$(call env_dependencies,$(PMODE_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
