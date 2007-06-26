#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/kaneton/core/region/Makefile
#
# created       julien quintard   [sun jun 10 17:00:15 2007]
# updated       julien quintard   [tue jun 26 15:50:59 2007]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers dependencies

#
# ---------- variables --------------------------------------------------------
#

REGION_C		:=		region.c			\
					region-fit.c			

REGION_O		:=		$(REGION_C:.c=.o)

REGION_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/region.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

main:			dependencies $(_REGION_LO_)

$(_REGION_LO_):		$(REGION_O) $(_LIBC_A_)
	$(call env_remove,$(_REGION_LO_),)

	$(call env_archive,$(_REGION_LO_),$(REGION_O),)

clear:
	$(call env_remove,$(REGION_O),)

	$(call env_remove,$(_REGION_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(REGION_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(REGION_C),)

dependencies:
	$(call env_launch,$(_LIBC_DIR_)/Makefile,,)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
