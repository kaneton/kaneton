##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/libs/libia32/paging/Makefile
##
## created       matthieu bucchianeri   [fri jan  6 11:45:56 2006]
## updated       matthieu bucchianeri   [fri jan  6 11:49:12 2006]
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

PAGING_LO		:=		paging.lo

PAGING_C		:=		paging.c			\
					pd.c				\
					pt.c				\
					tlb.c				\
					page.c

PAGING_O		:=		$(PAGING_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#

$(PAGING_LO):		$(PAGING_O)
	$(call remove,$(PAGING_LO),)

	$(call archive,$(PAGING_LO),$(PAGING_O),)

clear:
	$(call remove,$(PAGING_O),)

	$(call remove,$(PAGING_LO),)

	$(call purge,)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(PAGING_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
