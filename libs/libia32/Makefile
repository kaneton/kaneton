##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/libs/libia32/Makefile
##
## created       matthieu bucchianeri   [tue dec 20 13:40:52 2005]
## updated       matthieu bucchianeri   [tue dec 20 20:23:43 2005]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

LIBIA32_C		:=		pmode.c				\
					gdt.c				\
					ldt.c				\
					paging.c			\
					pde.c				\
					pte.c				\
					tlb.c				\
					interrupt.c			\
					idt.c

LIBIA32_O		:=		$(LIBIA32_C:.c=.o)

LIBIA32_INCLUDE		:=		$(_LIBIA32_INCLUDE_DIR_)/libia32.h

#
# ---------- rules ------------------------------------------------------------
#

$(_LIBIA32_A_):		$(LIBIA32_O)
	$(call static-library,$(_LIBIA32_A_),$(LIBIA32_O),)

clear:
	$(call remove,$(LIBIA32_O),)

	$(call remove,$(_LIBIA32_A_),)

	$(call purge,)

proto:
	$(call prototypes,$(LIBIA32_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(LIBIA32_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
