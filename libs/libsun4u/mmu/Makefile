##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/libs/libsun4u/mmu/Makefile
##
## created       matthieu bucchianeri   [thu aug 10 16:01:50 2006]
## updated       matthieu bucchianeri   [mon aug 14 18:43:16 2006]
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

MMU_LO			:=		mmu.lo

MMU_C			:=		mmu.c				\
					tlb.c				\
					tsb.c

MMU_O			:=		$(MMU_C:.c=.o)


#
# ---------- rules ------------------------------------------------------------
#

$(MMU_LO):		$(MMU_O)
	$(call remove,$(MMU_LO),)

	$(call archive,$(MMU_LO),$(MMU_O),)

clear:
	$(call remove,$(MMU_O),)

	$(call remove,$(MMU_LO),)

	$(call purge,)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(MMU_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
