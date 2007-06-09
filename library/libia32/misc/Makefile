
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

MISC_LO			:=		misc.lo

MISC_C			:=		cpuid.c			\
					extensions.c

MISC_O			:=		$(MISC_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#

$(MISC_LO):		$(MISC_O)
	$(call remove,$(MISC_LO),)

	$(call archive,$(MISC_LO),$(MISC_O),)

clear:
	$(call remove,$(MISC_O),)

	$(call remove,$(MISC_LO),)

	$(call purge,)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(MISC_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
