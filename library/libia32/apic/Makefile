
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

APIC_LO			:=		apic.lo

APIC_C			:=		apic.c

APIC_O			:=		$(APIC_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#

$(APIC_LO):		$(APIC_O)
	$(call env_remove,$(APIC_LO),)

	$(call env_archive,$(APIC_LO),$(APIC_O),)

clear:
	$(call env_remove,$(APIC_O),)

	$(call env_remove,$(APIC_LO),)

	$(call env_purge,)

dep:
	$(call env_remove,$(_MAKEFILE_MK_),)

	$(call env_dependencies,$(APIC_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
