##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/libs/libia32/tss/Makefile
##
## created       matthieu bucchianeri   [fri jan  6 11:45:56 2006]
## updated       matthieu bucchianeri   [fri jan  6 11:50:35 2006]
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

TSS_LO			:=		tss.lo

TSS_C			:=

TSS_O			:=		$(TSS_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#

$(TSS_LO):		$(TSS_O)
	$(call remove,$(TSS_LO),)

	$(call archive,$(TSS_LO),$(TSS_O),)

clear:
	$(call remove,$(TSS_O),)

	$(call remove,$(TSS_LO),)

	$(call purge,)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(TSS_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
