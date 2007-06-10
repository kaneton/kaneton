#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/kaneton/core/as/Makefile
#
# created       julien quintard   [sun jun 10 15:47:23 2007]
# updated       julien quintard   [sun jun 10 16:05:22 2007]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

AS_C			:=		as.c

AS_O			:=		$(AS_C:.c=.o)

AS_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/as.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

main:			$(_AS_LO_)

$(_AS_LO_):		$(AS_O)
	$(call env_remove,$(_AS_LO_),)

	$(call env_archive,$(_AS_LO_),$(AS_O),)

clear:
	$(call env_remove,$(AS_O),)

	$(call env_remove,$(_AS_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(AS_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(AS_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
