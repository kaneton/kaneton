
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

MISC_LO			:=		misc.lo

MISC_C			:=		cpuid.c			\
					extensions.c

MISC_O			:=		$(MISC_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#

$(MISC_LO):		$(MISC_O)
	$(call env_remove,$(MISC_LO),)

	$(call env_archive,$(MISC_LO),$(MISC_O),)

clear:
	$(call env_remove,$(MISC_O),)

	$(call env_remove,$(MISC_LO),)

	$(call env_purge,)

dep:
	$(call env_remove,$(_MAKEFILE_MK_),)

	$(call env_dependencies,$(MISC_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
