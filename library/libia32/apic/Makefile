
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

APIC_LO			:=		apic.lo

APIC_C			:=		apic.c

APIC_O			:=		$(APIC_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#

$(APIC_LO):		$(APIC_O)
	$(call remove,$(APIC_LO),)

	$(call archive,$(APIC_LO),$(APIC_O),)

clear:
	$(call remove,$(APIC_O),)

	$(call remove,$(APIC_LO),)

	$(call purge,)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(APIC_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
