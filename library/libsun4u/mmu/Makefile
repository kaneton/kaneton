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

include			../../../environment/env.mk

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
	$(call env_remove,$(MMU_LO),)

	$(call env_archive,$(MMU_LO),$(MMU_O),)

clear:
	$(call env_remove,$(MMU_O),)

	$(call env_remove,$(MMU_LO),)

	$(call env_purge,)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(MMU_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
