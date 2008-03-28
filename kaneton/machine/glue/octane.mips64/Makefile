#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/lec_l/kanet...eton/machine/glue/octane.mips64/Makefile
#
# created       laurent lec   [fri mar 28 17:20:31 2008]
# updated       laurent lec   [fri mar 28 17:20:42 2008]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/machine/glue

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

GLUE_C			:=		

GLUE_O			:= $(GLUE_C:.c=.o)

GLUE_INCLUDE		:=		

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(_GLUE_LO_)

$(_GLUE_LO_):		$(GLUE_O)

	$(call env_remove,$(_GLUE_LO_),)

	touch $@
#	$(call env_archive,$(_GLUE_LO_),$(GLUE_O),)

clear:
	$(call env_remove,$(GLUE_O),)

	$(call env_remove,$(_GLUE_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(GLUE_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(GLUE_C),)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
