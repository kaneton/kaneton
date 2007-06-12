#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...aneton/machine/glue/ibm-pc.ia32/Makefile
#
# created       julien quintard   [sun jun 10 15:47:23 2007]
# updated       julien quintard   [mon jun 11 05:53:16 2007]
#

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

GLUE_C			:=		timer.c				

GLUE_O			:=		$(GLUE_C:.c=.o)

GLUE_INCLUDE		:=		$(_GLUE_INCLUDE_DIR_)/timer.h	

#
# ---------- rules ------------------------------------------------------------
#

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

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
