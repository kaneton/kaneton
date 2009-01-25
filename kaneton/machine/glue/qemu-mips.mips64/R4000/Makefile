#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/enguerrand/...ine/glue/qemu-mips.mips64/R4000/Makefile
#
# created       enguerrand raymond   [sat oct 11 19:32:08 2008]
# updated       enguerrand raymond   [sun jan 25 15:19:14 2009]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/machine/glue

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

GLUE_C			:=		as.c

GLUE_O			:=		$(GLUE_C:.c=.o)

GLUE_INCLUDE		:=		$(_GLUE_INCLUDE_DIR_)/region.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(_GLUE_LO_)

$(_GLUE_LO_):		$(GLUE_O)
	$(call env_remove,$(_GLUE_LO_),)

	$(call env_archive,$(_GLUE_LO_),$(GLUE_O),)

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
