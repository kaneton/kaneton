##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/libs/libia32/paging/Makefile
##
## created       matthieu bucchianeri   [fri jan  6 11:45:56 2006]
## updated       matthieu bucchianeri   [mon jan 30 22:33:12 2006]
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

PAGING_LO		:=		paging.lo

PAGING_C		:=		paging.c			\
					pd.c				\
					pt.c				\
					tlb.c

PAGING_O		:=		$(PAGING_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#

$(PAGING_LO):		$(PAGING_O)
	$(call env_remove,$(PAGING_LO),)

	$(call env_archive,$(PAGING_LO),$(PAGING_O),)

clear:
	$(call env_remove,$(PAGING_O),)

	$(call env_remove,$(PAGING_LO),)

	$(call env_purge,)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(PAGING_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
